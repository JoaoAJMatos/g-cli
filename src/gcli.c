#include "../include/gcli.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

typedef struct {
      int a;
      int b;
      int c;
} color_t;

typedef struct {
      color_t color;
      int alpha;
} color_alpha_t;

static color_t rgb_to_color_t(rgb_t rgb_color) {
      color_t color;
      color.a = rgb_color.r;
      color.b = rgb_color.g;
      color.c = rgb_color.b;
      return color;
}

static color_t hsl_to_color_t(hsl_t hsl_color) {
      color_t color;
      color.a = hsl_color.h;
      color.b = hsl_color.s;
      color.c = hsl_color.l;
      return color;
}

/**
 * @brief Loads the terminal colors from the file.
 * 
 * Returns the number of colors loaded or -1 if the file could not be opened.
 * 
 * @param filename char*
 * @param colors int[][3]
 * @param max_colors int 
 * @return int 
 */
static int load_colors(char *filename, int colors[][3], int max_colors) {
      int i = 0;
      int r, g, b;
      int scanf_result;

      FILE *fp = fopen(filename, "r");
      if (fp == NULL) return GCLI_ERR;

      while (i < max_colors) {
            scanf_result = fscanf(fp, "%d %d %d", &r, &g, &b);
            if (scanf_result != 3) break;

            colors[i][0] = r;
            colors[i][1] = g;
            colors[i][2] = b;
            i++;
      }
      fclose(fp);
      return i;
}

/**
 * @brief Converts a given RGB color to HSL
 * 
 * @param rgb_color rgb_t
 * @param hsl_color hsl_t* 
 */
static void rgb_to_hsl(rgb_t rgb_color, hsl_t *hsl_color) {
      float r01 = rgb_color.r/255.0f;
      float g01 = rgb_color.g/255.0f;
      float b01 = rgb_color.b/255.0f;

      float cmax = r01;
      float cmin = r01;

      if (g01 > cmax) cmax = g01;
      if (b01 > cmax) cmax = b01;

      if (g01 < cmin) cmin = g01;
      if (b01 < cmin) cmin = b01;

      float delta = cmax - cmin;
      float epsilon = 1e-6;
      float hf = 0;

      if (delta < epsilon) hf = 0;
      else if (cmax == r01) hf = 60.0f*fmod((g01 - b01)/delta, 6.0f);
      else if (cmax == g01) hf = 60.0f*((b01 - r01)/delta + 2);
      else if (cmax == b01) hf = 60.0f*((r01 - g01)/delta + 4);
      else unreachable();

      float lf = (cmax + cmin)/2;

      float sf = 0;
      if (delta < epsilon) sf = 0;
      else sf = delta/(1 - fabsf(2*lf - 1));

      hsl_color->h = fmodf(fmodf(hf, 360.0f) + 360.0f, 360.0f);
      hsl_color->s = sf*100.0f;
      hsl_color->l = lf*100.0f;
}

/**
 * @brief Returns the distance between two colors.
 * 
 * @param color
 * @param color
 * @return int 
 */
static int distance_256(color_t color_one, color_t color_two) {
      int distance_a = color_one.a - color_two.a;
      int distance_r = color_one.b - color_two.b;
      int distance_g = color_one.c - color_two.c;
      return distance_a*distance_a + distance_r*distance_r + distance_g*distance_g;
}

/**
 * @brief Finds the closest ANSI color to the given color.
 * 
 * Returns the index of the closest color found.
 * 
 * @return int 
 */
static int find_closest_color_index(struct gcli* gcli, color_t color, color_family_t color_family) {
      int index = 0;
      for (int i = 0; i < MAX_COLORS; i++) {
            if (color_family == RGB) {
                  if (distance_256(color, rgb_to_color_t(gcli->rgb_colors[i])) < distance_256(color, rgb_to_color_t(gcli->rgb_colors[index]))) {
                        index = i;
                  }
            } else if (color_family == HSL) {
                  if (distance_256(color, hsl_to_color_t(gcli->hsl_colors[i])) < distance_256(color, hsl_to_color_t(gcli->hsl_colors[index]))) {
                        index = i;
                  }
            }
      }
      return index;
}

