#include <mas.h>
#include <stdio.h>

int main(int argc, char * argv[]){

    motion_vector one, two;
    motion_vector_itr m;
    unidimentional_series ang1, ang2;
    unidimentional_series tmp_ang;
    double mean, std;

    bvh_load_directory( argv[1], &one );
    bvh_load_directory( argv[2], &two );

    unidimentional_series_init( &ang1, 20 );
    unidimentional_series_init( &ang2, 20 );
    unidimentional_series_init( &tmp_ang, 100 );

    for(m = one.begin; m != one.end; ++m){
        vector_calculate_angle( 
            (*m)->data.get((*m)->data_ptr, "RightLeg"), 
            (*m)->data.get((*m)->data_ptr, "RightUpLeg"),
            (*m)->data.get((*m)->data_ptr, "RightFoot"),
            NULL,
            &tmp_ang
        );
        calc_mean_std_dev( &tmp_ang, &mean, &std, 0);
        ang1.append( &ang1, mean);
    }
       
     for(m = two.begin; m != two.end; ++m){
        vector_calculate_angle( 
            (*m)->data.get((*m)->data_ptr, "RightLeg"), 
            (*m)->data.get((*m)->data_ptr, "RightUpLeg"),
            (*m)->data.get((*m)->data_ptr, "RightFoot"),
            NULL,
            &tmp_ang
        );
        calc_mean_std_dev( &tmp_ang, &mean, &std, 0);
        ang2.append( &ang2, mean);
    }

    printf("Significancia estadística %lf\n", t_test_Welch(&ang1, &ang2 ));

    bvh_unload_directory( &one );
    bvh_unload_directory( &two );
    ang1.destroy( &ang1 );
    ang2.destroy( &ang2 );
    tmp_ang.destroy( &tmp_ang );
}
