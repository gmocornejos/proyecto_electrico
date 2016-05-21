#ifndef BVH_HEADER
#define BVH_HEADER

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <motion.h>

/*** Joint data structures ***/ 
typedef struct Joint Joint;
struct Joint{
    char * name;
    int isRoot;
    int isEnd;
    Joint * parent;
    double rotation[3];
    double * local_matrix;
    double * global_matrix;

    void (*calculate_position) (Joint *);
    void (*calculate_local_matrix) (Joint *);
    void (*dealloc) (Joint *, int);
};
/*** Joint functions ***/
Joint * Joint_alloc(char *, int, int, Joint *);

/*** bvh parser functions ***/
void load_bvh_data(FILE *, motion *);

#endif
