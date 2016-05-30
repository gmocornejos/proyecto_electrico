#include <stdio.h>

#include <motion.h>
#include <bvh_header.h>

void print_normal(time_series n){
    vector * j;
    for(j = n.begin; j != n.end; ++j)
        printf("%f, %f, %f\n", j -> x, j -> y, j -> z);             
}

int main(int argc, char * argv[]){

    FILE * input;
    int c;
    time_series n;
    char msg[] = "Sagital = 1, Transversal = 2, Coronal = 3: ";
    motion m;
    
    input = fopen(argv[1], "r");
    motion_init( & m );
    load_bvh_data(input, &m);
    calculate_std_planes( &m );

    while( printf("%s", msg), (c = getchar()) != EOF ){
        getchar(); // catches line feed
        switch ( c ){
            case '1':
                print_normal(m.sagital.normal);
                break;
            case '2':
                print_normal(m.transversal.normal);
                break;
            case '3':
                print_normal(m.coronal.normal);
                break;
        }
    }   

    printf("\n");

    dealloc_std_planes( &m );
    motion_dealloc( &m, 1, 1);

    return 0;
}
