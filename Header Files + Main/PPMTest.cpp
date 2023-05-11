#include "PpmImage.h"

#include <bits/stdc++.h>

using namespace std;

int main(){
    PpmImage image(200, 100);
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 100; j++) {
            if (i < 66) {
                image.setPixel(i, j, color(1, 0, 0)); // red
            } 
            else if (i < 133) {
                image.setPixel(i, j, color(0, 1, 0)); // green
            } 
            else {
                image.setPixel(i, j, color(0, 0, 1)); // blue
            }
        }
    }
    image.writeFile();
}