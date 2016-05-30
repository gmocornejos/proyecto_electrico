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

int calculate_angle(time_series * o, time_series * v1, time_series * v2, plane * p, unidimentional_series * angle){

    time_series origin, vector1, vector2;

    if( p != NULL){
        time_series_init( & origin, o -> length);
        time_series_init( & vector1, o -> length);
        time_series_init( & vector2, o -> length);
    } else {
        origin = * o;
        vector1 = * v1;
        vector2 = * v2;
    }

}

int calculate_std_planes(motion * m){
    // temporal variables
    vector v1, v2, v3, tmp_normal;
    double magnitude;
    int i;

    // Sets Hips time_series as plane point
    (m -> sagital).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> transversal).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> coronal).point = (m -> data).get( m -> data_ptr, "Hips");

    // Init normal vectors
    time_series_init( &(m -> sagital).normal, (m -> data).begin -> value.length);
    time_series_init( &(m -> transversal).normal, (m -> data).begin -> value.length);
    time_series_init( &(m -> coronal).normal, (m -> data).begin -> value.length);

/*-------------------------------------------------*/
    /**** Sagital plane *****/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        // Shoulder average
        v1.x = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].x + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].x );
        v1.y = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].y + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].y );
         v1.z = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].z + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].z );

        // UpLeg average
        v2.x = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftUpLeg") -> begin )[i].x + 
            ( (m -> data).get(m -> data_ptr, "RightUpLeg") -> begin )[i].x );
        v2.y = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftUpLeg") -> begin )[i].y + 
            ( (m -> data).get(m -> data_ptr, "RightUpLeg") -> begin )[i].y );
        v2.z = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftUpLeg") -> begin )[i].z + 
            ( (m -> data).get(m -> data_ptr, "RightUpLeg") -> begin )[i].z );

        // Third point
        v3 = ((m -> data).get(m -> data_ptr, "ToSpine") -> begin)[i];

        // Calculate normal vector
        tmp_normal.x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v3.y);
        tmp_normal.y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
        tmp_normal.z = (v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v2.x) * (v2.y - v1.y);

        // normalizes normal vector
        magnitude = sqrt(tmp_normal.x * tmp_normal.x + tmp_normal.y * tmp_normal.y + tmp_normal.z * tmp_normal.z);
        tmp_normal.x /= magnitude;
        tmp_normal.y /= magnitude;
        tmp_normal.z /= magnitude;
        (m -> sagital).normal.append( &( (m -> sagital).normal ), tmp_normal);
    }

/*-------------------------------------------------*/
    /**** Coronal plane *****/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        // Shoulder average
        v1.x = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].x + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].x );
        v1.y = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].y + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].y );
         v1.z = 0.5 * ( 
            ( (m -> data).get(m -> data_ptr, "LeftShoulder") -> begin )[i].z + 
            ( (m -> data).get(m -> data_ptr, "RightShoulder") -> begin )[i].z );

        // Second and third point 
        v2 = ((m -> data).get(m -> data_ptr, "LeftUpLeg") -> begin)[i];
        v3 = ((m -> data).get(m -> data_ptr, "RightUpLeg") -> begin)[i];
         // Calculate normal vector
        tmp_normal.x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v3.y);
        tmp_normal.y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
        tmp_normal.z = (v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y);

        // normalizes normal vector
        magnitude = sqrt(tmp_normal.x * tmp_normal.x + tmp_normal.y * tmp_normal.y + tmp_normal.z * tmp_normal.z);
        tmp_normal.x /= magnitude;
        tmp_normal.y /= magnitude;
        tmp_normal.z /= magnitude;
        (m -> coronal).normal.append( &((m -> coronal).normal), tmp_normal);
    }

/*-------------------------------------------------*/
    /**** Coronal plane *****/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        v1 = (m -> sagital).normal.begin[i];
        v2 = (m -> coronal).normal.begin[i];
        
        tmp_normal.x = v1.y * v2.z - v1.z * v2.y;
        tmp_normal.y = v1.z * v2.x - v1.x * v2.z;
        tmp_normal.z = v1.x * v2.y - v1.y * v2.x;

        magnitude = sqrt(tmp_normal.x * tmp_normal.x + tmp_normal.y * tmp_normal.y + tmp_normal.z * tmp_normal.z);
        tmp_normal.x /= magnitude;
        tmp_normal.y /= magnitude;
        tmp_normal.z /= magnitude;
        
        (m -> transversal).normal.append( &((m -> transversal).normal), tmp_normal);
    }

    return 0;
}
