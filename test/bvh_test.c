#include <stdio.h>
#include <stdlib.h>

#include <motion.h>

int main(int argc, char * argv[]){

    FILE * input;
    int i;
    motion m;
    motion_data_entry * j;
    motion_parameters_entry * p;

    input = fopen(argv[1], "r");

    if( ! motion_init( &m ) ){
        fprintf(stderr, "Error initiating motion object in %s at line %d\n", __FILE__, __LINE__ );
        exit(EXIT_FAILURE);    
    }

    load_bvh_data(input, &m);

    // prints all joints names and data size
    printf("Joints: ");
    for(j = m.data.begin; j != m.data.end; ++j)
        printf(" %s ", j -> key);
    printf("\n");

    for(p = m.parameters.begin; p != m.parameters.end; ++p)
        printf("%s:, %lf\n", p -> key, p -> value);

    for(i = 0; i != m.data.length; ++i){
        for(j = m.data.begin; j != m.data.end; ++j)
            printf(" %lf %lf %lf ", (j -> value).begin[i].x, (j -> value).begin[i].y, (j -> value).begin[i].z );
        printf("\n");
    }

    motion_dealloc( &m, 1, 1);

    fclose(input);

    return 0;
}
