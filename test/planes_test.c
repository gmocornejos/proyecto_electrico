#include <stdio.h>

#include <motion.h>
#include <bvh_header.h>

int main(int argc, char * argv[]){

    FILE * input;
    
    input = fopen(argv[1], "r");

    motion m;

    motion_init( & m );

    load_bvh_data(input, &m);

    calculate_std_planes( &m );

    return 0;
}
