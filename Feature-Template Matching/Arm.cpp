//
//  Arm.cpp
//  MacVision
//
//  Created by Walter Babcock on 1/17/17.
//  Copyright © 2017 Walter Babcock. All rights reserved.
//

#include "Body.cpp"

using std::vector;

class ArmDetector {
    public:
    
    //creates an arm template for convolution
    //there are three main areas placed on top of each other, each equal to armWidth x armHeight
    //the middle one contains positive values while the upper and lower contain negative values
    vector< vector<int>> setLeftTemplat(int armWidth, int armHeight) {
    	vector< vector<int>> templat;
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		int bad = -3;
    		if(y<armHeight/2) bad = -10;
    		for(int x = 0; x<armWidth*1; x++) row.push_back(bad);
    		templat.push_back(row);
    	}
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		for(int x = 0; x<armWidth*1; x++) {
    			if(x>armWidth) row.push_back(-10);
    			else row.push_back(10);
    		}
    		templat.push_back(row);
    	}
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		int bad = -3;
    		if(y>armHeight/2) bad = -10;
    		for(int x = 0; x<armWidth*1; x++) row.push_back(bad);
    		templat.push_back(row);
    	}
    	return templat;
    }
    
    //same as above but with negative values in the middle section on the other side
    //not used currently
    vector< vector<int>> setRightTemplat(int armWidth, int armHeight) {
    	vector< vector<int>> templat;
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		int bad = -1;
    		if(y<armHeight/2) bad = -10;
    		for(int x = 0; x<armWidth*1.25; x++) row.push_back(bad);
    		templat.push_back(row);
    	}
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		for(int x = 0; x<armWidth*1.25; x++) {
    			if(x<armWidth*0.25) row.push_back(-100);
    			else row.push_back(10);
    		}
    		templat.push_back(row);
    	}
    	
    	for(int y = 0; y<armHeight; y++) {
    		vector<int> row;
    		int bad = -1;
    		if(y>armHeight/2) bad = -10;
    		for(int x = 0; x<armWidth*1.25; x++) row.push_back(bad);
    		templat.push_back(row);
    	}
    	return templat;
    }
    
    //uses dynamic programming to scan binary image to match arm template
    //column totals are stored in colSums vector
    //each time the template moves to the right, a new column sum is added and the old column pointed out by
    //colPointer is subtracted, then colPointer moves to the next colSum
    //only the pixel with the highest arm template score is considered
    //this is the top-left of the arm template
    //only left arm is used at the moment
    vector<int> scanForArm(vector< vector<int>> pic, int armWidth, int armHeight) {
    	if(pic.size() == 0 || pic[0].size() == 0) {
    		vector<int> finalValues;
    		return finalValues;
    	}
    
    	vector< vector<int>> leftTemplat = setLeftTemplat(armWidth, armHeight);
    	vector< vector<int>> rightTemplat = setRightTemplat(armWidth, armHeight);
    	
    	int highLeftScore = -1000000, highLeftCoorX = 0, highLeftCoorY = 0;
    	int highRightScore = -1000000, highRightCoorX = 0, highRightCoorY = 0;
    	
    	for(int y = 0; y<pic.size()-leftTemplat.size()-1; y+=4) {
    		vector<int> colSums;
    		int colPointer = 0, grandTotal = 0;
    		for(int tx = 0; tx<leftTemplat[0].size(); tx++) {
    			int colTot = 0;
    			for(int ty = 0; ty<leftTemplat.size(); ty++) {
    				 colTot += leftTemplat[ty][0] * pic[y+ty][tx];
    				 grandTotal += leftTemplat[ty][0] * pic[y+ty][tx];
    			}
    			colSums.push_back(colTot);
    		}
    		for(int x = 1; x<pic[0].size()-leftTemplat[0].size()-1; x++) {
    			int colTot = 0;
    			for(int ty = 0; ty<leftTemplat.size(); ty++) colTot += leftTemplat[ty][0] * pic[y+ty][x+leftTemplat[0].size()];
    			grandTotal = grandTotal + colTot - colSums[colPointer];
    			colSums[colPointer] = colTot;
    			colPointer++;
    			if(colPointer >= colSums.size()) colPointer = 0;
    			
    			if(grandTotal > highLeftScore) {
    				highLeftScore = grandTotal;
    				highLeftCoorX = x;
    				highLeftCoorY = y + armHeight;
    			}
    		}
    	}
    	
    	vector<int> finalValues;
    	if(highLeftScore > armWidth*armHeight*3) {
    		finalValues.push_back(highLeftCoorX);
    		finalValues.push_back(highLeftCoorY);
    		finalValues.push_back(armWidth);
    		finalValues.push_back(armHeight);
    		finalValues.push_back(0);
    	}
    	return finalValues;
    }
        
        
};