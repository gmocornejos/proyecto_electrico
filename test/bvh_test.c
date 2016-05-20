#include <stdio.h>
#include <stdlib.h>

#include <motion.h>

int main(int argc, char * argv[]){

    FILE * input;
    input = fopen(argv[1], "r");

    motion m;
    if( ! motion_init( &m ) ){
        fprintf(stderr, "Error initiating motion object in %s at line %d\n", __FILE__, __LINE__ );
        exit(EXIT_FAILURE);    
    }

    load_bvh_data(input, &m);
    motion_parameters_entry * j;
    for(j = m.parameters.begin; j != m.parameters.end; ++j)
        printf("%s\n", j -> key);

    printf("data size %ld\n", m.data.length);

    return 0;
}
