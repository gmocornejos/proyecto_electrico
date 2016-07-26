#include <motion.h>
#include <bvh_header.h>
#include <space_temporal.h>
#include <analytics.h>

#include <stdio.h>

int main(int argc, char * argv[]){

    motion m;
    unidimentional_series joint;
    int i;
    double arm_ratio, rms_e;
    FILE * f;

    motion_init( &m );
    unidimentional_series_init( &joint, 100 );
    f = fopen(argv[1], "r");
    load_bvh_data(f, &m);

    for(i = 0; i != m.data.get(m.data_ptr, "RightFoot") -> length; ++i)
        joint.append( &joint, m.data.get(m.data_ptr, "RightFoot") -> begin[i].y);

    arm_ratio = armonic_ratio( &joint );
    rms_e = rms_error( &joint, NULL );
    fourier_transform( &joint, &joint );

//    for(i = 0; i != joint.length; ++i)
//        printf("%d    %lf\n", i, joint.begin[i]);
    printf("Razón armónica: %lf, RMS = %lf\n", arm_ratio, rms_e);
    
    fclose(f);
    motion_dealloc( &m, 1, 1);
    joint.destroy( &joint );

}
