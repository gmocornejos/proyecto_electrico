#include <string.h>

#include <stdlib.h>

#include <vector.h>
#include <bvh_header.h>

#define MAX_LINE 2000

VECTOR_LOCAL_DEFINE(Joint *, Joint_vector);
VECTOR_LOCAL_DEFINE(char *, string_vector);

int string_split(char * s, const char * token, string_vector * v){

    char * c; 

    v -> clean(v);
    c = strtok(s, token);
    v -> append(v, c);
    
    while( (c = strtok(NULL, token)) != NULL ){
        v -> append(v, c);
    }

    return v -> length;
}

int string_cmp(char * v1, char * v2){
    return ! strcmp(v1, v2);
}

void load_bvh_data(FILE *f, motion * m){

    char * param_name;
    char line[MAX_LINE];
    char ** index;
    Joint_vector joints, parents;
    string_vector line_split;

    Joint_vector_init( & joints, 30);
    Joint_vector_init( & parents, 30);
    string_vector_init( & line_split, 10);

    Joint ** j;

    while( fgets(line, MAX_LINE, f) ){
        if( strstr(line, "ROOT") ){
            string_split(line, " ", &line_split);
            index = line_split.search(&line_split, "ROOT", string_cmp);
            Joint * tmp = Joint_alloc(*++index, 1, 0, NULL);
            joints.append(&joints, tmp);
        } else if( strstr(line, "JOINT") ){
            string_split(line, " ", &line_split);
            index = line_split.search(&line_split, "JOINT", string_cmp);
            Joint * tmp = Joint_alloc(*++index, 0, 0, *parents.last(&parents) );
            joints.append(&joints, tmp);
        } else if( strstr(line, "End") ){
            int len = strlen( (*parents.last(&parents)) -> name);
            strncpy(line, "End", 3);
            strncpy(&line[3], (*parents.last(&parents)) -> name, len);
            line[len - 1] = '\0';
            Joint * tmp = Joint_alloc(line, 0, 1, *parents.last(&parents) );
            joints.append(&joints, tmp);
        } else if( strstr(line, "{") )
            parents.append(&parents, *joints.last(&joints) );
        else if( strstr(line, "}") )
            parents.pop(&parents);
        else if( strstr(line, "OFFSET") ){
            string_split(line, " ", &line_split);
            index = line_split.search(&line_split, "OFFSET", string_cmp);
            ((*joints.last(&joints)) -> local_matrix)[3] = atof(*++index);
            ((*joints.last(&joints)) -> local_matrix)[7] = atof(*++index);
            ((*joints.last(&joints)) -> local_matrix)[11] = atof(*++index);
        } else if( strstr(line, "MOTION") )
            break;
    }

    // stores Frames parameter
    fgets(line, MAX_LINE, f);
    string_split(line, "\t", &line_split);
    index = line_split.search(&line_split, "Frames:", string_cmp);
    param_name = malloc( sizeof(char) * (strlen(*index)) );
    strcpy(param_name, "Frames");
    (m -> parameters).insert(&(m -> parameters), param_name, atof(*++index));

    // Stores Frame Time parameter
    fgets(line, MAX_LINE, f);
    string_split(line, "\t", &line_split);
    index = line_split.search(&line_split, "Frame Time:", string_cmp);
    param_name = malloc( sizeof(char) * (strlen(*index)-1) );
    strcpy(param_name, "FrameTime");
    (m -> parameters).insert(m -> param_ptr, param_name, atof(*++index));

    // Creates dictionary entries for every joints
    for(j = joints.begin; j != joints.end; ++j){
        time_series tmp;
        time_series_init(&tmp, *(m -> parameters).get(m -> param_ptr, "Frames") );
        (m -> data).insert(m -> data_ptr, (*j) -> name, tmp);
    }

    while( fgets(line, MAX_LINE, f) ){
        string_split(line, " ", &line_split);
        index = line_split.begin;

        time_series * ts_tmp;
        for(j = joints.begin; j != joints.end; ++j){
            if( (*j) -> isRoot ){
                ( (*j) -> local_matrix )[3]  = atof(*index);
                ( (*j) -> local_matrix )[7]  = atof(*++index);
                ( (*j) -> local_matrix )[11] = atof(*++index);
            }
            if( ! (*j) -> isEnd ){
                ( (*j) -> rotation)[0] = atof(*++index);
                ( (*j) -> rotation)[1] = atof(*++index);
                ( (*j) -> rotation)[2] = atof(*++index);
            }
            (*j) -> calculate_position( (*j) );

            vector vec_tmp = { ((*j) -> global_matrix)[3], ((*j) -> global_matrix)[7], ((*j) -> global_matrix)[11] };
            ts_tmp = (m -> data).get(m -> data_ptr, (*j) -> name);
            ts_tmp -> append(ts_tmp, vec_tmp);
        }
    }

    // free memory
    for(j = joints.begin; j != joints.end; ++j){
        (*j) -> dealloc( (*j), 0 );
    }

    joints.destroy( & joints );
    parents.destroy( & parents );
    line_split.destroy( & line_split );
}

