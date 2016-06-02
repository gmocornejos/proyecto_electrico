#include <stdio.h>

#include <motion.h>

int main(int argc, char * argv[]){

    FILE * input;
    motion m;
    unidimentional_series angle;
    int i;

    input = fopen(argv[1], "r");
    motion_init( &m );
    unidimentional_series_init( &angle, m.data.length );
    load_bvh_data(input, &m);
    std_planes_calculate( &m, 1 );

    vector_calculate_angle( m.data.get(m.data_ptr, "LeftArm"), 
                            m.data.get(m.data_ptr, "LeftForeArm"), 
                            m.data.get(m.data_ptr, "Spine"), 
                            &m.sagital, 
                            &angle );

    printf("Angle length %ld\n", angle.length);
    for(i = 0; i != angle.length; ++i)
        printf("%d, %lf\n", i, angle.begin[i]);
    
    return 0;
}
