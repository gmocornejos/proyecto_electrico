#include <mas.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char * argv[]){

    motion_vector one, two;
    motion_vector_itr mv;
    unidimentional_series w_speed1, w_speed2;
    unidimentional_series cadence1, cadence2;
    unidimentional_series step_len1, step_len2;
    unidimentional_series w_ratio1, w_ratio2;
    unidimentional_series stv1, stv2;
    unidimentional_series acc_rms1, acc_rms2;
    unidimentional_series harm1, harm2;

    unidimentional_series_init( &w_speed1, 10 );
    unidimentional_series_init( &w_speed2, 10 );
    unidimentional_series_init( &cadence1, 10  );
    unidimentional_series_init( &cadence2, 10 );
    unidimentional_series_init( &step_len1, 10 );
    unidimentional_series_init( &step_len2, 10 );
    unidimentional_series_init( &w_ratio1, 10 );
    unidimentional_series_init( &w_ratio2, 10 );
    unidimentional_series_init( &stv1, 10 );
    unidimentional_series_init( &stv2, 10 );
    unidimentional_series_init( &acc_rms1, 10 );
    unidimentional_series_init( &acc_rms2, 10 );
    unidimentional_series_init( &harm1, 10 );
    unidimentional_series_init( &harm2, 10 );

    motion_vector_init( &one, 10);
    motion_vector_init( &two, 10);
    bvh_load_directory( argv[1], &one );
    bvh_load_directory( argv[2], &two );

    double speed, mean, std;
    time_series_itr i;
    time_series tmp_j;
    unidimentional_series steps, tmp;
    unidimentional_series_init( &steps, 100);
    unidimentional_series_init( &tmp, 100);
    time_series_init( &tmp_j, 100);
    for(mv = one.begin; mv != one.end; ++mv){
        // Detecta los pasos
        detect_steps( (*mv) -> data.get((*mv)->data_ptr, "RightFoot"), 'y', &steps, 5);
        // Calcula la distancia 
        speed = distance( (*mv) -> data.get((*mv)->data_ptr, "Hips"), 0, (*mv) -> data.get((*mv)->data_ptr, "Hips") -> length);
        speed /= (*mv) -> data.get((*mv) -> data_ptr, "Hips") -> length;
        w_speed1.append( &w_speed1, speed);
        // Calcula la cadencia
        cadence1.append( &cadence1, cadence( *((*mv)->parameters.get((*mv)->param_ptr,"FrameTime")), &steps ));
        // Calcula longitud promedio del paso
        step_length( (*mv)->data.get((*mv)->data_ptr, "RightFoot"), &steps, &tmp );
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        step_len1.append( &step_len1, mean);
        // Gait ratio
        w_ratio1.append( &w_ratio1, gait_ratio((*mv)->data.get((*mv)->data_ptr, "RightFoot"), &steps, (*(*mv)->parameters.get((*mv)->param_ptr,"FrameTime"))) );
        // Step timing variability
        step_time( (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &steps, &tmp);
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        stv1.append( &stv1, std);
        // Acceleration rms
        derivate( (*mv)->data.get((*mv)->data_ptr, "Hips"), (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &tmp_j );
        derivate( &tmp_j, (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &tmp_j );
        tmp.clean( &tmp );
        for(i = tmp_j.begin;i != tmp_j.end; ++i )
            tmp.append( &tmp, sqrt(i->x * i->x + i->y * i->y + i->z * i->z) );
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        acc_rms1.append( &acc_rms1, std);
        // Harmonic ratio
        get_component( (*mv)->data.get((*mv)->data_ptr, "Hips"), &tmp, 'y');
        harm1.append( &harm1, armonic_ratio( &tmp ) );
    }

    steps.clean( &steps );
    tmp.clean( &tmp );
    tmp_j.clean( &tmp_j );
    for(mv = two.begin; mv != two.end; ++mv){
        // Detecta los pasos
        detect_steps( (*mv) -> data.get((*mv)->data_ptr, "RightFoot"), 'y', &steps, 5);
        // Calcula la distancia 
        speed = distance( (*mv) -> data.get((*mv)->data_ptr, "Hips"), 0, (*mv) -> data.get((*mv)->data_ptr, "Hips") -> length);
        speed /= (*mv) -> data.get((*mv) -> data_ptr, "Hips") -> length;
        w_speed2.append( &w_speed2, speed);
        // Calcula la cadencia
        cadence2.append( &cadence2, cadence( *((*mv)->parameters.get((*mv)->param_ptr,"FrameTime")), &steps ));
        // Calcula longitud promedio del paso
        step_length( (*mv)->data.get((*mv)->data_ptr, "RightFoot"), &steps, &tmp );
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        step_len2.append( &step_len2, mean);
        // Gait ratio
        w_ratio2.append( &w_ratio2, gait_ratio((*mv)->data.get((*mv)->data_ptr, "RightFoot"), &steps, (*(*mv)->parameters.get((*mv)->param_ptr,"FrameTime"))) );
        // Step timing variability
        step_time( (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &steps, &tmp);
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        stv2.append( &stv2, std);
        // Acceleration rms
        derivate( (*mv)->data.get((*mv)->data_ptr, "Hips"), (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &tmp_j );
        derivate( &tmp_j, (*(*mv)->parameters.get((*mv)->param_ptr, "FrameTime")), &tmp_j );
        tmp.clean( &tmp );
        for(i = tmp_j.begin;i != tmp_j.end; ++i )
            tmp.append( &tmp, sqrt(i->x * i->x + i->y * i->y + i->z * i->z) );
        calc_mean_std_dev( &tmp, &mean, &std, 0);
        acc_rms2.append( &acc_rms2, std);
        // Harmonic ratio
        get_component( (*mv)->data.get((*mv)->data_ptr, "Hips"), &tmp, 'y');
        harm2.append( &harm2, armonic_ratio( &tmp ) );
    }


// t_test_Welch( series *, series *);
    printf("Test for walking speed %lf\n", t_test_Welch(&w_speed1, &w_speed2) );
    printf("Test for cadence %lf\n", t_test_Welch(&cadence1, &cadence2) );
    printf("Test for step length %lf\n", t_test_Welch(&step_len1, &step_len2) );
    printf("Test for Walking ratio %lf\n", t_test_Welch(&w_ratio1, &w_ratio2) );
    printf("Test for step time variance %lf\n", t_test_Welch(&stv1, &stv2) );
    printf("Test for acceleracion rms %lf\n", t_test_Welch(&acc_rms1, &acc_rms2) );
    printf("Test for harminic ratio %lf\n", t_test_Welch(&harm1, &harm2) );

    one.destroy( &one );
    two.destroy( &two );

    return 0;
}
