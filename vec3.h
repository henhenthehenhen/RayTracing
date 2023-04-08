#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using namespace std;

class vec3 {
    public:
        vec3 () : e {0, 0, 0} {} //default constructor, no parameters, sets x, y, z = 0 
        vec3 (double e0, double e1, double e2) : e{e0, e1, e2} {} //sets x, y, z = e0, e1, e2

        double x() const {  //returns x
            return e[0]; 
        }
        double y() const { //returns y
            return e[1]; 
        }
        double z() const { //returns z
            return e[2]; 
        }

        vec3 operator-() const { //returns negative vector
            return vec3 (-e[0], -e[1], -e[2]); 
        }
        double operator[] (int i) const { //returns x, y, or z (in double)
            return e[i]; 
        } 
        double& operator[](int i) { //ptr 
            return e[i]; 
        } 

        vec3& operator+=(const vec3 &v) { //add two vectors
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) { //multiply two vectors by scalar
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) { //divide two vectors by scalar
            return *this *= 1/t;
        }

        double length() const { // length of vector 
            return sqrt(length_squared());
        }

        double length_squared() const { //length of vector squared
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

    public:
        double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

#endif