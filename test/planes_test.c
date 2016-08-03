#include <stdio.h>

#include <mas.h>

void print_normal(time_series n){
    vector * j;
    for(j = n.begin; j != n.end; ++j)
        printf("%f, %f, %f\n", j -> x, j -> y, j -> z);             
}

int main(int argc, char * argv[]){

    FILE * input;
    time_series n;
    motion m;
    
    bvh_load_data( argv[1], &m );
    std_planes_calculate( &m );

    printf("Sagital plane:\n");
    print_normal(m.sagital.normal);
    printf("\n\n");

    printf("Transversal plane:\n");
    print_normal(m.transversal.normal);
    printf("\n\n");

    printf("Coronal plane:\n");
    print_normal(m.coronal.normal);

    std_planes_dealloc( &m );
    bvh_unload_data( &m );

    return 0;
}
