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
            char tmp_name[len + 3];
            strncpy(tmp_name, "End", 3);
            strncpy(&tmp_name[3], (*parents.last(&parents)) -> name, len);
            Joint * tmp = Joint_alloc(tmp_name, 0, 1, *parents.last(&parents) );
            joints.append(&joints, tmp);
        } else if( strstr(line, "{") )
            parents.append(&parents, *joints.last(&joints) );
        else if( strstr(line, "}") )
            parents.pop(&parents);
        else if( strstr(line, "MOTION") )
            break;
    }
    
//    for(j = joints.begin; j != joints.end; ++j)
//        printf("%s, string length: %ld\n", (*j) -> name, strlen( (*j) -> name) );


}
