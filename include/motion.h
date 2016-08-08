/*! \file motion.h
    \brief Data structures to easily store and manipulate motion data

In general, motion data consist of time-series of 3 real values, which forms up a mathematical vector. Data structures defined in this file are designed to make easy stores and manipulate motion data. 
*/

/*! \struct vector
    \brief Stores 3 real values (doubles) to form up a mathematical vector. 
    \var x
    \var y
    \var z
*/

/*! \typedef unidimentional_series
    \brief Stores a time series of real values. Time is implicit in index. Implemented through a vector container. 
*/

/*! \typedef time_series
    \brief Stores a time series of vectors. Time is implicit in index. Implemented through a vector container. 
*/

/*! \typedef motion_data
    \brief Dictionary of time_series. A constant char array is used as key value for each entry. Commonly this dictionary stores a time_series for joint, and joint name is used as key value. 
*/

/*! \typedef motion_parameters
    \brief Dictionary of real values to store motion parameters, like sample frequency and number of samples.   
*/

/*! \struct plane
    \brief a plane can be defined with a vector normal to its surface and a point in space, this structure defines those values.
    \var normal time_series of a vector normal to the plane
    \var point pointer to a time_series of points (vectors), to define a plane in space.  
*/

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

VECTOR_DECLARE(motion *, motion_vector);

// motion functions 
int motion_alloc( motion * m);

void motion_free( motion * m );

void get_component( time_series * joint,
                    unidimentional_series * component,
                    char select
                  );                    

#endif
