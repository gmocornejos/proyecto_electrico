#include <string.h>

#include <motion.h>

VECTOR_DEFINE(double, unidimentional_series);

VECTOR_DEFINE(vector, time_series);

DICTIONARY_DEFINE(char *, time_series, motion_data);

DICTIONARY_DEFINE(char *, double, motion_parameters);

int motion_init(motion * m){

    if( motion_data_init( &(m -> data), strcmp ) == NULL)
        return 0;
    if( motion_parameters_init( &(m -> parameters), strcmp ) == NULL )
        return 0;

    m -> data_ptr  = &(m -> data);
    m -> param_ptr = &(m -> parameters);

    return 1;
}

void motion_dealloc(motion * m, int free_data_key, int free_param_key){
    motion_data_entry * d;
    motion_parameters_entry * p;

    for(d = (m -> data).begin; d != (m -> data).end; ++d ){
        (d -> value).destroy( &(d -> value) );
        if( free_data_key )
            free( d -> key );
    }

    if( free_param_key )
        for(p = (m -> parameters).begin; p != (m -> parameters).end; ++p )
            free( p -> key);

    (m -> data).destroy(m -> data_ptr);
    (m -> parameters).destroy(m -> param_ptr);
}

