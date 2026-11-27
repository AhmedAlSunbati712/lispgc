#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t cells_per_chunk = 4096;
    uint64_t initial_num_of_chunks = 1;
    size_t max_num_of_chunks = 0; // cap on chunks. 0 is unlimited
    double grow_threshold = 0.75; // after a GC frow if live/total is above this
    size_t grow_amount = 1; // How many chunks to add when growing
    double shrink_threshold = 0.25; // after a GC, empty chunks may be freed if live/total is below this
    size_t min_spare_chunks = 1; // empty chunks always kept even when shrinking
    bool stress = false; // run a gc on every allocation if true
    bool gc_verbose = false; // print one line per GC to stderr

} Config;

#endif
