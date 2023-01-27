#include "../include/colors.h"

/// @brief Calculates the distance between two colors in the RGB/HSL color space.
static int distance256(int table[][3], int index, int a, int b, int c) {
      int distance_a = a - table[index][0];
      int distance_b = b - table[index][1];
      int distance_c = c - table[index][2];
      return distance_a * distance_a + distance_b * distance_b + distance_c * distance_c;
}

/// @brief Finds the closest ANSI color index to the given RGB/HSL color.
static int find_ansi_index(int table[][3], int a, int b, int c) {
      int best_index = 0, distance;
      int best_distance = distance256(table, 0, a, b, c);
      for (int i = 1; i < 256; i++) {
            distance = distance256(table, i, a, b, c);
            if (distance < best_distance) {
                  best_index = i;
                  best_distance = distance;
            }
      }
      return best_index;
}

/* ======================================================================= */

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
int load_colors(char *filename, int colors[][3], int max_colors) {
      int i = 0;
      int r, g, b;
      int scanf_result;

      FILE *fp = fopen(filename, "r");
      if (fp == NULL) return -1;

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
void rgb_to_hsl(rgb_t rgb_color, hsl_t *hsl_color) {
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
      else assert(0 && "unreachable");

      float lf = (cmax + cmin)/2;

      float sf = 0;
      if (delta < epsilon) sf = 0;
      else sf = delta/(1 - fabsf(2*lf - 1));

      *hsl_color->h = fmodf(fmodf(hf, 360.0f) + 360.0f, 360.0f);
      *hsl_color->s = sf*100.0f;
      *hsl_color->l = lf*100.0f;
}