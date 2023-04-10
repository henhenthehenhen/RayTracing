// vec3 Utility Functions
#include "vec3.h"

inline ostream& operator << (ostream &out, const vec3 &v) { //outputting the vector? 
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+ (const vec3 &u, const vec3 &v) { //adding two vectors
    return vec3 (u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator- (const vec3 &u, const vec3 &v) { //subtracting two vectors 
    return vec3 (u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator* (const vec3 &u, const vec3 &v) { //multuplying two vectors (element-wise)
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator* (double t, const vec3 &v) { //multiplying scalar by vector 
    return vec3 (t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator* (const vec3 &v, double t) { //multiplying vector by scalar
    return t * v;
}

inline vec3 operator/(vec3 v, double t) { //dividing vector by scalar
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) { //dot product 
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) { //cross product 
    return vec3 (u.e[1] * v.e[2] - u.e[2] * v.e[1],
                 u.e[2] * v.e[0] - u.e[0] * v.e[2],
                 u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { 
    return v / v.length();
}
