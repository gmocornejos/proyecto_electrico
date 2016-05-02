#include <string.h>

#include <bvh_header.h>

double * multiply_position_matrix(double * m1, double * m2){
    double * m3 = calloc(16, sizeof(double) );
    int i, j, k;

    for(i = 0; i != 4; ++i)
        for(j = 0; j != 4; ++j)
            for(k = 0; k != 4; ++k)
                m3[4*i + j] += m1[4*i + k] * m2[4*k + j];
    
    free(m1);

    return m3;
}

void Joint_calculate_position(Joint *self){
    if(! self -> isEnd)
        self -> calculate_local_matrix(self);
    if(self -> isRoot)
        self -> global_matrix = self -> local_matrix;
    else
        self -> global_matrix = multiply_position_matrix(self -> parent -> global_matrix, self -> local_matrix);
}

void Joint_calculate_local_matrix(Joint *self){
    double a = cos(M_PI * (self -> rotation)[1]/180);
    double b = sin(M_PI * (self -> rotation)[1]/180);
    double c = cos(M_PI * (self -> rotation)[2]/180);
    double d = sin(M_PI * (self -> rotation)[2]/180);
    double e = cos(M_PI * (self -> rotation)[0]/180);
    double f = sin(M_PI * (self -> rotation)[0]/180);

    (self -> local_matrix)[0] = c*e - b*d*f;
    (self -> local_matrix)[1] = -a*f; 
    (self -> local_matrix)[2] = d*e + b*c*f; 
    (self -> local_matrix)[4] = c*f + b*d*e; 
    (self -> local_matrix)[5] = a*e; 
    (self -> local_matrix)[6] = d*f - b*c*e; 
    (self -> local_matrix)[8] = -a*d; 
    (self -> local_matrix)[9] = b; 
    (self -> local_matrix)[10] = a*c;
}

Joint * Joint_alloc(char * name, int isRoot, int isEnd, Joint * parent){
    Joint * j = malloc( sizeof(Joint) );

    int len = strlen(name) - 2;
    j -> name = malloc( sizeof(char) * len );
    memcpy(j -> name, name, len);

    if(! isRoot){ // Root needs no global matrix
        j -> global_matrix = calloc(16, sizeof(double) );
        (j -> global_matrix)[15] = 1;
    }
    j -> local_matrix = calloc(16, sizeof(double) );
    (j -> local_matrix)[15] = 1;

    j -> isRoot = isRoot;
    j -> isEnd = isEnd;
    j -> parent = parent;

    j -> calculate_position = Joint_calculate_position;
    j -> calculate_local_matrix = Joint_calculate_local_matrix;

    return j;
}

void Joint_free(Joint * j){

    if(! j -> isRoot )
        free(j -> global_matrix);
    free(j -> local_matrix);
    free(j);
}
