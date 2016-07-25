#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <motion.h>

#include <math.h>

void calc_mean_std_dev( unidimentional_series * data, 
                        double * mean, 
                        double * std_dev, 
                        int bessel_correction
                      );

double rms_error( unidimentional_series * data1, 
                  unidimentional_series * data2
                );

double step_ratio( time_series * joint,
                   unidimentional_series * peaks,
                   double sample_time
                 );

void fourier_transform( unidimentional_series * signal );

#endif
