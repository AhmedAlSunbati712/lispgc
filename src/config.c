#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void config_defaults(Config *c) {
    c->cells_per_chunk = 4096;
    c->initial_num_of_chunks = 1;
    c->max_num_of_chunks = 0; // unlimited
    c->grow_threshold = 0.75;
    c->grow_amount = 1;
    c->shrink_threshold = 0.25;
    c->min_spare_chunks = 1;
#ifdef GC_STRESS
    c->stress = true; // the stress build turns it on by default
#else
    c->stress = false;
#endif
    c->gc_verbose = false;
}

// Returns the index in argv of the .scm file,
// argc if there is no file for repl, or -1 on an error.
// Flags must come before the file.
int config_parse(Config *c, int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        // First argument that isn't a flag is the file to run
        if (strncmp(arg, "--", 2) != 0) {
            return i;
        }

        // Flags without a value
        if (strcmp(arg, "--stress") == 0) {
            c->stress = true;
            continue;
        }
        if (strcmp(arg, "--gc-verbose") == 0) {
            c->gc_verbose = true;
            continue;
        }

        // Every other flag takes a value: the next argument
        if (i + 1 >= argc) {
            fprintf(stderr, "error: %s needs a value\n", arg);
            return -1;
        }
        char *value = argv[++i];

        if (strcmp(arg, "--chunk-cells") == 0) {
            c->cells_per_chunk = strtoull(value, NULL, 10);
        } else if (strcmp(arg, "--initial-chunks") == 0) {
            c->initial_num_of_chunks = strtoull(value, NULL, 10);
        } else if (strcmp(arg, "--max-chunks") == 0) {
            c->max_num_of_chunks = strtoull(value, NULL, 10);
        } else if (strcmp(arg, "--grow-amount") == 0) {
            c->grow_amount = strtoull(value, NULL, 10);
        } else if (strcmp(arg, "--min-spare-chunks") == 0) {
            c->min_spare_chunks = strtoull(value, NULL, 10);
        } else if (strcmp(arg, "--grow-threshold") == 0) {
            c->grow_threshold = strtod(value, NULL);
        } else if (strcmp(arg, "--shrink-threshold") == 0) {
            c->shrink_threshold = strtod(value, NULL);
        } else {
            fprintf(stderr, "error: unknown flag %s\n", arg);
            return -1;
        }
    }
    return argc; // no file, repl
}

// Checks that the values make sense together. Prints the problem and returns false if not.
bool config_validate(const Config *c) {
    if (c->cells_per_chunk == 0) {
        fprintf(stderr, "error: --chunk-cells must be greater than 0\n");
        return false;
    }
    if (c->initial_num_of_chunks == 0) {
        fprintf(stderr, "error: --initial-chunks must be greater than 0\n");
        return false;
    }
    if (c->max_num_of_chunks != 0 && c->max_num_of_chunks < c->initial_num_of_chunks) {
        fprintf(stderr, "error: --max-chunks must be 0 (unlimited) or at least --initial-chunks\n");
        return false;
    }
    if (!(0 <= c->shrink_threshold && c->shrink_threshold < c->grow_threshold && c->grow_threshold <= 1)) {
        fprintf(stderr, "error: thresholds must satisfy 0 <= shrink < grow <= 1\n");
        return false;
    }
    return true;
}
