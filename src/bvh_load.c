#include <string.h>

#include <vector.h>
#include <bvh_header.h>

#define MAX_LINE 1000

VECTOR_DEFINE(Joint *, Joint_vector);
VECTOR_DEFINE(char *, string_vector);

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

void load_bvh_data(FILE *f){
    
    char line[MAX_LINE];
    char ** index;
    Joint_vector joints, parents;
    string_vector line_split;

    Joint_vector_init( & joints, 30);
    Joint_vector_init( & parents, 30);
    string_vector_init( & line_split, 10);

    while( fgets(line, MAX_LINE, f) ){
        if( strstr(line, "ROOT") ){
            string_split(line, " ", &line_split);
            index = line_split.search(&line_split, "ROOT", string_cmp);
            Joint * tmp = Joint_alloc(*++index, 1, 0, NULL);
            joints.append(&joints, tmp);
        }
    }
}
