#include <mas.h>
#include <stdio.h>

int main(int argc, char * argv[]){

    motion m;
    unidimentional_series joint;
    int i;
    double arm_ratio, rms_e;

    bvh_load_data( argv[1], &m );
    unidimentional_series_init( &joint, 1000 );

    get_component( m.data.get(m.data_ptr, "RightFoot"), &joint, 'y');

    arm_ratio = armonic_ratio( &joint );
    rms_e = rms_error( &joint, NULL );
    fourier_transform( &joint, &joint );

    for(i = 0; i != joint.length; ++i)
        printf("%d    %lf\n", i, joint.begin[i]);
//    printf("Razón armónica: %lf, RMS = %lf\n", arm_ratio, rms_e);
    
    joint.destroy( &joint );
    bvh_unload_data( &m );

}
