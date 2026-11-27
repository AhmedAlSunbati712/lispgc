#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Defaults (set in config_defaults) are noted in [brackets].
typedef struct {
    uint64_t cells_per_chunk; // [4096]
    uint64_t initial_num_of_chunks; // [1]
    size_t max_num_of_chunks; // cap on chunks. 0 is unlimited [0]
    double grow_threshold; // after a GC frow if live/total is above this [0.75]
    size_t grow_amount; // How many chunks to add when growing [1]
    double shrink_threshold; // after a GC, empty chunks may be freed if live/total is below this [0.25]
    size_t min_spare_chunks; // empty chunks always kept even when shrinking [1]
    bool stress; // run a gc on every allocation if true [false]
    bool gc_verbose; // print one line per GC to stderr [false]

} Config;

void config_defaults(Config *c);
int config_parse(Config *c, int argc, char **argv);

#endif
