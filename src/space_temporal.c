#include <math.h>
#include <stdlib.h>

#include <space_temporal.h>

void linear_fit(unidimentional_series * curve, double * m, double * b){
    double m_x = 0, m_y = 0, m_dxdx = 0, m_dxdy = 0;
    int i; 

    for(i = 0; i < curve -> length; i++){
        m_x += (i - m_x) / (i + 1.0);
        m_y += (curve -> begin[i] - m_y) / (i + 1.0);
    }

    for(i = 0; i < curve -> length; i++){
        const double dx = i - m_x;
        const double dy = curve -> begin[i] - m_y;

        m_dxdx += (dx * dx - m_dxdx) / (i + 1.0);
        m_dxdy += (dx * dy - m_dxdy) / (i + 1.0);
    }

    *m = m_dxdy / m_dxdx;
    *b = m_y - m_x * (*m);

}

double * minimum(double * p1, double * p2){

    double * j, * pv, value;

    if(p2 > p1)
        return NULL;

    pv = p1;  // if no minimum, return first element 
//    value = HUGE_VAL;
    value = -HUGE_VAL;
    for(j = p1; j <= p2; ++j)
//        if( *j < value){
        if( *j > value ){
            pv = j;
            value = *j;
        }

    return pv;
}

void detect_peaks(unidimentional_series * curve, unidimentional_series * peaks){
    
    double m, b;
    double * j, * lms;
    int L; 
    int gamma_indx, i, k; 
    double gamma_sum, gamma_min;
    double std_dev, average;

    peaks -> clean( peaks );

    L = ceil(curve -> length / 2.0) - 1;
    lms = malloc( sizeof(double) * L * curve -> length);

    // Primer paso del algoritmo: detrimenta linealmente la señal
    linear_fit( curve, &m, &b );

    for(j = curve -> begin; j != curve -> end; ++j)
        *j = m * (*j) + b;

    // segundo paso: crea el "local maxima scalogram"
    // en realidad aquí bucamos el mínimo
    for(k = 0; k < L; ++k){
        for(i = 0, j = curve -> begin; i < curve -> length; ++i, ++j)
            if( i < k || i > (curve -> length - k) )
                lms[k * (curve -> length) + i] = rand() + 1.0;
            else
                if( j == minimum(j-k, j+k) )
                    lms[k * (curve -> length) + i] = 0.0;
                else
                    lms[k * (curve -> length) + i] = rand() + 1.0;
    }

    // 3ro: encuentra gamma
    gamma_min = HUGE_VAL;
    gamma_indx = 0;
    for(k = 0; k < L; ++k){
        gamma_sum = 0;
        for(i = 0; i < curve -> length; ++i)
            gamma_sum += lms[k * (curve -> length) + i];

        if( gamma_sum < gamma_min )
            gamma_min = gamma_sum;
            gamma_indx = k; 
    }

    printf("gama %d, L %d\n", gamma_indx, L);

    // 4to: calcula la desviación estándar por columnas de lms
    for(i = 0; i < curve -> length; ++i){
        std_dev = 0;
        average = 0;

        for(k = 0; k < gamma_indx; ++k)
            average += lms[k * (curve -> length) + i];
        average /= gamma_indx;

        for(k = 0; k < gamma_indx; ++k)
            std_dev += fabs( lms[k * (curve -> length) + i] - average);
        std_dev /= (gamma_indx - 1);

        if( std_dev == 0)
           peaks -> append(peaks, i);
    }

    free(lms);
}
