#include <stdio.h>
#include <assert.h>

#include <gcli.h>

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
     
    //assert(argc > 0);
    //args_step(&argc, &argv);

    struct gcli gcli;
    if (gcli_init(&gcli) == GCLI_ERR) {
        gcli_print_last_error(&gcli);
        return 1;
    }

    if (gcli_img_to_terminal(&gcli, "../img/test.png", RGB, resized_width) == GCLI_ERR) {
        gcli_print_last_error(&gcli);
        return 1;
    }

    return 0;
}