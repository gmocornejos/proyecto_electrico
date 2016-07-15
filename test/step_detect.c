#include <stdio.h>

#include <motion.h>
#include <bvh_header.h>
#include <space_temporal.h>

int main(int argc, char * argv[]){
    

    FILE * f;

    vector * v;
    time_series * ankle_joint;
    motion m;
    unidimentional_series ankle, ankle_peaks;

    f = fopen(argv[1], "r");

    motion_init( &m );
    load_bvh_data(f, &m);
    ankle_joint = m.data.get( m.data_ptr, "LeftFoot");

    unidimentional_series_init( &ankle, ankle_joint -> length);
    unidimentional_series_init( &ankle_peaks, ankle.length * 0.1);

    for(v = ankle_joint -> begin; v != ankle_joint -> end; ++v)
        ankle.append( &ankle, v -> y);

    detect_peaks( &ankle, &ankle_peaks);

    printf("Angle peaks length %ld\n", ankle_peaks.length);

    motion_dealloc( &m, 1, 1);
    fclose(f);
    ankle.destroy( &ankle );

    return 0; 

}
