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

hittable_list random_scene(){
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random() * random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random (0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


int main(){
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int> (image_width/aspect_ratio);
    const int samples_per_pixel = 300;
    const int max_depth = 50; 

    /*
    json sceneJson;
    ifstream inputFile("scene.json");
    inputFile >> sceneJson;

    hittable_list world = parse_shapes (sceneJson["shapes"]);
    */

    auto world = random_scene();

    point3 lookfrom (13,2,3);
    point3 lookat (0,0,0);
    vec3 vup (0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam (lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    ofstream outputFile("FinalScene.ppm");
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