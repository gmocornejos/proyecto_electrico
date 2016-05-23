#include <stdio.h>

#include <kinematics.h>

int main(int argc, char * argv[]){

    time_series linear, square;
    vector tmp;
    int i;

    time_series_init( &square, 100);
    time_series_init( &linear, 0);

    for(i = 0; i != 100; ++i){
        tmp.x = i*i;
        tmp.y = tmp.x + 1;
        tmp.z = tmp.y + 1;
        square.append( &square, tmp);
    }

    printf("Linear size %d \n", derivate( &square, 1, &linear) );
        

    return 0;
}
