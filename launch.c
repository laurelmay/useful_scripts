/*
 * Basic CLI launcher for GUI applications. This will launch the given
 * application but redirect stdout and stderr to /dev/null unless they've
 * already been redirected to a file. The launcher exits immediately and does
 * not wait for the child to launch. Currently, this works only when given
 * a path to a binary and does not process .desktop files.
 *
 * Preferable over appending & on the CLI in some of the following
 * situations:
 *  - when the application will write to stderr or stdout, causing unwanted
 *    output in the terminal
 *  - when you want to close the terminal window the application was launched
 *    from
 *
 * I recommend using this with https://github.com/kylelaker/binfmt if you don't
 * want to have to compile it.
 *
 * You also might just be able to use nohup.
 *
 * Licensed under the terms of the MIT License.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int dev_null;

/*
 * Close the given fd if it is a TTY.
 */
int close_and_copy(int fd) {
    int copy = dup(fd);
    if (isatty(fd)) {
        dup2(dev_null, fd);
    }
    return copy;
}

/*
 * Restore the given fd and close the replaced one
 */
void restore_fd(int restore, int replace) {
    dup2(restore, replace);
    close(replace);
}

/*
 * Print program usage information
 */
void usage(char **argv) {
    printf("Usage: %s PROGRAM [ARGS ...]\n", argv[0]);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Incorrect usage\n");
        usage(argv);
        return EXIT_FAILURE;
    }

    dev_null = open("/dev/null", O_WRONLY);

    /*
     * The first argument to this program is where the arguments for the
     * launched application begin.
     */
    char **child_call = (argv + 1);

    close_and_copy(STDOUT_FILENO);

    pid_t child = fork();

    if (child == -1) {
        // Fork failed
        fprintf(stderr, "%s: unable to fork\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (child != 0) {
        // Parent always returns success.
        return EXIT_SUCCESS;
    }

    // Close stderr and attempt to call the request program and restore stderr
    // if there was a problem.
    int stderr_copy = close_and_copy(STDERR_FILENO);
    int call_status = execvp(child_call[0], child_call);
    int exec_err = errno;
    restore_fd(stderr_copy, STDERR_FILENO);

    if (call_status != 0) {
        char *msg = NULL;
        switch (exec_err) {
            case ENOENT:
                msg = "command not found";
                break;
            case EPERM:
                msg = "permission denied";
                break;
            default:
                msg = "unable to launch";
                break;
        }
        fprintf(stderr, "%s: %s: %s\n", argv[0], msg, child_call[0]);
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
