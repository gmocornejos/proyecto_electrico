#include <mas.h>

#include <stdio.h>

int main(int argc, char * argv[]){

    motion m;
    unidimentional_series angle;
    int i;

    bvh_load_data( argv[1], &m);
    std_planes_calculate( &m );
    unidimentional_series_init( &angle, 1000 );

    vector_calculate_angle( m.data.get(m.data_ptr, "LeftArm"), 
                            m.data.get(m.data_ptr, "LeftForeArm"),
                            m.data.get(m.data_ptr, "Spine"), 
                            &m.sagital, 
                            &angle );

    printf("Angle length %ld\n", angle.length);
    for(i = 0; i != angle.length; ++i)
        printf("%d, %lf\n", i, angle.begin[i]);

    angle.destroy( &angle );
    std_planes_dealloc( &m );
    bvh_unload_data( &m ); 
    
    return 0;
}
