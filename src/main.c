#include <stdio.h>
#include "config.h"
#include "error.h"
int main(int argc, char **argv) {
    Config config;
    config_defaults(&config);

    int file_idx = config_parse(&config, argc, argv);
    if (file_idx == -1) {
        return 1;
    } 

    int rv = config_validate(&config);
    if (!rv) {
        return 1;
    }

    if (file_idx != argc) {
        char *filename = argv[file_idx];
        printf("file: %s\n", filename);
    } else {
        printf("REPL\n");
    }
    // Temporary test of the error jump
    if (setjmp(error_jump) == 0) {
        error_jump_set = true;
        printf("before\n");
        printf("error: test\n");
        longjmp(error_jump, 1);
        printf("after\n"); // should never print
    } else {
        printf("recovered\n");
    }

    printf("lispgc 0.0\n");
    return 0;
}
