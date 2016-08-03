#include <stdio.h>

#include <mas.h>

int main(int argc, char * argv[]){
    

    FILE * signal, * peaks;
    int i, j;
    time_series * ankle_joint;

    motion m;
    unidimentional_series ankle, ankle_peaks;


    bvh_load_data( argv[1], &m );
    unidimentional_series_init( &ankle, 1000);
    unidimentional_series_init( &ankle_peaks, 100);

    ankle_joint = m.data.get( m.data_ptr, "RightFoot");
    get_component( ankle_joint, &ankle, 'y');
    detect_peaks( &ankle, &ankle_peaks, 5);

    signal = fopen("signal.txt", "w");
    peaks = fopen("peaks.txt", "w");

    for(i = 0; i < ankle_joint -> length; ++i)
        fprintf(signal, "%d    %lf\n", i, ankle_joint -> begin[i].y);

    for(i = 0; i < ankle_peaks.length; ++i){
        j = ankle_peaks.begin[i];
        fprintf(peaks, "%d    %lf\n", j, ankle_joint -> begin[j].y);
    }

    fclose(signal);
    fclose(peaks);

    bvh_unload_data( &m );
    ankle.destroy( &ankle );
    ankle_peaks.destroy( &ankle_peaks );

    return 0; 

}
