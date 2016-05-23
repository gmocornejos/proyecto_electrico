#include <kinematics.h>

int derivate(time_series * input, double sample_time, time_series * output){

    vector * v, tmp;

    output -> clean(output);
    
    for(v = input -> begin + 1; v != input -> end; ++v){
        tmp.x = ( v -> x - (v-1) -> x ) / sample_time;
        tmp.y = ( v -> y - (v-1) -> y ) / sample_time;
        tmp.z = ( v -> z - (v-1) -> z ) / sample_time;
        output -> append(output, tmp);
    }

    // linear approximation for last point
    v = output -> last(output);
    tmp.x = 2 * (v -> x) - (v-1) -> x;  
    tmp.y = 2 * (v -> y) - (v-1) -> z;  
    tmp.z = 2 * (v -> z) - (v-1) -> z;  
    output -> append(output, tmp);

    return output -> length;        
}

int integrate(time_series * input, double sample_time, time_series * output){

    vector * v, tmp, sum;

    output -> clean(output);
    sum.x = sum.y = sum.z = 0;

    for(v = input -> begin + 1; v != input -> end; ++v){
        tmp.x = 0.5 * sample_time * ( v -> x + (v-1) -> x ) + sum.x;
        tmp.y = 0.5 * sample_time * ( v -> y + (v-1) -> y ) + sum.y;
        tmp.z = 0.5 * sample_time * ( v -> z + (v-1) -> z ) + sum.z;
        output -> append(output, tmp);
        sum = tmp;
    }
    
    // linear approximation for last point
    v = output -> last(output);
    tmp.x = 2 * (v -> x) - (v-1) -> x;
    tmp.y = 2 * (v -> y) - (v-1) -> z;  
    tmp.z = 2 * (v -> z) - (v-1) -> z;  
    output -> append(output, tmp);

    return output -> length;
}
