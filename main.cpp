#include "image.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[]){
    int h, w;
    char* filename;
    for(int i = 1; i < argc; i+= 2){
        const std::string s = std::string(argv[i]);
        if(s == "-height") {
            h = std::atoi(argv[i+1]);
        }
        else if (s == "-width") {
            w = std::atoi(argv[i+1]);
        }
        else if (s == "-output"){
            filename = argv[i+1];
        }
        else {
            std::cout << "Unknown argument: " << argv[i] << " at index " << i << std::endl;
        }
    }
    Image picture = Image(w, h);
    picture.fillBackground(Vec3f(1,0,0));
    picture.savePPM(filename);

    return 0;
}