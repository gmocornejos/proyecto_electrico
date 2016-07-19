#include <math.h>
#include <float.h>

#include <space_temporal.h>

#define binwidth 5

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

    double * j, * pv;

    if(p2 < p1)
        return NULL;

    pv = p1;  // if no minimum, return first element 
    for(j = p1; j <= p2; ++j)
        if( *j < *pv )
            pv = j;

    return pv;
}

void detect_peaks(unidimentional_series * curve, unidimentional_series * peaks){

    double m, b; // Variables for linerly detriment the signal.
    double * j, * histogram; 
    int L, H, i, k;
    int cont, * peaks_per_win_size;

    peaks -> clean( peaks );

    L = ceil(curve -> length / 2.0 ) - 1;
    H = ceil( (double) L / binwidth);

    peaks_per_win_size = calloc( L, sizeof(int) );
    histogram = calloc( H, sizeof(double) );

    // detrimentar linealmente la señal
    linear_fit( curve, &m, &b );
    for(i = 0, j = curve -> begin; j != curve -> end; ++j, ++i)
        *j = *j - m * i - b;

    for(k = 2; k < L; ++k){
        cont = 0;
        for(i = 0, j = curve -> begin; j != curve -> end; ++i, ++j){
            if( i < k || i > (curve -> length - k))
                continue;
            if( j == minimum(j-k, j+k) )
                ++cont;
        }
        peaks_per_win_size[k] = cont;
        ++histogram[ (int) ceil( (double) cont/binwidth) ];
    }

    double p1, p2, s1, s2, J_tmp, J = HUGE_VAL;
    double m1, m2;
    int J_indx = 0;

    // normaliza el histograma
    histogram[0] = 0;
    for(p1 = 0, i = 0; i < H; ++i)
        p1 += histogram[i];
    for(i = 0; i < H; ++i)
        histogram[i] /= p1;

    // Encuentra el umbral óptimo, utilizando Kittler
    for(i = 0; i < H; ++i){
        p1 = p2 = s1 = s2 = FLT_EPSILON;
        m1 = m2 = FLT_EPSILON;
        for(k = 0; k < i; ++k){
            p1 += histogram[k];
            m1 += k * histogram[k];
        }
        for(; k < H; ++k){
            p2 += histogram[k];
            m2 += k * histogram[k];
        }
        for(k = 0; k < i; ++k)
            s1 += histogram[k] * (k - m1) * (k - m1);
        for(; k < H; ++k)
            s2 += histogram[k] * (k - m2) * (k - m2);

        s1 = sqrt(s1/p1);
        s2 = sqrt(s2/p2);

        J_tmp = 1 + 2 * (p1 * log(s1) + p2 * log(s2) ) - 2 * (p1 *log(p1) + p2 * log(p2) );

        if(J_tmp < J){
            J = J_tmp;
            J_indx = i;
        }
    }


    for(p1 = p2 = 0; J_indx < H; J_indx++){
        p1 += J_indx * histogram[J_indx];
        p2 += histogram[J_indx];
    }

    J_indx = floor(p1/p2) * binwidth ;

    for(k = 0; k < L; ++k)
        if( peaks_per_win_size[k] == J_indx )
            break;

    for(i = 0, j = curve -> begin; j != curve -> end; ++i, ++j)
        if( j == minimum(j-k, j+k) )
            peaks -> append(peaks, i);

}

