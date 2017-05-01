//this module runs the background subtraction
//background property stores the pixel values of what we think is the non-moving part of image
//last frame holds the values of the last frame sent
//pixelCertainty holds a value showing how sure we are that this pixel is part of the background
//if a new pixel from new frame comes in and is exactly the same, then the certainty is increased
//if different, then the certainty is decreased

//isGenerated has three states, -1 means nothing has occurred or camera has been unanchored
//0 means not enough frames have come in 
//1 means background values have successfully been created
//this is determined by summing up all the certainty values over and over
//timesConfirmed and timesWrong are incremented based on the result of this summing up

//disappearFactor controls how fast bad background values are eliminated
//that is we have a certain values, but new pixels repeatedly come in with a different value

#include "Anchor.cpp"

using std::vector;
using std::array;

class BackgroundGenerator {
    public:
    
    vector< vector<Pixel> > background;
    vector< vector<Pixel> > lastFrame;
    vector< vector<int>> pixelCertainty;
    int isGenerated = -1;
    int height = 0;
    int width = 0;
    int timesConfirmed = 0;
    int timesWrong = 0;
    int disappearFactor = -700;
    
    //called when camera becomes unanchored
    void reset() {
        isGenerated = -1;
        background.clear();
        lastFrame.clear();
        pixelCertainty.clear();
        timesConfirmed = 0;
        timesWrong = 0;
        height = 0;
        width = 0;
    }
    
    //called when nothing else has happened
    //sets initial values and moves isGenerated from -1 to 0
    void initializeBackground(vector< vector<Pixel> > b) {
        isGenerated = 0;
        background = b;
        lastFrame = b;
        pixelCertainty.clear();
        timesConfirmed = 0;
        timesWrong = 0;
        height = b.size();
        width = b[0].size();
        for(int h = 0; h<height; h++) {
            vector<int> vI;
            for(int w = 0; w<width; w++) {
                vI.push_back(0);
            }
            pixelCertainty.push_back(vI);
        }
    }
    
    //gets a blank template of 0 values
    //some of these will be switched to 1's upon comparison between background and new frame
    //this binary map will be passed back to visionManager as return of new camera input
    vector< vector<Pixel>> getTemplate(int widt, int heigh) {
        vector< vector<Pixel>> templat;
        Pixel p;
        for(int h = 0; h<heigh; h++) {
            vector<Pixel> vI;
            for(int w = 0; w<widt; w++) {
                vI.push_back(p);
            }
            templat.push_back(vI);
        }
        return templat;
    }
    
    //determines if background should be considered generated or a reset should happen if too many bad values are present
    void checkStatus() {
        if(timesConfirmed > 100 && timesWrong < timesConfirmed/2) {isGenerated = 1;}
        else if(timesWrong > 100 && timesWrong > timesConfirmed*2) {initializeBackground(lastFrame);}
    }
    
    //removes noise from binary map
    //only 1's remain in output if 19/25 of surrounding values are also 1's
    //uses dynamic programming
    vector< vector<int>> cleanMap(vector< vector<int>> input, vector< vector<int> >* points) {
  		vector< vector<int>> output = input;

  		int fW = 5;
  		int fH = 5;
  		int w = input[0].size();
  		int h = input.size();
  
  		for(int i = 0; i<h-fH; i++) {
    		double total = 0;
  			int colPointer = 0;
  			vector<double> colTotals;
  			for(int ii = 0; ii<fW; ii++) {
  				int cTot = 0;
  				for(int jj = i; jj<fH+i; jj++) {
  					cTot += input[jj][ii];
  				}
  				colTotals.push_back(cTot);
  				total += cTot;
  			}
  			if(total > 10) {
  				output[fH/2][fW/2] = 1;
  				vector<int> coor;
                coor.push_back(fW/2);
                coor.push_back(fH/2);
                points->push_back(coor);
  			}
  			else output[fH/2][fW/2] = 0;
  			
  			for(int j = 1; j<w-fW; j++) {
  				int cTot = 0;
  				for(int jj = i; jj<fH+i; jj++) {
  					cTot += input[jj][j+fW-1];
  				}
  				total = total - colTotals[colPointer] + cTot;
  				colTotals[colPointer] = cTot;
  				colPointer++;
  				if(colPointer >= colTotals.size()) colPointer = 0;
  				if(total > 10) {
  					output[i+fH/2][j+fW/2] = 1;
  					vector<int> coor;
                    coor.push_back(j+fW/2);
                	coor.push_back(i+fH/2);
                    points->push_back(coor);
  				}
  				else output[i+fH/2][j+fW/2] = 0;
  			}
  		}
  
  		return output;
	}
    
