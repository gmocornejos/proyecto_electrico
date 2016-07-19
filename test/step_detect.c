#include <stdio.h>

#include <motion.h>
#include <bvh_header.h>
#include <space_temporal.h>

int main(int argc, char * argv[]){
    

    FILE * f, * signal, * peaks;
    int i, j;

    vector * v;
    time_series * ankle_joint;
    motion m;
    unidimentional_series ankle, ankle_peaks;

    f = fopen(argv[1], "r");

    motion_init( &m );
    load_bvh_data(f, &m);
    ankle_joint = m.data.get( m.data_ptr, "RightFoot");

    unidimentional_series_init( &ankle, 100);
    unidimentional_series_init( &ankle_peaks, 100);

   for(v = ankle_joint -> begin; v != ankle_joint -> end; ++v)
        ankle.append( &ankle, v -> y);

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

    motion_dealloc( &m, 1, 1);
    fclose(f);
    ankle.destroy( &ankle );

    return 0; 

}
