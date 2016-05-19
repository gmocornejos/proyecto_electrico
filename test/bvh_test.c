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

    load_bvh_data(input);

    return 0;
}
