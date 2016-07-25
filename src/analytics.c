#include<analytics.h>

#include <complex.h>
#include <math.h>

void calc_mean_std_dev(unidimentional_series * data, double * mean, double * std_dev, int bessel_correction){

    double m, sd;
    double * i;

    m = sd = 0;

    for(i = data -> begin; i != data -> end; ++i)
        m += *i;

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

double step_ratio(time_series * joint, unidimentional_series * peaks, double sample_time){

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

void fourier_transform(unidimentional_series * signal){

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

    for(i = 0; i < signal -> length; ++i){
        tmp = freq_signal[i];
        signal -> begin[i] = sqrt( creal(tmp) * creal(tmp) + cimag(tmp) * cimag(tmp) );
    }

    free(time_signal);
    free(freq_signal);
}
