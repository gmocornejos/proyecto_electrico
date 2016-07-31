#include <math.h>
#include <float.h>

#include <motion.h>
#include <kinematics.h>
#include <space_temporal.h>

double cadence(double sample_time, unidimentional_series * peaks){

    double steps, time;

    steps = peaks -> length;
    time = *(peaks -> last(peaks)) - *(peaks -> begin);
    time *= sample_time;

    return steps/time;
}


void step_length(time_series * joint, unidimentional_series * peaks, unidimentional_series * step_len){

    int i;
    vector v1, v2, v;

    step_len -> clean(step_len);

    v1 = joint -> begin[ (int) peaks -> begin[0] ];
    for(i = 1; i < peaks -> length; ++i){
        v2 = v1;
        v1 = joint -> begin[ (int) (peaks -> begin[i]) ];
        v = vector_vector(v1, v2);
        step_len -> append( step_len, sqrt( vector_dot_product(v,v) ) );
    } 
}


void step_time(double sample_time, unidimentional_series * peaks, unidimentional_series * step_time){

    int i;
    double st;

    step_time -> clean(step_time);

    for(i = 1; i < peaks -> length; ++i){
        st = sample_time * (peaks -> begin[i] - peaks -> begin[i-1]);
        step_time -> append( step_time, st);
    }
}


double distance(time_series * point, int init, int end){

    double dist = 0;
    int i; 
    vector v1, v2, v;

    v1 = point -> begin[init];
    for(i = init + 1; i < end + 1; ++i){
        v2 = v1;
        v1 = point -> begin[i];
        v = vector_vector(v1, v2);
        dist += sqrt( vector_dot_product(v, v) );
    }

    return dist;
}

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

void detect_peaks(unidimentional_series * curve, unidimentional_series * peaks, int binwidth){

    double m, b; // Variables for linerly detriment the signal.
    double * j, * histogram; 
    int L, H, i, k;
    int cont, * peaks_per_win_size;
    // Variables para Kitler
    double p1, p2, s1, s2, J_tmp, J = HUGE_VAL;
    double m1, m2;
    int J_indx = 0;

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
        if( cont == 1 )
            break;
    }

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
   
    free( peaks_per_win_size );
    free( histogram );
}

void detect_steps(time_series * joint, char component, unidimentional_series * steps, int binwidth){

    unidimentional_series joint_comp;

    steps -> clean( steps );
    unidimentional_series_init( &joint_comp, 100);

    get_component( joint, &joint_comp, component);

    detect_peaks( &joint_comp, steps, binwidth);
    
    joint_comp.destroy( &joint_comp );
}
