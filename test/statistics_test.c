#include <mas.h>

#include <stdio.h>

int main(int argc, char * argv[]){

    double averg, sd;
    double knee_height_hint = 0.5;
    motion_vector mv1, mv2;
    motion_vector_itr m;
    unidimentional_series tmp, knee_h1, knee_h2;

    unidimentional_series_init( &tmp, 100);
    unidimentional_series_init( &knee_h1, 100);
    unidimentional_series_init( &knee_h2, 100);

    bvh_load_directory( argv[1], &mv1 );
    bvh_load_directory( argv[2], &mv2 );

    for(m = mv1.begin; m != mv1.end; ++m){
        get_component( (*m) -> data.get( (*m) -> data_ptr, "RightLeg"), &tmp, 'y');
        calc_mean_std_dev( &tmp, &averg, &sd, 0);
        knee_h1.append( &knee_h1, averg);
    }

    for(m = mv2.begin; m != mv2.end; ++m){
        get_component( (*m) -> data.get( (*m) -> data_ptr, "RightLeg"), &tmp, 'y');
        calc_mean_std_dev( &tmp, &averg, &sd, 0);
        knee_h2.append( &knee_h2, averg);
    }


    printf("T-Test at %lf m = %lf\n", knee_height_hint, t_test_one_sample( &knee_h1, knee_height_hint) );

    printf("T-test two samples %lf\n", t_test_two_samples( &knee_h1, &knee_h2) );

    printf("T-test Welch %lf\n", t_test_Welch( &knee_h1, &knee_h2) );

    printf("Anova test %lf\n", anova_one_way(2, &knee_h1, &knee_h2) );

    bvh_unload_directory( &mv1 );
    bvh_unload_directory( &mv2 );
    tmp.destroy( &tmp );
    knee_h1.destroy( &knee_h1 );
    knee_h2.destroy( &knee_h2 );

    return 0;
}
