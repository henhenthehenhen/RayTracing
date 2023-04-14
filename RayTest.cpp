#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

using namespace std;

double hit_sphere (const point3& center, double radius, const ray& r){
    vec3 dist = r.origin() - center; //distance from ray to sphere
    auto a = r.direction().length_squared();
    auto half_b = dot (dist, r.direction());
    auto c = dist.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return -1.0; //doesn't hit 
    else return (-half_b-sqrt(discriminant))/a; //distance b/w origin of ray and closest hit point
}

color ray_color (const ray& r){
    double t = hit_sphere (point3 (0, 0, -1), 0.5, r);
    if (t > 0.0){ //hit
        vec3 N = unit_vector (r.at(t) - vec3(0, 0, -1)); //normal vector
        return 0.5 * color (N.x()+1, N.y()+1, N.z()+1); //color b/w [0, 1] (dimmed by half)
    } 
    vec3 unit_direction = unit_vector (r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t)*color (1.0, 1.0, 1.0) + t*color (0.5, 0.7, 1.0); //background (gradient)
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0; //widescreen 
    const int image_width = 400; 
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto fdistal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, fdistal_length);

    // Render

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
       cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r (origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(cout, pixel_color);
        }
    }
    cerr << "\nDone.\n";
}
