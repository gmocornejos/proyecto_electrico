#include <string.h>
#include <math.h>

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

void dealloc_std_planes(motion * m){

    (m -> sagital).normal.destroy( & (m -> sagital).normal );
    (m -> transversal).normal.destroy( & (m -> transversal).normal );
    (m -> coronal).normal.destroy( & (m -> coronal).normal );

}

int calculate_std_planes(motion * m){
    // temporal variables
    vector v1, v2, v3, tmp_normal;
    double magnitude;
    int i;

    // Sets Hips time_series as plane point
    (m -> sagital).point = (m -> data).get( &(m -> data), "Hips");
    (m -> transversal).point = (m -> data).get( &(m -> data), "Hips");
    (m -> coronal).point = (m -> data).get( &(m -> data), "Hips");

    // Init normal vectors
    time_series_init( &(m -> sagital).normal, (m -> data).length);
    time_series_init( &(m -> transversal).normal, (m -> data).length);
    time_series_init( &(m -> coronal).normal, (m -> data).length);

    /**** Sagital plane *****/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        // Shoulder average
        v1.x = 0.5 * ( ( (m -> data).get(m, "LeftShoulder") -> begin )[i].x + ( (m -> data).get(m, "RightShoulder") -> begin )[i].x );
        v1.y = 0.5 * ( ( (m -> data).get(m, "LeftShoulder") -> begin )[i].y + ( (m -> data).get(m, "RightShoulder") -> begin )[i].y );

        v1.z = 0.5 * ( ( (m -> data).get(m, "LeftShoulder") -> begin )[i].z + ( (m -> data).get(m, "RightShoulder") -> begin )[i].y );
        // UpLeg average
        v2.x = 0.5 * ( ( (m -> data).get(m, "LeftUpLeg") -> begin )[i].x + ( (m -> data).get(m, "RightUpLeg") -> begin )[i].x );
        v2.y = 0.5 * ( ( (m -> data).get(m, "LeftUpLeg") -> begin )[i].y + ( (m -> data).get(m, "RightUpLeg") -> begin )[i].y );
        v2.z = 0.5 * ( ( (m -> data).get(m, "LeftUpLeg") -> begin )[i].z + ( (m -> data).get(m, "RightUpLeg") -> begin )[i].z );

        // Third point
        v3 = *( ((m -> data).get(m, "ToSpine") -> begin)[i] );

        // Calculate normal vector
        tmp_normal.x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v3.y);
        tmp_normal.y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
        tmp_normal.z = (v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v2.x) * (v2.y - v1.y);

        // normalizes normal vector
        magnitude = sqrt(tmp_normal.x * tmp_normal.x + tmp_normal.y * tmp_normal.y + tmp_normal.z * tmp_normal.z);
        tmp_normal.x /= magnitude;
        tmp_normal.y /= magnitude;
        tmp_normal.z /= magnitude;
    }

    return 0;
}
