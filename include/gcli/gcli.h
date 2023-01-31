#ifndef GCLI_H
#define GCLI_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define unreachable() assert(0 && "unreachable")
#define print_color(index) printf("\e[48;5;%dm  ", index)

#define GCLI_SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#define GCLI_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define GCLI_ABS(T, x)  ((x) * GCLI_SIGN(T, x))

#define DEFAULT_WIDTH  500
#define MAX_COLORS     256

typedef enum {
      GCLI_OK  = 0,
      GCLI_ERR = -1
} Errno;

typedef struct {
      int r;
      int g;
      int b;
} rgb_t;

typedef struct {
      const char *name;
      uint32_t   *pixels;
      size_t      width;
      size_t      height;
      size_t      stride;
} gcli_canvas_t;

struct gcli {
      rgb_t colors[MAX_COLORS];
      gcli_canvas_t canvases[MAX_COLORS];
      int width;
      int height;
      int resized_width;
      int resized_height;
      char* last_error;
};

Errno gcli_init(struct gcli* gcli);
Errno gcli_img_to_terminal(struct gcli* gcli, const char* filename, int resized_width, int resized_height);
gcli_canvas_t gcli_canvas(const char* name, uint32_t pixels[], size_t width, size_t height);
Errno gcli_add_canvas(struct gcli* gcli, gcli_canvas_t canvas);
Errno gcli_save_canvas(gcli_canvas_t canvas, const char* filename);
void  gcli_print_last_error(struct gcli* gcli);


#endif // GCLI_H