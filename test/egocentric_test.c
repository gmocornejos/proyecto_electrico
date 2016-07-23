#include <motion.h>
#include <kinematics.h>

#include <stdio.h>

int main(int argc, char * argv[]){
    
    FILE * f;
    motion m;
    int i;
    motion_data_entry * j;

    f = fopen(argv[1], "r");

    motion_init( &m );

    load_bvh_data(f, &m );
    std_planes_calculate( &m, 1);
    transform_egocentric( &m );

    for(i = 0; i != m.data.begin -> value.length; ++i){
        for(j = m.data.begin; j != m.data.end; ++j)
            printf(" %lf, %lf, %lf ", j -> value.begin[i].x, j -> value.begin[i].y, j -> value.begin[i].z );
        printf("\n");
    }

    motion_dealloc( &m, 1, 1);
    fclose( f );

    return 0;

}
