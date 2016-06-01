#ifndef MOTION_H
#define MOTION_H

#include <vector.h>
#include <dictionary.h>

typedef struct {
    double x,y,z;
} vector;

VECTOR_DECLARE(double, unidimentional_series);

VECTOR_DECLARE(vector, time_series);

DICTIONARY_DECLARE(char *, time_series, motion_data);

DICTIONARY_DECLARE(char *, double, motion_parameters);

typedef struct{
    time_series normal, * point;
} plane;

typedef struct {
    motion_data data, * data_ptr;
    motion_parameters parameters, * param_ptr;
    plane sagital, transversal, coronal;
} motion;

// motion data structures functions 
int motion_init(motion *);
void motion_dealloc(motion *, int, int);
void std_planes_alloc(motion *);
void std_planes_dealloc(motion *);

// motion calculations
int std_planes_calculate(motion * m, int allocate);
int vector_calculate_angle(time_series *, time_series *, time_series *, plane *, unidimentional_series *);

#endif
