#include "../include/gcli/gcli.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/thirdparty/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../include/thirdparty/stb_image_resize.h"

#define CHAR_HEIGHT 32
#define CHAR_WIDTH  16

static int colors[MAX_COLORS][3] = {
      {0,0,0},
      {128,0,0},
      {0,128,0},
      {128,128,0},
      {0,0,128},
      {128,0,128},
      {0,128,128},
      {192,192,192},
      {128,128,128},
      {255,0,0},
      {0,255,0},
      {255,255,0},
      {0,0,255},
      {255,0,255},
      {0,255,255},
      {255,255,255},
      {0,0,0},
      {0,0,95},
      {0,0,135},
      {0,0,175},
      {0,0,215},
      {0,0,255},
      {0,95,0},
      {0,95,95},
      {0,95,135},
      {0,95,175},
      {0,95,215},
      {0,95,255},
      {0,135,0},
      {0,135,95},
      {0,135,135},
      {0,135,175},
      {0,135,215},
      {0,135,255},
      {0,175,0},
      {0,175,95},
      {0,175,135},
      {0,175,175},
      {0,175,215},
      {0,175,255},
      {0,215,0},
      {0,215,95},
      {0,215,135},
      {0,215,175},
      {0,215,215},
      {0,215,255},
      {0,255,0},
      {0,255,95},
      {0,255,135},
      {0,255,175},
      {0,255,215},
      {0,255,255},
      {95,0,0},
      {95,0,95},
      {95,0,135},
      {95,0,175},
      {95,0,215},
      {95,0,255},
      {95,95,0},
      {95,95,95},
      {95,95,135},
      {95,95,175},
      {95,95,215},
      {95,95,255},
      {95,135,0},
      {95,135,95},
      {95,135,135},
      {95,135,175},
      {95,135,215},
      {95,135,255},
      {95,175,0},
      {95,175,95},
      {95,175,135},
      {95,175,175},
      {95,175,215},
      {95,175,255},
      {95,215,0},
      {95,215,95},
      {95,215,135},
      {95,215,175},
      {95,215,215},
      {95,215,255},
      {95,255,0},
      {95,255,95},
      {95,255,135},
      {95,255,175},
      {95,255,215},
      {95,255,255},
      {135,0,0},
      {135,0,95},
      {135,0,135},
      {135,0,175},
      {135,0,215},
      {135,0,255},
      {135,95,0},
      {135,95,95},
      {135,95,135},
      {135,95,175},
      {135,95,215},
      {135,95,255},
      {135,135,0},
      {135,135,95},
      {135,135,135},
      {135,135,175},
      {135,135,215},
      {135,135,255},
      {135,175,0},
      {135,175,95},
      {135,175,135},
      {135,175,175},
      {135,175,215},
      {135,175,255},
      {135,215,0},
      {135,215,95},
      {135,215,135},
      {135,215,175},
      {135,215,215},
      {135,215,255},
      {135,255,0},
      {135,255,95},
      {135,255,135},
      {135,255,175},
      {135,255,215},
      {135,255,255},
      {175,0,0},
      {175,0,95},
      {175,0,135},
      {175,0,175},
      {175,0,215},
      {175,0,255},
      {175,95,0},
      {175,95,95},
      {175,95,135},
      {175,95,175},
      {175,95,215},
      {175,95,255},
      {175,135,0},
      {175,135,95},
      {175,135,135},
      {175,135,175},
      {175,135,215},
      {175,135,255},
      {175,175,0},
      {175,175,95},
      {175,175,135},
      {175,175,175},
      {175,175,215},
      {175,175,255},
      {175,215,0},
      {175,215,95},
      {175,215,135},
      {175,215,175},
      {175,215,215},
      {175,215,255},
      {175,255,0},
      {175,255,95},
      {175,255,135},
      {175,255,175},
      {175,255,215},
      {175,255,255},
      {215,0,0},
      {215,0,95},
      {215,0,135},
      {215,0,175},
      {215,0,215},
      {215,0,255},
      {215,95,0},
      {215,95,95},
      {215,95,135},
      {215,95,175},
      {215,95,215},
      {215,95,255},
      {215,135,0},
      {215,135,95},
      {215,135,135},
      {215,135,175},
      {215,135,215},
      {215,135,255},
      {215,175,0},
      {215,175,95},
      {215,175,135},
      {215,175,175},
      {215,175,215},
      {215,175,255},
      {215,215,0},
      {215,215,95},
      {215,215,135},
      {215,215,175},
      {215,215,215},
      {215,215,255},
      {215,255,0},
      {215,255,95},
      {215,255,135},
      {215,255,175},
      {215,255,215},
      {215,255,255},
      {255,0,0},
      {255,0,95},
      {255,0,135},
      {255,0,175},
      {255,0,215},
      {255,0,255},
      {255,95,0},
      {255,95,95},
      {255,95,135},
      {255,95,175},
      {255,95,215},
      {255,95,255},
      {255,135,0},
      {255,135,95},
      {255,135,135},
      {255,135,175},
      {255,135,215},
      {255,135,255},
      {255,175,0},
      {255,175,95},
      {255,175,135},
      {255,175,175},
      {255,175,215},
      {255,175,255},
      {255,215,0},
      {255,215,95},
      {255,215,135},
      {255,215,175},
      {255,215,215},
      {255,215,255},
      {255,255,0},
      {255,255,95},
      {255,255,135},
      {255,255,175},
      {255,255,215},
      {255,255,255},
      {8,8,8},
      {18,18,18},
      {28,28,28},
      {38,38,38},
      {48,48,48},
      {58,58,58},
      {68,68,68},
      {78,78,78},
      {88,88,88},
      {98,98,98},
      {108,108,108},
      {118,118,118},
      {128,128,128},
      {138,138,138},
      {148,148,148},
      {158,158,158},
      {168,168,168},
      {178,178,178},
      {188,188,188},
      {198,198,198},
      {208,208,208},
      {218,218,218},
      {228,228,228},
      {238,238,238},
};