/* ============================================================== */


/**
 * @brief Initializes the gcli library.
 * 
 * Loads the terminal colors from the files and stores them in the gcli struct.
 * 
 * @param gcli struct gcli*
 * @return Errno
 */
Errno gcli_init(struct gcli* gcli) {
      int rgb_colors[MAX_COLORS][3];
      int hsl_colors[MAX_COLORS][3];

      int rgb_colors_count = load_colors("../common/rgb256.txt", rgb_colors, MAX_COLORS);
      int hsl_colors_count = load_colors("../common/hsl256.txt", hsl_colors, MAX_COLORS);

      if (rgb_colors_count == -1 || hsl_colors_count == -1) {
            gcli->last_error = "Could not open colors file.";
            return GCLI_ERR;
      }

      for (int i = 0; i < rgb_colors_count; i++) {
            gcli->rgb_colors[i].r = rgb_colors[i][0];
            gcli->rgb_colors[i].g = rgb_colors[i][1];
            gcli->rgb_colors[i].b = rgb_colors[i][2];
      }

      for (int i = 0; i < hsl_colors_count; i++) {
            gcli->hsl_colors[i].h = hsl_colors[i][0];
            gcli->hsl_colors[i].s = hsl_colors[i][1];
            gcli->hsl_colors[i].l = hsl_colors[i][2];
      }

      gcli->width = DEFAULT_WIDTH;
      gcli->height = 0;
      gcli->resized_width = DEFAULT_WIDTH;
      gcli->resized_height = 0;

      return GCLI_OK;
}


/**
 * @brief Prints an image from a file into the terminal.
 * 
 * @param gcli 
 * @param filename 
 * @param color_type 
 * @return Errno 
 */
Errno gcli_img_to_terminal(struct gcli* gcli, const char* filename, color_family_t color_type, int resized_width) {
      uint32_t *pixels = NULL;
      uint32_t *resized_pixels = NULL;

      pixels = (uint32_t*)stbi_load(filename, &gcli->width, &gcli->height, NULL, 4);
      if (pixels == NULL) {
            gcli->last_error = "Could not open image file.";
            return GCLI_ERR;
      }

      gcli->resized_width = resized_width;
      gcli->resized_height = gcli->height * gcli->resized_width / gcli->width;

      resized_pixels = malloc(gcli->resized_width * gcli->resized_height * sizeof(uint32_t));
      if (resized_pixels == NULL) {
            gcli->last_error = "Could not allocate memory for resized pixels.";
            return GCLI_ERR;
      }

      stbir_resize_uint8((uint8_t*)pixels, gcli->width, gcli->height, 0, (uint8_t*)resized_pixels, gcli->resized_width, gcli->resized_height, 0, 4);

      for (int y = 0; y < gcli->resized_height; y++) {
            for (int x = 0; x < gcli->resized_width; x++) {
                  uint32_t pixel = resized_pixels[y*gcli->resized_width + x];
                  color_alpha_t color = { 
                        .color.a = (pixel >> 24) & 0xFF, 
                        .color.b = (pixel >> 16) & 0xFF, 
                        .color.c = (pixel >> 8) & 0xFF, 
                        .alpha = pixel & 0xFF 
                  };

                  rgb_t rgb_color = {
                        .r = color.alpha * color.color.a / 255,
                        .g = color.alpha * color.color.b / 255,
                        .b = color.alpha * color.color.c / 255
                  };

                  if (color_type == RGB) {
                        print_color(find_closest_color_index(gcli, rgb_to_color_t(rgb_color), RGB));
                  } else if (color_type == HSL) {
                        hsl_t hsl_color;
                        rgb_to_hsl(rgb_color, &hsl_color);
                        print_color(find_closest_color_index(gcli, hsl_to_color_t(hsl_color), HSL));
                  }
            }
            printf("\e[0m\n");
      }
      printf("\e[0m\n");

      free(resized_pixels);
      stbi_image_free(pixels);
      return GCLI_OK;
}


void gcli_print_last_error(struct gcli* gcli) {
      printf("%s\n", gcli->last_error);
}