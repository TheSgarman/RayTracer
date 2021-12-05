#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

#include "Ray.h"
#include "Vec3.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


inline double random_double() {
    return rand()/(RAND_MAX+1.0);
}

inline double random_double(double min, double max) {
    return min+(max-min)*random_double();
}

//def "infinity"
const double infinity = std::numeric_limits<double>::infinity();
//def pi
const double pi = 3.1415926535897932385;


inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double clamp (double x, double min, double max) {
    if (x<min) return min;
    if (x>max) return max;
    return x;
}


#endif