#include<analytics.h>

#include <complex.h>
#include <math.h>
#include <stdarg.h>

void calc_mean_std_dev(unidimentional_series * data, double * mean, double * std_dev, int bessel_correction){

    double m, sd;
    double * i;

    m = sd = 0;

    for(i = data -> begin; i != data -> end; ++i)
        m += *i;
    m /= data -> length;
    
    for(i = data -> begin; i != data -> end; ++i)
        sd += (*i - m) * (*i - m);

    if( bessel_correction )
        sd /= data -> length - 1;
    else
        sd /= data -> length;

    sd = sqrt(sd);

    *mean = m;
    *std_dev = sd;
    
}

double rms_error(unidimentional_series * data1, unidimentional_series * data2){

    int should_free = 0, i;
    double rms = 0;
    unidimentional_series rms_signal;

    if( data2 == NULL )
        rms_signal = *data1;
    else{
        unidimentional_series_init( &rms_signal, 100);
        should_free = 1;
        for(i = 0; i < data1 -> length; ++i)
            rms_signal.append( &rms_signal, data1 -> begin[i] - data2 -> begin[i]);
    }

    for(i = 0; i < rms_signal.length; ++i)
        rms += (rms_signal.begin[i]) * (rms_signal.begin[i]);

    rms = sqrt( rms / rms_signal.length );

    if( should_free )
        rms_signal.destroy( &rms_signal );

    return rms;
}

double gait_ratio(time_series * joint, unidimentional_series * peaks, double sample_time){

    unidimentional_series sl;
    double step_len_avrg, step_len_dev, cad;

    unidimentional_series_init( &sl, 100 );
    step_length(joint, peaks, &sl);
    calc_mean_std_dev( &sl, &step_len_avrg, &step_len_dev, 0);
    cad = cadence( sample_time, peaks);

    sl.destroy( &sl );

    return step_len_avrg / cad;
}

void fft(double complex * in, double complex * out, int size, int step){

    int i;
    double complex t;

    if(step < size){
        fft(out, in, size, 2 * step);
        fft(out + step, in + step, size, 2 * step);

        for(i = 0; i < size; i += 2 * step){
            t = cexp(-I * M_PI * i / size) * out[i + step];
            in[i/2]        = out[i] + t;
            in[(i+size)/2] = out[i] - t;
        } 
    }
}

void fourier_transform(unidimentional_series * signal, unidimentional_series * output){

    double complex * time_signal, * freq_signal, tmp;
    int i, size;

    size = pow(2, ceil(log2(signal -> length)));

    time_signal = calloc( size, sizeof(double complex) );
    freq_signal = calloc( size, sizeof(double complex) );

    for(i = 0; i < signal -> length; ++i){
        time_signal[i] = signal -> begin[i];
        freq_signal[i] = signal -> begin[i];
    }
    
    fft(time_signal, freq_signal, size, 1);

    size = signal -> length;
    output -> clean( output );
    for(i = 0; i < size; ++i){
        tmp = freq_signal[i];
        output -> append( output, sqrt( creal(tmp) * creal(tmp) + cimag(tmp) * cimag(tmp) ) );
    }

    free(time_signal);
    free(freq_signal);
}

double armonic_ratio(unidimentional_series * signal){

    double even = 0, odd = 0, * j;
    int i;
    unidimentional_series tmp_signal;

    unidimentional_series_init( &tmp_signal, 100);
    for(j = signal -> begin; j != signal -> end; ++j)
        tmp_signal.append( &tmp_signal, *j);

    fourier_transform( &tmp_signal, &tmp_signal);

    for(i = 1; i < tmp_signal.length; i += 2)
        odd += tmp_signal.begin[i];
    for(i = 2; i < tmp_signal.length; i += 2)
        even += tmp_signal.begin[i];

    tmp_signal.destroy( &tmp_signal );

    return even / odd; 

}


double students_t(double value, double df){

    long double a, b;

    a = tgamma( (df + 1) / 2 );
    b = sqrt(df * M_PI) * tgamma( df/2 ) * pow(1 + value*value/df, (df+1)/2 );

    return a/b;
}

double t_test_one_sample(unidimentional_series * sample, double mean){

    double aver, std;
    double t;

    calc_mean_std_dev( sample, &aver, &std, 1);

    t =  (aver - mean) / ( std / sqrt(sample -> length));

    return students_t(t, sample -> length - 1);
}

double t_test_two_samples(unidimentional_series * sample1, unidimentional_series * sample2){

    double mean1, mean2, std1, std2, stdN; 
    double t;

    calc_mean_std_dev( sample1, &mean1, &std1, 1);
    calc_mean_std_dev( sample2, &mean2, &std2, 1);

    stdN = sqrt( ( (sample1 -> length - 1)*std1*std1 + (sample2 -> length - 1)*std2*std2 ) / (sample1 -> length + sample2 -> length - 2) );

    t = (mean1 - mean2) / (stdN * sqrt(1/(sample1 -> length) + 1/(sample2 -> length)));

    return students_t(t, sample1 -> length + sample2 -> length -2);

}

double t_test_Welch(unidimentional_series * sample1, unidimentional_series * sample2){

    double mean1, mean2, std1, std2, stdN; 
    double t, df;

    calc_mean_std_dev( sample1, &mean1, &std1, 1);
    calc_mean_std_dev( sample2, &mean2, &std2, 1);

    stdN = sqrt( std1/(sample1 -> length) + std2/(sample2 -> length) );
    df = pow(std1*std1/(sample1 -> length) + std2*std2/(sample2 -> length), 2) / ( pow(std1*std1/sample1 -> length, 2)/(sample1 -> length - 1) + pow(std2*std2/sample2 -> length, 2)/(sample2 -> length - 1) );
    t = (mean1 - mean2) / stdN;

    return students_t(t, df);
}

double F_distribution(double F, double n, double m){

    long double a, b;

    a = tgamma( (n+m)/2 ) * pow(n, n/2) * pow(m, m/2) * pow(F, n/2 - 1);
    b = tgamma(n/2) * tgamma(m/2) * pow(m + n*F, (n+m)/2);

    return a / b;

}

double anova_one_way(int n_levels, ...){

    va_list ap;
    int i, dfB, dfW;
    double m, std, global_mean;
    double B, MSB, W, MSW, F;
    unidimentional_series mean_vector, * level;
    unidimentional_series_itr j;

    va_start(ap, n_levels);
    unidimentional_series_init( &mean_vector, n_levels );

    for(i = 0; i < n_levels; ++i){
        level = va_arg(ap, unidimentional_series *);
        calc_mean_std_dev(level, &m, &std, 0);
        mean_vector.append( &mean_vector, m);
    }
    calc_mean_std_dev( &mean_vector, &global_mean, &std, 0);

    B = 0;
    va_start(ap, n_levels);
    for(j = mean_vector.begin; j != mean_vector.end; ++j){
        level = va_arg(ap, unidimentional_series *);
        B += pow(*j - global_mean, 2) * (level -> length);
    }
    dfB = n_levels - 1;
    MSB = B/dfB;

    W = 0;
    va_start(ap, n_levels);
    for(i = 0; i < n_levels; ++i){
        level = va_arg(ap, unidimentional_series *);
        for(j = level -> begin; j != level -> end; ++j)
            W += pow(*j - mean_vector.begin[i], 2);
    }
    dfW = n_levels * (level -> length - 1);
    MSW = W/dfW;
    
    F = MSB / MSW;

    va_end( ap );
    mean_vector.destroy( &mean_vector );

    return F_distribution(F, dfB, dfW);
}

