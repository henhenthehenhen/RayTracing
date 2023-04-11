#ifndef PPMIMAGE_H
#define PPMIMAGE_H

#include "color.h"
#include "vec3.h"
#include "vec3utility.h"

#include <bits/stdc++.h>

using namespace std;

class PpmImage {
public:
    PpmImage (int w, int h) : width(w), height(h){
        pixels.resize (w * h); //1d but stores 2d image (first w elements represent first row, next w elements represent second...)
    }
    void setPixel (int x, int y, const color& c){
        pixels[y*width + x] = c;
    }
    void writeFile () const {
        cout << "P3\n" << width << " " << height << "\n255\n";
        for (int j = height-1; j >= 0; --j) {
            for (int i = 0; i < width; ++i) {
                write_color(cout, pixels[j*width + i]);
            }
        }
    }
    string writeString() const {
        string result = "P3\n" + to_string(width) + " " + to_string(height) + "\n255\n";
        for (int j = height-1; j >= 0; --j) {
            for (int i = 0; i < width; ++i) {
                const color& c = pixels[j*width + i];
                result += to_string(static_cast<int>(255.999*c.x())) + " "
                        + to_string(static_cast<int>(255.999*c.y())) + " "
                        + to_string(static_cast<int>(255.999*c.z())) + "\n";
            }
        }
        return result;
    }

private:
    int width;
    int height;
    vector <color> pixels; 
};

#endif