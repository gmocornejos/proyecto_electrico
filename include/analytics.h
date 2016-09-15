/*
    Code for motion analysis. This functions helps to characterize the motion. 

    Copyright (C) 2016  Luis Guillermo Cornejo Suárez gmocornejos@gmail.com
                        Universidad de Costa Rica
                        Pattern Recognition and Intelligent Systems Lab. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <motion.h>
#include <math.h>

//@ function Calculates mean and standard deviation of a unidimentional time-series. For more information about Bessel correction, visit: https://en.wikipedia.org/wiki/Bessel's_correction . As a thumb rule, if you have ALL the population in your time-series, set bessel_correction = 0, other ways, if you have a sample of the population, set bessel_correction = 1. 

void calc_mean_std_dev( unidimentional_series * data, 
                        double * mean, 
                        double * std_dev, 
                        int bessel_correction
                      );
//@ function Calculates RMS value. If data2 -> NULL, then the RMS value of signal data1 is calculated. If data2 points to a valid time_series, then the RMS error value of both signals is calculated.

double rms_error( unidimentional_series * data1, 
                  unidimentional_series * data2
                );

//@ function The Gait ratio represents how many steps do you achieve with a given cadence (step frequency). It is known that for elderly people, this number is smaller than young people, because taking small steps gives you more stability. 

double gait_ratio( time_series * joint,
                   unidimentional_series * steps,
                   double sample_time
                 );

//@ function Calculates the discrete Fourier transform of unidimentional time-series in signal. Output length is the smalles power of to not less than the size of input signal.

void fourier_transform( unidimentional_series * signal,
                        unidimentional_series * output
                      );

double armonic_ratio( unidimentional_series * signal );


double t_test_one_sample( unidimentional_series * sample,
                          double mean
                        );

double t_test_two_samples( unidimentional_series * sample1,
                           unidimentional_series * sample2
                         );

double t_test_Welch( unidimentional_series * sample1,
                     unidimentional_series * sample2
                   );

double anova_one_way( int n_levels, ... );

#endif
