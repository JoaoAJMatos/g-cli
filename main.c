#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>

#include <gcli/gcli.h>

#define DEFAULT_RESIZED_WIDTH 600
#define DEFAULT_HEIGHT 300

#define GCLI_VERSION "0.9.0"

/// @brief Steps through argument list and returns the next argument.
static char *args_step(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

/// @brief Prints the help message.
static void show_help()
{
      printf("Usage: gcli [options] [file]\n");
      printf("Options:\n");
      printf("  -w, --width <width>    Set the width of the image  | Default 600\n");
      printf("  -h, --height <height>  Set the height of the image | Default 300\n");
      printf("  -v, --version          Show version\n");
      printf("  -h, --help             Show this help\n");
}

/// @brief Parses the arguments.
/// @param height The height of the image.
/// @param width The width of the image.
/// @param argc The number of arguments.
/// @param argv The argument list.
///
/// @return 0 if the argument parsing was unsuccessful or
///         if the command doesn't require further execution (like the help message, etc).
///         1 if the argument parsing was successful and further execution is required (like showing the image).
static int arg_parse(int* height, int* width, char* filename, int argc, char** argv)
{
      while (argc > 0) {
            char *arg = args_step(&argc, &argv);
            if (strcmp(arg, "-w") == 0 || strcmp(arg, "--width") == 0) {
                  if (argc == 0) {
                        printf("Missing argument for width\n");
                        return 0;
                  }
                  *width = atoi(args_step(&argc, &argv));
            } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--height") == 0) {
                  if (argc == 0) {
                        printf("Missing argument for height\n");
                        return 0;
                  }
                  *height = atoi(args_step(&argc, &argv));
            } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
                  printf("gcli version %s\n", GCLI_VERSION);
                  return 0;
            } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
                  show_help();
                  return 0;
            }
            else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--file") == 0) {
                  if (argc == 0) {
                        printf("Missing argument for file\n");
                        return 0;
                  }
                  strcpy(filename, args_step(&argc, &argv));
            } else {
                  printf("Unknown argument: %s\n", arg);
                  return 0;
            }
      }

      if (*height <= 0) {
            printf("Invalid height: %d\n", *height);
            return 0;
      }

      if (*width <= 0) {
            printf("Invalid width: %d\n", *width);
            return 0;
      }

      return 1;
}

int main(int argc, char **argv) {
      struct timeval start, end;
      int width = DEFAULT_RESIZED_WIDTH;
      int height = DEFAULT_HEIGHT;
      char filename[100];

      assert(argc > 0);
      if (argc == 1) {
        show_help();
        return 0;
      }

      args_step(&argc, &argv);

      struct gcli gcli;
      if (gcli_init(&gcli) == GCLI_ERR) {
            gcli_print_last_error(&gcli);
            return 1;
      }

      if (!(arg_parse(&height, &width, filename, argc, argv))) return 1;

      gettimeofday(&start, NULL);

      if (gcli_img_to_terminal(&gcli, filename, width, height) == GCLI_ERR) {
            gcli_print_last_error(&gcli);
            return 1;
      }

      gettimeofday(&end, NULL);
      long milliseconds = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
      printf("Time taken: %ldms", milliseconds);

      return 0;
}