    //compares pixel values of new camera input to current background
    //called when isGenerated = 0
    //adjusts pixelCertainty for each comparison, and sums them up
    //increments timesConfirmed if lots of pixels match, otherwise increments timesWrong
    void addFrame(vector< vector<Pixel> > newFrame) {
        int totalCorrect = 0;
        for(int w = 0; w<width; w++) {
            for(int h =0; h<height; h++) {
                int cert = ratePixels(background[h][w], newFrame[h][w]);
                pixelCertainty[h][w] += cert;
                if(pixelCertainty[h][w] < disappearFactor && ratePixels(lastFrame[h][w], newFrame[h][w]) >= 0) {
                    pixelCertainty[h][w] = 0;
                    background[h][w] = newFrame[h][w];
                }
                if(cert > -5) totalCorrect++;
            }
        }
        float percentCorrect = (float)(totalCorrect)/(float)(height*width);
        if( percentCorrect > 0.9) {timesConfirmed += 2;}
        else if( percentCorrect > 0.8) {timesConfirmed += 1;}
        else if(percentCorrect < 0.1) {timesWrong += 5; timesConfirmed -= 5;}
        else if(percentCorrect < 0.2) {timesWrong += 4; timesConfirmed -= 4;}
        else if(percentCorrect < 0.3) {timesWrong += 3; timesConfirmed -= 3;}
        else if(percentCorrect < 0.4) {timesWrong += 2; timesConfirmed -= 1;}
        else if(percentCorrect < 0.5) {timesWrong += 1;}
        checkStatus();
        lastFrame = newFrame;
    }
    
    //called when isGenerated = 1
    //does the same as above, but will return a binary map showing where pixels did not match background
    //will swap background pixels will values in newFrame if they have a low certainty, and match the last frame's value
    vector< vector<int> > addFrameWithReturn(vector< vector<Pixel> > newFrame, int bodyFound, vector< vector<int> >* points) {
        vector< vector<int>> differenceMap;
        int totalCorrect = 0;
        for(int h = 0; h<height; h++) {
            vector<int> dRow;
            for(int w =0; w<width; w++) {
                int cert = ratePixels(background[h][w], newFrame[h][w]);
                if(bodyFound == 0) {
                    pixelCertainty[h][w] += cert;
                    if(pixelCertainty[h][w] < disappearFactor && ratePixels(lastFrame[h][w], newFrame[h][w]) >= 0) {
                        pixelCertainty[h][w] = 0;
                        background[h][w] = newFrame[h][w];
                    }
                }
                
                if(cert > -5) {
                    totalCorrect++;
                    dRow.push_back(0);
                }
                else {
                	dRow.push_back(1);
                	/*vector<int> coor;
                    coor.push_back(w);
                	coor.push_back(h);
                    points->push_back(coor);*/
                }
            }
            differenceMap.push_back(dRow);
        }
        float percentCorrect = (float)(totalCorrect)/(float)(height*width);
        if( percentCorrect > 0.9) {timesConfirmed += 2;}
        else if( percentCorrect > 0.8) {timesConfirmed += 1;}
        else if(percentCorrect < 0.1) {timesWrong += 5; timesConfirmed -= 5;}
        else if(percentCorrect < 0.2) {timesWrong += 4; timesConfirmed -= 4;}
        else if(percentCorrect < 0.3) {timesWrong += 3; timesConfirmed -= 3;}
        else if(percentCorrect < 0.4) {timesWrong += 2; timesConfirmed -= 1;}
        else if(percentCorrect < 0.5) {timesWrong += 1;}
        checkStatus();
        if(bodyFound == 0) lastFrame = newFrame;
        
        vector< vector<int>> diffMap = cleanMap(differenceMap, points);
        return diffMap;
        //return differenceMap;
    }
    
};
