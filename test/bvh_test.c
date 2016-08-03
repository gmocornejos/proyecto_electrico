#include <mas.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

    int i;
    motion m;
    motion_data_entry * j;
    motion_parameters_entry * p;

    bvh_load_data( argv[1], &m);

    // prints all joints names and data size
    printf("Joints: ");
    for(j = m.data.begin; j != m.data.end; ++j)
        printf(" %s ", j -> key);
    printf("\n");

    for(p = m.parameters.begin; p != m.parameters.end; ++p)
        printf("%s: %lf\n", p -> key, p -> value);

    for(i = 0; i != m.data.begin -> value.length; ++i){
        for(j = m.data.begin; j != m.data.end; ++j)
            printf(" %lf %lf %lf ", (j -> value).begin[i].x, (j -> value).begin[i].y, (j -> value).begin[i].z );
        printf("\n");
    }

    bvh_unload_data( &m );

    return 0;
}
