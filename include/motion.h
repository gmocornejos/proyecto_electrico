#ifndef MOTION_H
#define MOTION_H

#include <vector.h>
#include <dictionary.h>

typedef struct {
    double x,y,z;
} vector;

VECTOR_DECLARE(vector, time_series);

DICTIONARY_DECLARE(char *, time_series, motion_data);

DICTIONARY_DECLARE(char *, double, motion_parameters);

typedef struct {
    motion_data data, * data_ptr;
    motion_parameters parameters, * param_ptr;
} motion;

int motion_init(motion *);
void motion_dealloc(motion *, int, int);

#endif
