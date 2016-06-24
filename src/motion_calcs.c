#include <math.h>

#include <motion.h>

vector vector_vector(vector v1, vector v2){

    vector tmp = { v2.x - v1.x, 
                   v2.y - v1.y, 
                   v2.z - v1.z };

    return tmp;
}

double vector_dot_product(vector v1, vector v2){

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

vector vector_project_plane(vector A, vector point, vector normal){

    vector Ap = vector_vector(point, A);
    double dimention = vector_dot_product(Ap, normal);
    vector proy = { normal.x * dimention, 
                    normal.y * dimention,
                    normal.z * dimention };

    return vector_vector(proy, A);
}

int vector_calculate_angle(time_series * origin, time_series * vector1, time_series * vector2, plane * p, unidimentional_series * angle){

    int i;
    double dot, ang;
    vector o, v1, v2;

    angle -> clean(angle);

    for(i = 0; i < origin -> length; ++i){

        o  = origin -> begin[i];
        v1 = vector1 -> begin[i];
        v2 = vector2 -> begin[i];

        if( p != NULL ){
            o = vector_project_plane(o, p -> point -> begin[i], p -> normal.begin[i]);
            v1 = vector_project_plane(v1, p -> point -> begin[i], p -> normal.begin[i]);
            v2 = vector_project_plane(v2, p -> point -> begin[i], p -> normal.begin[i]);
        }

        v1 = vector_vector(o, v1);
        v2 = vector_vector(o, v2);

        dot = vector_dot_product(v1, v2)/( sqrt(vector_dot_product(v1, v1)) * sqrt(vector_dot_product(v2, v2)) );

        ang = acos(dot) * 180.0 / M_PI;

        ang = dot > 0 ? ang : - ang;

        angle -> append(angle, ang);
    }

    return 0;
}

int std_planes_calculate(motion * m, int allocate){

    vector v1, v2, v3, tmp_normal;
    double magnitude;
    int i;

    if( allocate )
        std_planes_alloc( m );

    
    /*************** Coronal Plane *********************/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        v1 = vector_vector( 
          ((m -> data).get(m -> data_ptr, "Hips") -> begin)[i], 
          ((m -> data).get(m -> data_ptr, "LeftShoulder") -> begin)[i]  );
        v2 = vector_vector(
          ((m -> data).get(m -> data_ptr, "Hips") -> begin)[i],
          ((m -> data).get(m -> data_ptr, "RightShoulder") -> begin)[i] );
        tmp_normal = vector_cross_product(v1, v2);

        (m -> coronal).normal.append( &((m -> coronal).normal), vector_normalize(tmp_normal) );
    }

    /*************** Transversal Plane *********************/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){

        tmp_normal = vector_vector(
          ((m -> data).get(m -> data_ptr, "Spine") -> begin)[i],
          ((m -> data).get(m -> data_ptr, "Neck") -> begin)[i]
        );
        
        (m -> transversal).normal.append( &((m -> transversal).normal), vector_normalize(tmp_normal) );
    }

    /*************** Sagital Plane *********************/
    for(i = 0; i != (m -> data).begin -> value.length; ++i){
        
        tmp_normal = vector_cross_product(
          (m -> coronal).normal.begin[i],
          (m -> transversal).normal.begin[i]
        );

        (m -> sagital).normal.append( &((m -> sagital).normal), vector_normalize(tmp_normal) );
    }

    return 0;
}

