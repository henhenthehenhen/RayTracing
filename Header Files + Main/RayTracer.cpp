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
#include "material.h"

using namespace std;
using json = nlohmann::json;

color ray_color (const ray& r, const hittable& world, int depth){
    hit_record rec;
    if (depth <= 0) return color (0, 0, 0); //no more light (returns black)
    if (world.hit (r, 0.001, infinity, rec)){ //hit sphere
        ray scattered;
        color attenuation; //reducing light intensity
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * ray_color (scattered, world, depth-1); 
        }
        return color (0, 0, 0); 
    }
    vec3 unit_direction = unit_vector (r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //background
}

hittable_list parse_shapes(const json& shapesJson){
    hittable_list shapes;
    for (const auto& shapeJson : shapesJson) {
        string type = shapeJson["type"];
        if (type == "sphere"){
            point3 center (shapeJson["center"][0], shapeJson["center"][1], shapeJson["center"][2]);
            double radius = shapeJson["radius"];
            auto materialJson = shapeJson["material"];
            string materialType = materialJson["type"];
            shared_ptr<material> mat_ptr;
            if (materialType == "lambertian"){
                color albedo (materialJson["albedo"][0], materialJson["albedo"][1], materialJson["albedo"][2]);
                mat_ptr = make_shared<lambertian>(albedo);
            } 
            else if (materialType == "metal"){
                color albedo(materialJson["albedo"][0], materialJson["albedo"][1], materialJson["albedo"][2]);
                double fuzz = materialJson["fuzz"];
                mat_ptr = make_shared <metal> (albedo, fuzz);
            }
            else if (materialType == "dielectric"){
                double ir = materialJson["ir"];
                mat_ptr = make_shared <dielectric> (ir); 
            }
            shapes.add (make_shared<sphere> (center, radius, mat_ptr));
        }
    }
    return shapes;
}



int main(){
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int> (image_width/aspect_ratio);
    const int samples_per_pixel = 200;
    const int max_depth = 50; 

    json sceneJson;
    ifstream inputFile("scene.json");
    inputFile >> sceneJson;

    hittable_list world = parse_shapes (sceneJson["shapes"]);

    camera cam (point3 (-2, 2, 1), point3 (0, 0, -1), vec3 (0, 1, 0), 90, aspect_ratio);

    ofstream outputFile("camera.ppm");
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