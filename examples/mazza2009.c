#include <mas.h>
#include <stdio.h>

#define G 9.81

int main(int argc, char * argv[]){

    motion_vector one, two;
    motion_vector_itr m;
    unidimentional_series c_series, steps;
    unidimentional_series ws1, ws2, cadence1, cadence2, froude1, froude2;
    unidimentional_series Cph1, Cph2, Cps1, Cps2, Csh1, Csh2;
    time_series_itr i;
    double walking_speed, leg_length, rmsH, rmsP, rmsS;
    vector v1, v2;

    bvh_load_directory( argv[1], &one );
    bvh_load_directory( argv[2], &two );

    unidimentional_series_init( &steps, 100 );
    unidimentional_series_init( &c_series, 100 );
    unidimentional_series_init( &ws1, 100 );
    unidimentional_series_init( &ws2, 100 );
    unidimentional_series_init( &cadence1, 100 );
    unidimentional_series_init( &cadence2, 100 );
    unidimentional_series_init( &froude1, 100 );
    unidimentional_series_init( &froude2, 100 );
    unidimentional_series_init( &Cph1, 100 );
    unidimentional_series_init( &Cph2, 100 );
    unidimentional_series_init( &Cps1, 100 );
    unidimentional_series_init( &Cps2, 100 );
    unidimentional_series_init( &Csh1, 100 );
    unidimentional_series_init( &Csh2, 100 );
    
    for(m = one.begin; m != one.end; ++m){
       detect_steps( (*m)-> data.get((*m)->data_ptr, "RightFoot"), 'y', &steps, 5);
        walking_speed = distance( (*m)->data.get((*m)->data_ptr, "Hips"), *steps.begin, *steps.last(&steps));
        walking_speed *= ( *steps.last(&steps) - *steps.begin ) * *((*m)->parameters.get((*m)->param_ptr, "FrameTime"));
        ws1.append( &ws1, walking_speed );
        cadence1.append( &cadence1, cadence(*(*m)->parameters.get((*m)->param_ptr, "FrameTime"), &steps ) );

        v1 = vector_vector( (*m)->data.get((*m)->data_ptr, "RightUpLeg")->begin[(int)steps.begin[0]], (*m)->data.get((*m)->data_ptr, "RightLeg")->begin[(int)steps.begin[0]] );
        v2 = vector_vector( (*m)->data.get((*m)->data_ptr, "RightLeg")->begin[(int)steps.begin[0]], (*m)->data.get((*m)->data_ptr, "TightFoot")->begin[(int)steps.begin[0]] );
        leg_length = vector_magnitude(v1) + vector_magnitude(v2);
        froude1.append( &froude1, walking_speed*walking_speed/(G * leg_length) );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Hips")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "Hips")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsP = rms_error( &c_series, NULL );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Spine1")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "spine1")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsS = rms_error( &c_series, NULL );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Head")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "Head")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsH = rms_error( &c_series, NULL );

        Cph1.append( &Cph1, 100.0 * (1.0 - rmsH/rmsP));
        Cps1.append( &Cps1, 100.0 * (1.0 - rmsS/rmsP));
        Csh1.append( &Csh1, 100.0 * (1.0 - rmsH/rmsS));
    }

    for(m = two.begin; m != two.end; ++m){
       detect_steps( (*m)-> data.get((*m)->data_ptr, "RightFoot"), 'y', &steps, 5);
        walking_speed = distance( (*m)->data.get((*m)->data_ptr, "Hips"), *steps.begin, *steps.last(&steps));
        walking_speed *= ( *steps.last(&steps) - *steps.begin ) * *((*m)->parameters.get((*m)->param_ptr, "FrameTime"));
        ws2.append( &ws2, walking_speed );
        cadence2.append( &cadence2, cadence(*(*m)->parameters.get((*m)->param_ptr, "FrameTime"), &steps ) );

        v1 = vector_vector( (*m)->data.get((*m)->data_ptr, "RightUpLeg")->begin[(int)steps.begin[0]], (*m)->data.get((*m)->data_ptr, "RightLeg")->begin[(int)steps.begin[0]] );
        v2 = vector_vector( (*m)->data.get((*m)->data_ptr, "RightLeg")->begin[(int)steps.begin[0]], (*m)->data.get((*m)->data_ptr, "TightFoot")->begin[(int)steps.begin[0]] );
        leg_length = vector_magnitude(v1) + vector_magnitude(v2);
        froude2.append( &froude2, walking_speed*walking_speed/(G * leg_length) );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Hips")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "Hips")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsP = rms_error( &c_series, NULL );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Spine1")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "spine1")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsS = rms_error( &c_series, NULL );

        c_series.clean( &c_series );
        for(i = (*m)->data.get((*m)->data_ptr, "Head")->begin; 
          i != (*m)->data.get((*m)->data_ptr, "Head")->end; 
          ++i){
            c_series.append( &c_series, vector_magnitude(*i));
        }
        rmsH = rms_error( &c_series, NULL );

        Cph2.append( &Cph2, 100.0 * (1.0 - rmsH/rmsP));
        Cps2.append( &Cps2, 100.0 * (1.0 - rmsS/rmsP));
        Csh2.append( &Csh2, 100.0 * (1.0 - rmsH/rmsS));
    }

    printf("Walking speed Welch %lf\n", t_test_Welch(&ws1, &ws2) );
    printf("Cadence Welch %lf\n", t_test_Welch(&cadence1, &cadence2) );
    printf("Froude number Welch %lf\n", t_test_Welch(&froude1, &froude2) );
    printf("Atenuación pelvis-cabeza, Welch %lf\n", t_test_Welch(&Cph1, &Cph2) );
    printf("Atenuación pelvis-7vertebra, Welch %lf\n", t_test_Welch(&Cps1, &Cps2) );
    printf("Atenuación 7vertebra-cabeza, Welch %lf\n", t_test_Welch(&Csh1, &Csh2) );

    return 0;
}
