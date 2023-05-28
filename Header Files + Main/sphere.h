#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rtweekend.h"
#include "material.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere (point3 cen, double r, shared_ptr <material> m): center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 dist = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(dist, r.direction());
    auto c = dist.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false; //doesn't hit
    auto sqrtd = sqrt (discriminant);

    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root){ //lies within optimal range
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at (rec.t); //intersection point
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal (r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif