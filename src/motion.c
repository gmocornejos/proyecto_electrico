#include <string.h>

#include <motion.h>

VECTOR_DEFINE(double, unidimentional_series);

VECTOR_DEFINE(vector, time_series);

DICTIONARY_DEFINE(char *, time_series, motion_data);

DICTIONARY_DEFINE(char *, double, motion_parameters);

VECTOR_DEFINE(motion *, motion_vector);

int motion_alloc(motion * m){

    if( motion_data_init( &(m -> data), strcmp ) == NULL)
        return 0;
    if( motion_parameters_init( &(m -> parameters), strcmp ) == NULL )
        return 0;

    m -> data_ptr  = &(m -> data);
    m -> param_ptr = &(m -> parameters);

    return 1;
}

void motion_free(motion * m){
    (m -> data).destroy(m -> data_ptr);
    (m -> parameters).destroy(m -> param_ptr);
}

void get_component( time_series * joint, unidimentional_series * component, char select){

    time_series_itr v;

    component -> clean( component );

    switch(select){
        case 'x':
            for(v = joint -> begin; v != joint -> end; ++v)
                component -> append( component, v -> x);
            break;
        case 'y':
            for(v = joint -> begin; v != joint -> end; ++v)
                component -> append( component, v -> y);
            break;
        case 'z':
            for(v = joint -> begin; v != joint -> end; ++v)
                component -> append( component, v -> z);
            break;   
    }
}
