#ifndef SPACE_TEMPORAL_H
#define SPACE_TEMPORAL_H
    
#include <motion.h>

    /* Miscellaneous functions */
void linear_fit( unidimentional_series *, 
                 double *, 
                 double *
               );

void detect_peaks( unidimentional_series *, 
                   unidimentional_series *, 
                   int binwidth
                 );

void detect_steps( time_series * joint,
                   char component,
                   unidimentional_series * steps,
                   int binwidth
                 );                 
    
    /* Space - tempral variables */

    // Calculates the number of steps per unit of time
double cadence( double sample_time, 
                unidimentional_series * peaks // steps index
              );

double distance( time_series * point,
                 int init,
                 int end
               );              
 
void step_length( time_series * joint, 
                  unidimentional_series * peaks, // steps index
                  unidimentional_series * step_len
                );

void step_time( double sample_time, 
                unidimentional_series * peaks,
                unidimentional_series * step_time
              );
              
#endif
