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

int motion_init(motion *);
int calculate_std_planes(motion * m);
int calculate_angle(time_series *, time_series *, time_series *, plane *, unidimentional_series *);
void motion_dealloc(motion *, int, int);

#endif
