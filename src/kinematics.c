#include <motion.h>
#include <kinematics.h>

#include <math.h>

int derivate(time_series * input, double sample_time, time_series * output){

    vector * v, tmp;

    output -> clean(output);
    
    for(v = input -> begin + 1; v != input -> end; ++v){
        tmp.x = ( v -> x - (v-1) -> x ) / sample_time;
        tmp.y = ( v -> y - (v-1) -> y ) / sample_time;
        tmp.z = ( v -> z - (v-1) -> z ) / sample_time;
        output -> append(output, tmp);
    }

    // linear approximation for last point
    v = output -> last(output);
    tmp.x = 2 * (v -> x) - (v-1) -> x;  
    tmp.y = 2 * (v -> y) - (v-1) -> z;  
    tmp.z = 2 * (v -> z) - (v-1) -> z;  
    output -> append(output, tmp);

    return output -> length;        
}

int integrate(time_series * input, double sample_time, time_series * output){

    vector * v, tmp, sum;

    output -> clean(output);
    sum.x = sum.y = sum.z = 0;

    for(v = input -> begin + 1; v != input -> end; ++v){
        tmp.x = 0.5 * sample_time * ( v -> x + (v-1) -> x ) + sum.x;
        tmp.y = 0.5 * sample_time * ( v -> y + (v-1) -> y ) + sum.y;
        tmp.z = 0.5 * sample_time * ( v -> z + (v-1) -> z ) + sum.z;
        output -> append(output, tmp);
        sum = tmp;
    }
    
    // linear approximation for last point
    v = output -> last(output);
    tmp.x = 2 * (v -> x) - (v-1) -> x;
    tmp.y = 2 * (v -> y) - (v-1) -> z;  
    tmp.z = 2 * (v -> z) - (v-1) -> z;  
    output -> append(output, tmp);

    return output -> length;
}

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

    // Sets Hips time_series as plane point
    (m -> sagital).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> transversal).point = (m -> data).get( m -> data_ptr, "Hips");
    (m -> coronal).point = (m -> data).get( m -> data_ptr, "Hips");
    
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

void std_planes_alloc(motion * m){

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

void transform_egocentric(motion * m){

    vector x_i = {1,0,0}, y_i = {0,1,0}, z_i = {0,0,1};
    vector x_o, y_o, z_o;
    vector * v, new_v;
    int i;
    time_series h;
    motion_data_entry * j;

    time_series_init( &h, 100);

    for(v = m -> data.get(m -> data_ptr, "Hips") -> begin; v != m -> data.get(m -> data_ptr, "Hips") -> end; ++v)
        h.append( &h, *v);

    for(j = m -> data.begin; j != m -> data.end; ++j){
        v = j -> value.begin;
        for(i = 0; i < m -> sagital.normal.length; ++i){
            x_o = m -> coronal.normal.begin[i];
            y_o = m -> transversal.normal.begin[i];
            z_o = m -> sagital.normal.begin[i];

            v[i] = vector_vector(h.begin[i], v[i]);

            new_v.x = v[i].x * vector_dot_product(x_o, x_i) +
                      v[i].y * vector_dot_product(x_o, y_i) +
                      v[i].z * vector_dot_product(x_o, z_i);

            new_v.y = v[i].x * vector_dot_product(y_o, x_i) +
                      v[i].y * vector_dot_product(y_o, y_i) +
                      v[i].z * vector_dot_product(y_o, z_i);

            new_v.z = v[i].x * vector_dot_product(z_o, x_i) +
                      v[i].y * vector_dot_product(z_o, y_i) +
                      v[i].z * vector_dot_product(z_o, z_i);

            v[i] = new_v;
        }
    }

    h.destroy( &h ); // Deallocs memory.

}
