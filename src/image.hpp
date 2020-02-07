#pragma once
#include <vector>
#include "vec3.hpp"
#include <fstream>

using std::endl;

class Image {
    // This class implements basic image manipulation.
    // Conventions:  (0,0) is in the up-left corner, x axis to the right, y axis to the bottom
    std::vector< Vec3f> pixels;
    int height, width; // height = nuber of lines, width = number of colors

    public:

        //Constructors
        Image(){}

        Image(int w, int h){
            height = h;
            width = w;
            pixels.resize(width*height, Vec3f());
        }

        Image(int w, int h, Vec3f color){
            height = h;
            width = w;
            pixels.resize(width*height, color);
        }

        // Image(Image &pic){
        //     height = pic.get_height();
        //     width = pic.get_width();
        //     pixels.resize(width*height, Vec3f());
        // }

        // Save the image to PPM format with 256 colors. Erase pre-existing file without warning.
        void savePPM(const char* filename){
            std::ofstream outfile;
            outfile.open(filename, std::ios::out|std::ios::trunc); 

            outfile << "P3" << endl;
            outfile << width << " " << height << endl;
            outfile << 255 << endl;

            for(int j=height-1;j>=0; j--){
                for(int i=width-1; i>=0; i--) {
                    const Vec3f color = pixels[j*width+i]*255;
                    outfile << int(color[0]) << " "<< int(color[1]) << " "<< int(color[2]) <<endl;
                }
                outfile << endl;                 
            }
        }

        // Fills the image with gradient from black to color following the y axis
        void fillBackground(Vec3f color){
            for(int i=0; i<3; i++)
                color[i] = (color[i]>1) ? 1 : color[i];

            for(int j=0;j<height; j++){
                const float a = float(j)/height; 
                for(int i=0; i<width; i++)
                    pixels[j*width+i] = a*color + (1-a)*pixels[j*width+i];
            }
        }

        inline const Vec3f & operator() (size_t x, size_t y) const { return pixels[y*width+x]; }
    	inline Vec3f & operator() (size_t x, size_t y) { return pixels[y*width+x]; }

        void set(int w, int h, Vec3f color) {
            pixels[h*width+w] = color;
        }

        int get_height(){
            return height;
        }

        int get_width(){
            return width;
        }
};