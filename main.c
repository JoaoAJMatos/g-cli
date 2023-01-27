#include <iostream>
#include "include/gcli.h"
#include <assert.h>

/// @brief Steps through argument list and returns the next argument.
static char *args_step(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

int main(int argc, char **argv) {
      int resized_width = DEFAULT_WIDTH;
      
      assert(argc > 0);
      args_step(&argc, &argv);



      return 0;
}