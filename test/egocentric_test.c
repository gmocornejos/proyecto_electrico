#include <mas.h>

#include <stdio.h>

int main(int argc, char * argv[]){
    
    motion m;
    int i;
    motion_data_entry * j;

    bvh_load_data( argv[1], &m );
    std_planes_calculate( &m );

    transform_egocentric( &m );

    for(i = 0; i != m.data.begin -> value.length; ++i){
        for(j = m.data.begin; j != m.data.end; ++j)
            printf(" %lf, %lf, %lf ", j -> value.begin[i].x, j -> value.begin[i].y, j -> value.begin[i].z );
        printf("\n");
    }

    std_planes_dealloc( &m );
    bvh_unload_data( &m );

    return 0;

}
