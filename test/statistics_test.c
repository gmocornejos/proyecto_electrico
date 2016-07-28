#include <motion.h>
#include <bvh_header.h>
#include <analytics.h>

#include <stdio.h>

int main(int argc, char * argv[]){

    double averg, sd;
    double knee_height_hint = 0.5;
    motion_vector mv1, mv2;
    motion_vector_itr m;
    unidimentional_series tmp, knee_height;

    unidimentional_series_init( &tmp, 100);
    unidimentional_series_init( &knee_height, 100);

    bvh_load_directory( argv[1], &mv1 );
//    load_bvh_directory( argv[2], &mv2 );

    for(m = mv1.begin; m != mv1.end; ++m){
        get_component( (*m) -> data.get( (*m) -> data_ptr, "RightLeg"), &tmp, 'y');
        calc_mean_std_dev( &tmp, &averg, &sd, 0);
        knee_height.append( &knee_height, averg);
    }
   
    unidimentional_series_itr k;
    calc_mean_std_dev( &knee_height, &averg, &sd, 0);
    printf("Knee hight mean and std: %lf, %lf\n", averg, sd);
    for(k = knee_height.begin; k != knee_height.end; ++k)
        printf("%lf\n", *k);

    printf("T-Test at %lf m = %lf\n", knee_height_hint, t_test_one_sample( &knee_height, knee_height_hint) );

    bvh_unload_directory( &mv1 );
    tmp.destroy( &tmp );
    knee_height.destroy( &knee_height );

    return 0;
}
