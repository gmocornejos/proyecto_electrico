#include <mas.h>

int main(int argc, char * argv[]){

    int i;
    motion m;
    unidimentional_series steps;
    unidimentional_series step_len, step_t;
    double cad, dist;

    // Init storage
    unidimentional_series_init( &steps    , 100 );
    unidimentional_series_init( &step_len , 100 );
    unidimentional_series_init( &step_t   , 100 );

    // Loads data and calculates steps
    bvh_load_data( argv[1], &m );
    detect_steps( m.data.get(m.data_ptr, "RightFoot"), 'y', &steps, 5);

    cad = cadence( *(m.parameters.get( m.param_ptr, "FrameTime")),
                   & steps );
    dist = distance( m.data.get( m.data_ptr, "Hips"), 
                     (int) steps.begin[0],
                     (int) *(steps.last(&steps))
                   );

    printf("Candencia %lf, distacia %lf\n", cad, dist);

    step_length( m.data.get(m.data_ptr, "RightFoot"),
                 & steps,
                 & step_len );

    step_time( *(m.parameters.get(m.param_ptr, "FrameTime")),
               & steps,
               & step_t );

    printf("Step, Step Length, step Time\n");
    for(i = 0; i < step_len.length; ++i)
        printf("%d, %lf, %lf\n", i, step_len.begin[i], step_t.begin[i]);

    bvh_unload_data( &m );
    steps.destroy( &steps );
    step_len.destroy( &step_len );
    step_t.destroy( &step_t );

}
