#include <math.h>

#include <motion.h>

vector vector_vector(vector v1, vector v2){

    vector tmp = { v2.x - v1.x, 
                   v2.y - v1.y, 
                   v2.z - v1.z };

    return tmp;
}

vector vector_dot_product(vector v1, vector v2){

    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
    
}

vector vector_cross_product(vector v1, vector v2){

    vector tmp = { v1.y * v2.z - v1.z * v2.y ,
                   v1.z * v2.x - v1.x * v2.z ,
                   v1.x * v2.y - v1.y * v2.x };

    return tmp;
}

vector vector_normalize(vector v){
    double mag = sqrt( v.x * v.x + v.y * v.y + v.z * v.z);

    v.x /= mag;
    v.y /= mag;
    v.z /= mag;

    return v;
}

int vector_calculate_angle(time_series * o, time_series * v1, time_series * v2, plane * p, unidimentional_series * angle){

    time_series origin, vector1, vector2;

/*
    if( p != NULL){
        time_series_init( & origin, o -> length);
        time_series_init( & vector1, o -> length);
        time_series_init( & vector2, o -> length);
    } else {
        origin = * o;
        vector1 = * v1;
        vector2 = * v2;
    }
*/

    return 0;
}

int std_planes_calculate(motion * m, int allocate){

    vector v1, v2, v3, tmp_normal;
    double magnitude;
    int i;

    if( allocate )
        std_planes_alloc( m );

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
        v1 = vector_vector(v3, v1);
        v2 = vector_vector(v3, v2);
        tmp_normal = vector_cross_product(v1, v2);

        (m -> sagital).normal.append( &( (m -> sagital).normal ), vector_normalize(tmp_normal) );
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
         v2 = vector_vector(v1, v2);
         v3 = vector_vector(v1, v3);
         tmp_normal = vector_cross_product(v2, v3);
         
        (m -> coronal).normal.append( &((m -> coronal).normal), vector_normalize(tmp_normal) );
    }

/*-------------------------------------------------*/
    /**** Coronal plane *****/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        v1 = (m -> sagital).normal.begin[i];
        v2 = (m -> coronal).normal.begin[i];

        tmp_normal = vector_cross_product(v1, v2);
        
        (m -> transversal).normal.append( &((m -> transversal).normal), vector_normalize(tmp_normal) );
    }

    return 0;
}
