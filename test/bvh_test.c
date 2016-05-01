#include <stdio.h>

int main(int argc, char * argv[]){

    FILE * input;
    input = fopen(argv[1], "r");

    load_bvh_data(input);

    return 0;
}
