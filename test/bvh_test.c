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

/*    // prints all joints names and data size
    motion_data_entry * j;
    for(j = m.data.begin; j != m.data.end; ++j)
        printf("%s\n", j -> key);
    printf("data size %ld\n", m.data.length);
*/

/*    // prints Hips time series
    time_series * tmp = m.data.get( &m.data, "Hips");
    vector *v;
    for(v = tmp -> begin; v != tmp -> end; ++v)
        printf("%f %f %f \n", v -> x, v -> y, v -> z);
*/

    motion_dealloc( &m, 1, 1);

    fclose(input);

    return 0;
}
