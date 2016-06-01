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

void std_planes_alloc(motion * m){

    // Sets Hips time_series as plane point
    (m -> sagital).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> transversal).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> coronal).point = (m -> data).get( m -> data_ptr, "Hips");

    // Init normal vectors
    time_series_init( &(m -> sagital).normal, (m -> data).begin -> value.length);
    time_series_init( &(m -> transversal).normal, (m -> data).begin -> value.length);
    time_series_init( &(m -> coronal).normal, (m -> data).begin -> value.length);
   
}

void std_planes_dealloc(motion * m){

    (m -> sagital).normal.destroy( & (m -> sagital).normal );
    (m -> transversal).normal.destroy( & (m -> transversal).normal );
    (m -> coronal).normal.destroy( & (m -> coronal).normal );

}
