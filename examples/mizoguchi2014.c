#include <mas.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char * argv[]){

    motion_vector one, two;
    unidimentional_series ts1, ts2, steps, height;
    vector v;
    motion_vector_itr m;
    unidimentional_series_itr j;
    time_series * i;
    double h = 0;

    unidimentional_series_init( &ts1, 20);
    unidimentional_series_init( &ts2, 20);
    unidimentional_series_init( &steps, 100);
    unidimentional_series_init( &height, 100);

    bvh_load_directory( argv[2], &one );
    bvh_load_directory( argv[3], &two );

    for(m = one.begin; m != one.end; ++m){
        detect_steps( (*m) -> data.get((*m)->data_ptr, "RightFoot"), 'y', &steps, 5);
        i = (*m)-> data.get((*m)->data_ptr, "RightFoot");
        v = vector_vector( i -> begin[(int)steps.begin[3]], i -> begin[(int)steps.begin[4]]);
        get_component( (*m)->data.get((*m)->data_ptr, "RightFoot"), &height, 'y');
        for(j = height.begin; j != height.end; ++j)
            if( *j > h )
                h = *j;
        ts1.append( &ts1, h / sqrt(v.x * v.x + v.y * v.y + v.z * v.z) );
    }

    for(m = two.begin; m != two.end; ++m){
        detect_steps( (*m) -> data.get((*m)->data_ptr, "RightFoot"), 'y', &steps, 5);
        i = (*m)-> data.get((*m)->data_ptr, "RightFoot");
        v = vector_vector( i -> begin[(int)steps.begin[3]], i -> begin[(int)steps.begin[4]]);
        get_component( (*m)->data.get((*m)->data_ptr, "RightFoot"), &height, 'y');
        for(j = height.begin; j != height.end; ++j)
            if( *j > h )
                h = *j;
        ts2.append( &ts2, h / sqrt(v.x * v.x + v.y * v.y + v.z * v.z) );
    }

    for(j = ts1.begin; j != ts1.end; ++j)
        printf(" %lf \n", *j);

    for(j = ts2.begin; j != ts2.end; ++j)
        printf(" %lf \n", *j);

    printf("Welch test %lf \n", t_test_Welch( &ts1, &ts2) );

    return 0;
}
