#include <vector.h>
#include <bvh_header.h>

#define MAX_LINE 1000
VECTOR_DEFINE(Joint *, Joint_vector);

void load_bvh_data(FILE *f){
    
    char line[MAX_LINE];
    Joint_vector joints, parents;

    Joint_vector_init( & joints, 30);
    Joint_vector_init( & parents, 30);

    while( fgets(line, MAX_LINE, f) ){
        printf("%s", line);
    }
}
