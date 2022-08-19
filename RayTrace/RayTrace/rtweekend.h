#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H


#include <cmath>
#include <limits>
#include <memory>
#include <random>


//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;


//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
	return degrees / 180.0 * PI;
}

//inline double random_double() {
//    // Returns a random real in [0,1).
//    return rand() / (RAND_MAX + 1.0);
//}



inline static double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline static double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}



#endif
