#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <motion.h>

int derivate(time_series *, double, time_series *);
int integrate(time_series *, double, time_series *);

#endif
