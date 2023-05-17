#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"

using namespace std;
using json = nlohmann::json;

color ray_color (const ray& r, const hittable& world, int depth){
    hit_record rec;
    if (depth <= 0) return color (0, 0, 0); //no more light (returns black)
    if (world.hit (r, 0, infinity, rec)){ //hit sphere
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
    }
    vec3 unit_direction = unit_vector (r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //background
}

hittable_list parse_shapes (const json& shapesJson) {
    hittable_list shapes;
    for (const auto& shapeJson: shapesJson) {
        string type = shapeJson["type"];
        if (type == "sphere") {
            point3 center (shapeJson["center"][0], shapeJson["center"][1], shapeJson["center"][2]);
            double radius = shapeJson["radius"];
            shapes.add (make_shared<sphere>(center, radius));
        }
    }
    return shapes;
}

int main(){
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50; 

    json sceneJson;
    ifstream inputFile("scene.json");
    inputFile >> sceneJson;

    hittable_list world = parse_shapes (sceneJson["shapes"]);

    camera cam;

    ofstream outputFile("JSONtest.ppm");
    outputFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--){
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; i++){
            color pixel_color (0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double())/(image_width-1);
                auto v = (j + random_double())/(image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color (outputFile, pixel_color, samples_per_pixel);
        }
    }
    cerr << "\nDone.\n";
    return 0;
}