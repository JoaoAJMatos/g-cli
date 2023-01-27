#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define DEFAULT_WIDTH  32

typedef enum {
      RGB,
      HSL
} color_t;

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

int img_to_terminal(char* filename, int resized_width, color_t color_type);