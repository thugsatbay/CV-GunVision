//contains classes and functions to compute pixel data from raw camera input buffer
//also functions to compare two pixel values

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

using std::max;
using std::min;

class Pixel {
    public:
    
    int red;
    int green;
    int blue;
    int gray;
    
    void setValues(int r,int g,int b) {
        red = r; green = g; blue = b;
        gray = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    }
};

class Anchor {
    public:
    
    int x;
    int y;
    Pixel pixel;
    
        void setValues(int xx, int yy, Pixel p) {
            x = xx; y = yy; pixel = p;
        }
};

Pixel calcPixel(uint8_t yp, uint8_t cb, uint8_t cr) {
    double ri = double(yp) + 1.402 * (double(cr) - 128);
    double gi = double(yp) - 0.34414 * (double(cb) - 128) - 0.71414 * (double(cr) - 128);
    double bi = double(yp) + 1.772 * (double(cb) - 128);
    Pixel p;
    p.setValues(int(min(max(ri,0.0), 255.0)), int(min(max(bi,0.0), 255.0)), int(min(max(gi,0.0), 255.0)));
    return p;
}

Pixel calcPixel2(uint8_t yp, uint8_t cb, uint8_t cr) {
    Pixel p;
    p.setValues(int(yp), int(cb), int(cr));
    return p;
}
    

int checkPixel(Pixel p1, Pixel p2) {
    if(abs(p1.red - p2.red) > 20) return 1;
    if(abs(p1.green - p2.green) > 20) return 1;
    if(abs(p1.blue - p2.blue) > 20) return 1;
    return 0;
}

int checkGray(Pixel p1, Pixel p2) {
    if(abs(p1.gray - p2.gray) > 20) return 1;
    return 0;
}


int pixelChart(int p, int total) {
    switch(p) {
        case 0:
            total += 3;
            break;
        case 1:
            total += 2;
            break;
        case 2:
            total += 1;
            break;
        case 3:
            break;
        case 4:
            total -= 1;
            break;
        case 5:
            total -= 2;
            break;
        case 6:
            total -= 3;
            break;
        case 7:
            total -= 4;
            break;
        case 8:
            total -= 5;
            break;
        default:
            total -= 8;
            break;
    }
    return total;
}

int ratePixels(Pixel p1, Pixel p2) {
    int total = 0;
    int dR = (abs(p1.red - p2.red))/10;
    int dG = (abs(p1.green - p2.green))/10;
    int dB = (abs(p1.blue - p2.blue))/10;
    total += pixelChart(dR, total);
    total += pixelChart(dG, total);
    total += pixelChart(dB, total);
    return total;
}


