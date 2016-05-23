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
    output -> append(output, tmp);

    return output -> length;        
}
