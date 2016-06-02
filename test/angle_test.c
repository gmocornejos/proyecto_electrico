#include <stdio.h>

#include <motion.h>

int main(int argc, char * argv[]){

    FILE * input;
    motion m;
    unidimentional_series angle;
    int i;

    input = fopen(argv[1], "r");
    motion_init( &m );
    unidimentional_series_init( &angle );
    load_bvh_data(input, &m);
    std_planes_calculate( &m, 1 );

    vector_calculate_angle( m.data.get(m.data_ptr, "LeftLeg"), 
                            m.data.get(m.data_ptr, "LeftUpLeg"), 
                            m.data.get(m.data_ptr, "LeftFoot"), 
                            &m.sagital, 
                            &angle );

    printf("Angle length %ld", angle.length);                    
    for(i = 0; i != angle.length; ++i)
        printf("%d, %lf", i, angle.begin[i]);
    
    return 0;
}
