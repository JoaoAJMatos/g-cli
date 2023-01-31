#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include <gcli/gcli.h>

#define WIDTH 600
#define HEIGHT 300

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
    struct timeval start, end;


    //assert(argc > 0);
    //args_step(&argc, &argv);

    struct gcli gcli;
    if (gcli_init(&gcli) == GCLI_ERR) {
        gcli_print_last_error(&gcli);
        return 1;
    }

    gettimeofday(&start, NULL);

    if (gcli_img_to_terminal(&gcli, "../img/test.jpeg", WIDTH, HEIGHT) == GCLI_ERR) {
        gcli_print_last_error(&gcli);
        return 1;
    }

    gettimeofday(&end, NULL);
    long milliseconds = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    printf("Time taken: %ldms", milliseconds);

    return 0;
}