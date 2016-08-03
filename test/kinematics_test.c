#include <stdio.h>
#include <mas.h>


int main(int argc, char * argv[]){

    time_series parabola, linear, square;
    vector tmp;
    double i;
    int j;

    time_series_init( &parabola, 200);
    time_series_init( &square, 0);
    time_series_init( &linear, 0);

    for(i = -24.75; i != 25; i+= 0.25){
        tmp.x = i*i;
        tmp.y = tmp.x + 1;
        tmp.z = tmp.y + 1;
        parabola.append( &parabola, tmp);
    }

    // linear = d/dx square
    derivate( &parabola, 0.25, &linear );

    // square = integral linear
    integrate( &linear, 0.25, &square );

    for(j = 0; j != linear.length; ++j)
        printf("%d   %lf\n", j, square.begin[j].z);

    return 0;
}
