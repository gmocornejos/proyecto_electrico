#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <motion.h>

// derivate and integrate a Time series.
int derivate(time_series *, double, time_series *);
int integrate(time_series *, double, time_series *);

// Standard planes allocate, calculate and deallocate.
void std_planes_dealloc(motion * m);

// Calculates the standard planes 
int std_planes_calculate( motion * m );

// Using the standard planes, transforms all data points to a new coordinate system, define by normal vector of the three planes. 
// This function requiers the programmer to allocate and calculate standard planes prior calling it. 
void transform_egocentric( motion * m);                        

/**** vectors manipulation ****/

double vector_magnitude(vector v);

// Creates a vector from v1 to v2
vector vector_vector(vector v1, vector v2); 

vector vector_cross_product(vector v1, vector v2);
double vector_dot_product(vector v1, vector v2);

// makes v an unitary vector
vector vector_normalize(vector v); 

// projects vector A over a plane defined by normal vector and point. 
vector vector_project_plane(vector A, vector point, vector normal);

// Calculates the angle between three points in 3d space, if plane != NULL, it first projects the vectors over plane and then calculates the angle. 
int vector_calculate_angle( time_series * origin, 
                            time_series * vector1, 
                            time_series * vector2, 
                            plane * p, 
                            unidimentional_series * angle
                          );


#endif
