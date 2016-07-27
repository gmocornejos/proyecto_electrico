#include <motion.h>
#include <bvh_header.h>
#include <analytics.h>

#include <stdio.h>

int main(int argc, char * argv[]){

    double averg, sd;
    motion_vector mv1, mv2;
    motion_vector_itr m;
    unidimentional_series tmp, knee_height;

    motion_vector_init( &mv1, 10 );
    motion_vector_init( &mv2, 10 );
    unidimentional_series_init( &tmp, 100);
    unidimentional_series_init( &knee_height, 100);

    load_bvh_directory( argv[1], &mv1 );
//    load_bvh_directory( argv[2], &mv2 );

    for(m = mv1.begin; m != mv1.end; ++m){
        get_component( m -> data.get( m -> data_ptr, "RightLeg"), &tmp, 'y');
        printf("tmp size %ld\n", tmp.length);
//        calc_mean_std_dev( &tmp, &averg, &s, 0);
        printf("mean = %lf, std_dev = %lf\n", averg, sd);
//        knee_height.append( &knee_height, averg);
    }
    
    printf("T-Test a 0.05 m %lf\n", t_test_one_sample( &knee_height, 0.05) );

    return 0;
}