/**
 * @brief Returns the distance between two RGB colors.
 * 
 * @param color
 * @param color
 * @return int 
 */
static int distance_256(rgb_t color_one, rgb_t color_two) {
      int distance_r = color_one.r - color_two.r;
      int distance_g = color_one.g - color_two.g;
      int distance_b = color_one.b - color_two.b;
      return distance_r*distance_r + distance_g*distance_g + distance_b*distance_b;
}

/**
 * @brief Finds the closest ANSI color to the given RGB color.
 * 
 * Returns the index of the closest color found.
 * 
 * @return int 
 */
static int find_closest_color_index(struct gcli* gcli, rgb_t color) {
      int index = 0;
      for (int i = 0; i < MAX_COLORS; i++) {
            if (distance_256(color, gcli->colors[i]) < distance_256(color, gcli->colors[index])) {
                  index = i;
            }
      }
      return index;
}

/**
 * @brief Creates a new RGB color.
 * 
 * @param r 
 * @param g 
 * @param b 
 * @return rgb_t 
 */
static rgb_t gcli_rgb(int r, int g, int b) {
      rgb_t color;
      color.r = r;
      color.g = g;
      color.b = b;
      return color;
}

/* ============================================================== */


/**
 * @brief Initializes the gcli library.
 * 
 * @param gcli struct gcli*
 * @return Errno
 */
Errno gcli_init(struct gcli* gcli) {
      for (int i = 0; i < MAX_COLORS; i++) {
            gcli->colors[i] = gcli_rgb(colors[i][0], colors[i][1], colors[i][2]);
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
Errno gcli_img_to_terminal(struct gcli* gcli, const char* filename, int resized_width, int resized_height) {
      uint32_t *pixels = NULL;
      uint32_t *resized_pixels = NULL;

      pixels = (uint32_t*)stbi_load(filename, &gcli->width, &gcli->height, NULL, 4);
      if (pixels == NULL) {
            gcli->last_error = "Could not open image file.";
            return GCLI_ERR;
      }

      gcli->resized_width = resized_width;
      gcli->resized_height = resized_height;

      resized_pixels = malloc(gcli->resized_width * gcli->resized_height * sizeof(uint32_t));
      if (resized_pixels == NULL) {
            gcli->last_error = "Could not allocate memory for resized pixels.";
            return GCLI_ERR;
      }

      stbir_resize_uint8((uint8_t*)pixels, gcli->width, gcli->height, 0, (uint8_t*)resized_pixels, gcli->resized_width, gcli->resized_height, 0, 4);

      for (int y = 0; y < gcli->resized_height; y++) {
            rgb_t line[gcli->resized_width];

            for (int x = 0; x < gcli->resized_width; x++) {
                  uint32_t pixel = resized_pixels[y*gcli->resized_width + x];
                  
                  rgb_t rgb_color = {
                        .r = (pixel & 0xFF) * ((pixel >> 24) & 0xFF) / 255,
                        .g = (pixel & 0xFF) * ((pixel >> 16) & 0xFF) / 255,
                        .b = (pixel & 0xFF) * ((pixel >> 8) & 0xFF)  / 255
                  };

                  print_color(find_closest_color_index(gcli, rgb_color));
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


/* ============================================================== */

/**
 * @brief Creates a new canvas.
 * 
 * @param name 
 * @param pixels 
 * @param width 
 * @param height 
 * @return gcli_canvas_t 
 */
gcli_canvas_t gcli_canvas(const char* name, uint32_t pixels[], size_t width, size_t height){
      gcli_canvas_t canvas;
      canvas.name   = name;
      canvas.pixels = pixels;
      canvas.width  = width;
      canvas.height = height;
      return canvas;
}

