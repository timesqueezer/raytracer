#ifndef RANDOMH
#define RANDOMH

#include <stdlib.h>

drand48_data* d_buffer = (drand48_data*) malloc(sizeof(drand48_data));

void seed_random(long int n) {
    srand48_r(n, d_buffer);
}

inline float get_random() {
    double out;
    drand48_r(d_buffer, &out);
    //return 0.5;

    return static_cast<float>(out);
}

void free_random() {
    free(d_buffer);
}

#endif
