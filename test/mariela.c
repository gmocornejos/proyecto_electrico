#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <motion.h>

int main(int argc, char * argv[]){

    FILE * input;
    motion m;
    double cadera_aceleracion_promedio, rodilla_angulo_prom, brazos_angulo_prom, cadera_angulo_prom, * d;
    vector * j;
    time_series cadera_velocidad, cadera_aceleracion;
    unidimentional_series rodilla_angulo, brazos_angulo, cadera_angulo;

    input = fopen(argv[1], "r");

    // Inicializa todas las estructuras de datos 
    motion_init( &m );
    load_bvh_data(input, &m);
    std_planes_calculate( &m, 1);

    time_series_init( & cadera_velocidad, 500);
    time_series_init( & cadera_aceleracion, 500);

    unidimentional_series_init( & rodilla_angulo, 500);
    unidimentional_series_init( & brazos_angulo, 500);
    unidimentional_series_init( & cadera_angulo, 500);

    // Calcula la aceleracion promedio
    derivate( m.data.get( m.data_ptr, "Hips"), *(m.parameters.get(m.param_ptr, "FrameTime")) , & cadera_velocidad );
    derivate( & cadera_velocidad, *(m.parameters.get(m.param_ptr, "FrameTime")), & cadera_aceleracion );

    cadera_aceleracion_promedio = 0;
    for(j = cadera_aceleracion.begin; j != cadera_aceleracion.end; ++j)
        cadera_aceleracion_promedio += sqrt( (j -> x * j -> x) + (j -> y * j -> y) + (j -> z * j -> z));
    cadera_aceleracion_promedio /= cadera_aceleracion.length;

    // Calcula angulo promedio de la rodilla
    vector_calculate_angle( m.data.get(m.data_ptr, "LeftLeg"),
                            m.data.get(m.data_ptr, "LeftUpLeg"),
                            m.data.get(m.data_ptr, "LeftFoot"),
                            & m.sagital,
                            & rodilla_angulo );

    rodilla_angulo_prom = 0;                            
    for(d = rodilla_angulo.begin; d != rodilla_angulo.end; ++d)
        rodilla_angulo_prom += *d;
    rodilla_angulo_prom /= rodilla_angulo.length;

    // Calcula el angulo promedio del brazo respecto a la vertical
    vector_calculate_angle( m.data.get(m.data_ptr, "LeftArm"),
                            m.data.get(m.data_ptr, "LeftForeArm"),
                            m.data.get(m.data_ptr, "Spine"),
                            & m.sagital,
                            & brazos_angulo );

    brazos_angulo_prom = 0;
    for(d = brazos_angulo.begin; d != brazos_angulo.end; ++d)
        brazos_angulo_prom += *d;
    brazos_angulo_prom /= brazos_angulo.length;

    // Calcula el angulo de la cadera 
    vector_calculate_angle( m.data.get(m.data_ptr, "LeftUpLeg"),
                            m.data.get(m.data_ptr, "LeftLeg"),
                            m.data.get(m.data_ptr, "Spine"),
                            & m.sagital,
                            & cadera_angulo );

    cadera_angulo_prom = 0;
    for(d = cadera_angulo.begin; d != cadera_angulo.end; ++d)
        cadera_angulo_prom += *d;
    cadera_angulo_prom /= cadera_angulo.length;

    // Imprime los resultados
    printf("Aceleración promedio: %lf\n", cadera_aceleracion_promedio);
    printf("Angulo promedio de la rodilla: %lf\n", rodilla_angulo_prom);
    printf("Angulo promedio del brazo respecto a la vertical: %lf\n", brazos_angulo_prom);
    printf("Angulo promedio de la cadera %lf\n", cadera_angulo_prom);

    std_planes_dealloc( &m );
    motion_dealloc( &m, 1, 1);

    fclose(input);

    return 0;
}
