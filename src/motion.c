#include <string.h>

#include <motion.h>

VECTOR_DEFINE(vector, time_series);

DICTIONARY_DEFINE(char *, time_series, motion_data);

DICTIONARY_DEFINE(char *, double, motion_parameters);

int motion_init(motion * m){

    if( motion_data_init( &(m -> data), strcmp ) == NULL)
        return 0;
    if( motion_parameters_init( &(m -> parameters), strcmp ) == NULL )
        return 0;

    return 1;
}

