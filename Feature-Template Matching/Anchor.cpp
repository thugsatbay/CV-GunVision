
//AnchorDetector has three states represented by isAnchored
//-1 means nothing has occurred
//0 means initial anchors have been set, but comparison to previous anchors failed
//1 means camera is anchors - that is comparison of current anchors to previous anchors was successful
//Anchor class is found in Color.cpp
//it is x/y coordinates and a Pixel value
//AnchorDetector will receive eight anchors that are spread out along the edges/corners of the screen

#include "Color.cpp"

using std::vector;
using std:: array;

class AnchorDetector {
    public:
    
    int isAnchored = -1;
    vector<Anchor> anchors;
    
    //only called when inAnchored is -1
    //moves state to 0, and sets values of anchors
    void initializeAnchors(vector<Anchor> a) {
        isAnchored = 0;
        anchors = a;
    }
    
    //this function checks the anchors by receiving a new frame
    //it is actually never called and potentially can be removed
    int checkAnchors(vector< vector<Pixel> > newFrame) {
        int badCount = 0;
        for(int i = 0; i<anchors.size(); i++) {
            badCount += checkPixel(anchors[i].pixel, newFrame[anchors[i].y][anchors[i].x]);
            anchors[i].pixel = newFrame[anchors[i].y][anchors[i].x];
        }
        if(anchors.size() - badCount > 2) isAnchored = 1;
        else isAnchored = 0;
        return isAnchored;
    }
    
    //compares old anchor values to new anchor values
    // if at least 3/8 are similar, then success
    //otherwise failure and no further processing will occur on that camera input
    //old anchor values are replaced with new anchor values after this function
    int checkAnchors(vector<Anchor> newAnchors) {
        int badCount = 0;
        for(int i = 0; i<anchors.size(); i++) {
            badCount += checkPixel(anchors[i].pixel, newAnchors[i].pixel);
        }
        if(anchors.size() - badCount > 2) isAnchored = 1;
        else isAnchored = 0;
        return isAnchored;
    }
    
    //generates locations of anchors when given a particular screen width and height
    array< array<int,2>, 8> getAnchorCoor(int w, int h) {
        array< array<int,2>, 8> o;
        o[0][0] = 1; o[0][1] = 1;
        o[1][0] = w-1; o[1][1] = 1;
        o[2][0] = 1; o[2][1] = h-1;
        o[3][0] = w-1; o[3][1] = h-1;
        o[4][0] = w/2; o[4][1] = 1;
        o[5][0] = 1; o[5][1] = h/2;
        o[6][0] = w-1; o[6][1] = h/2;
        o[7][0] = w/2; o[7][1] = h-1;
        return o;
    }
    
    
};


    
