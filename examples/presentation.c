// \begin{verbatim}
#include <mas.h>
#include <stdio.h>

int main(int argc, char * argv[]){
    motion_vector captures;
    motion_vector_itr m;
    unidimentional_series right_ang, left_ang;
    unidimentional_series tmp_ang;
    double mean, std;

    bvh_load_directory( argv[1], &captures );
    unidimentional_series_init( &right_ang, 20 );
    unidimentional_series_init( &left_ang, 20 );
    unidimentional_series_init( &tmp_ang, 100 );

    for(m = captures.begin; m != captures.end; ++m){
        vector_calculate_angle( 
            (*m)->data.get((*m)->data_ptr, "RightLeg"), 
            (*m)->data.get((*m)->data_ptr, "RightUpLeg"),
            (*m)->data.get((*m)->data_ptr, "RightFoot"),
            NULL,
            &tmp_ang
        );
        calc_mean_std_dev( &tmp_ang, &mean, &std, 0);
        right_ang.append( &right_ang, mean );

        vector_calculate_angle(
            (*m)->data.get((*m)->data_ptr, "LeftLeg"), 
            (*m)->data.get((*m)->data_ptr, "LeftUpLeg"),
            (*m)->data.get((*m)->data_ptr, "LeftFoot"),
            NULL,
            &tmp_ang
        );
        calc_mean_std_dev( &tmp_ang, &mean, &std, 0);
        left_ang.append( &left_ang, mean );
    }
    printf("Significancia estadística %lf\n", 
        t_test_Welch(&right_ang, &left_ang ) );

    bvh_unload_directory( &captures );
    right_ang.destroy( &right_ang );
    left_ang.destroy( &left_ang );
    tmp_ang.destroy( &tmp_ang );
}// \end{verbatim}
