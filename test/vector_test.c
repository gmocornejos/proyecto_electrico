#include <stdio.h>

#include <mas.h>

VECTOR_LOCAL_DEFINE(double, double_vector);

int comp_double(double v1, double v2){
    return (v1 == v2);
}

int main(int argc, char * argv[]){

    double i, *j;

    double_vector my_vec;
    double_vector_init(&my_vec, 30);

    for(i = 6; i < 100; ++i)
        my_vec.append(&my_vec, i);

    for(j = my_vec.begin; j != my_vec.end; ++j)
        printf(" %lf ", *j);
    printf("\n");

    printf("Vector size: %ld, poping out 10 elements\n", my_vec.length);

    for(i = 0; i < 10; ++i)
        my_vec.pop(&my_vec);
    printf("Vector size: %ld\n", my_vec.length);

    i = 19.0;
    j = my_vec.search(&my_vec, i, comp_double);
    printf("Found %lf, %lf at %p\n", i, *j, j);

    my_vec.destroy(&my_vec);

    printf("Vector pointer %p\n", my_vec.begin);

    return 0;
}
