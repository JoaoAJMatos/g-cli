#ifndef GCLI_H
#define GCLI_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define unreachable() assert(0 && "unreachable")
#define print_color(index) printf("\e[48;5;%dm  ", index)

#define DEFAULT_WIDTH  500
#define MAX_COLORS     256

typedef enum {
      GCLI_OK  = 0,
      GCLI_ERR = -1
} Errno;

typedef enum {
      RGB,
      HSL
} color_family_t;

typedef struct {
      int r;
      int g;
      int b;
} rgb_t;

typedef struct {
      float h;
      float s;
      float l;
} hsl_t;


struct gcli {
      hsl_t hsl_colors[MAX_COLORS];
      rgb_t rgb_colors[MAX_COLORS];
      int width;
      int height;
      int resized_width;
      int resized_height;
      char* last_error;
};


Errno gcli_init(struct gcli* gcli);
Errno gcli_img_to_terminal(struct gcli* gcli, const char* filename, color_family_t color_type, int resized_width);
void  gcli_print_last_error(struct gcli* gcli);

#endif // GCLI_H