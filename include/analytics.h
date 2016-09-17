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

//@ function The Gait ratio represents how many steps do you achieve with a given cadence (step frequency). It is known that for elderly people, this number is smaller than young people, because taking small steps gives you more stability. Step index time-series can be calculated with detect_peaks or detect_steps functions, located at include/space_temporal.h

double gait_ratio( time_series * joint,
                   unidimentional_series * steps,
                   double sample_time
                 );

//@ function Calculates the discrete Fourier transform of unidimentional time-series in signal. Output length is the smalles power of to not less than the size of input signal.

void fourier_transform( unidimentional_series * signal,
                        unidimentional_series * output
                      );
//@ function Menz et al (2008) says that if the fourier transformation of a gait signal is calculated, the even components will contain the "power" used to produce the motion, while the odd components represents the irregularities in the gait pattern. The ratio between even and odd components could then describe the regularity of gait motion. 

double armonic_ratio( unidimentional_series * signal );

//@ function Makes a T-test for time series sample i.e. test how likely is that the real mean value of population unidimentional_series * sample is double mean. Returns the p-value of the test.

double t_test_one_sample( unidimentional_series * sample,
                          double mean
                        );

//@ function Makes a T-test for two population samples, i.e test how likely is that the mean value of both samples is the same. The result is only meaningful if both populations has the same variance, other ways, you should use a Welch test. Returns the p-value of the test.

double t_test_two_samples( unidimentional_series * sample1,
                           unidimentional_series * sample2
                         );

//@ function Makes a Welch test for two populations samples, i.e. test how likely is that the mean value of both samples is the same. This test should be used when it is not for sure that both populations has the same variance. 

double t_test_Welch( unidimentional_series * sample1,
                     unidimentional_series * sample2
                   );

//@ function Makes an Anova test for ONE factor (this is implicit) and int n_levels. Returns the p-value. The functions expects n unidimentional_series * data. This is similar to printf function, in which the number of parameters is unknown. 

double anova_one_way( int n_levels, ... );   

#endif
