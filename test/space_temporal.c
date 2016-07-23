#include <motion.h>
#include <space_temporal.h>

int main(int argc, char * argv[]){

    int i;
    FILE * f;
    motion m;
    unidimentional_series steps;
    unidimentional_series step_len, step_t;
    double cad, dist;

    f = fopen(argv[1], "r");

    // Init storage
    motion_init( &m );
    unidimentional_series_init( &steps    , 100 );
    unidimentional_series_init( &step_len , 100 );
    unidimentional_series_init( &step_t   , 100 );

    // Loads data and calculates steps
    load_bvh_data(f, &m);
    detect_steps( &m, "RightFoot", &steps, 5);

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

    motion_dealloc( &m, 1, 1);
    steps.destroy( &steps );
    step_len.destroy( &step_len );
    step_t.destroy( &step_t );

}
