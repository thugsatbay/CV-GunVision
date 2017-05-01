//contains other important CV modules and orchestrates behavior between them
//templat will be colored image from camera buffer

#include "Weapon.cpp"
#include <iostream>

using std::vector;
using std::array;

class VisionManager {
    public:
    
    AnchorDetector anchorDetector;
    BackgroundGenerator backgroundGenerator;
    BodyDetector bodyDetector;
    ArmDetector armDetector;
    WeaponDetector weaponDetector;
    vector< vector<Pixel>> templat;
    
    int isProcessing = 0;
    int isMirrored = 0;

    void newCameraImage(int width, int height, int step, int channels, uint8_t* pixelPtr, vector< vector<int> >* points, vector<int>* arm, vector<int>* gun) {
    	//generate new anchors and send to anchor detector
    	vector<Anchor> newAnchors;
    	array< array<int, 2>, 8> coors = anchorDetector.getAnchorCoor(width, height);  
    	for(int i = 0; i<coors.size(); i++) {
        	Anchor a;
        	a.setValues(coors[i][0], coors[i][1], calcPixel2(pixelPtr[coors[i][0]*height + coors[i][1]], pixelPtr[coors[i][0]*height + coors[i][1] + 1], pixelPtr[coors[i][0]*height + coors[i][1] + 2]));
        	newAnchors.push_back(a);
    	}
    	if(anchorDetector.isAnchored == -1) {
        	anchorDetector.initializeAnchors(newAnchors);
        	if(backgroundGenerator.isGenerated == 0) backgroundGenerator.reset();
    	}
    	else if(anchorDetector.isAnchored == 0) {
    		anchorDetector.checkAnchors(newAnchors);
    	}
    	else {
    		//if anchored, will retrieve a shrunken and cropped image from camera buffer
    		anchorDetector.checkAnchors(newAnchors);
        	if(anchorDetector.checkAnchors(newAnchors) == 1) {
        		int diffuser = 3;
        		int nHeight = (4*height/5)/diffuser;
        		int nWidth = (4*width/5)/diffuser;
        		templat = backgroundGenerator.getTemplate(nWidth, nHeight);
        		int wBuf = width/10;
        		int hBuf = height/10;
        		
        		if(isMirrored == 0) {
        			for(int w = 0; w<nWidth; w++) {
        				for(int h = 0; h<nHeight; h++) {
        					int w1 = w*diffuser + wBuf;
        					int h1 = h*diffuser + hBuf;
        					int p1 = h1*step + w1*channels;
        					templat[h][w] = calcPixel(pixelPtr[p1], pixelPtr[p1 + 1], pixelPtr[p1 + 2]);
        				}
        			}
        		}
        		else {
        			int realW = 0;
        			for(int w = nWidth-1; w>=0; w--) {
        				for(int h = 0; h<nHeight; h++) {
        					int w1 = w*diffuser + wBuf;
        					int h1 = h*diffuser + hBuf;
        					int p1 = h1*step + w1*channels;
        					templat[h][realW] = calcPixel(pixelPtr[p1], pixelPtr[p1 + 1], pixelPtr[p1 + 2]);
        				}
        				realW++;
        			}
        		}
        		
        		//if background has not been initialized and created, send new data with no return
        		if(backgroundGenerator.isGenerated == -1) {
        			backgroundGenerator.initializeBackground(templat);
        		}
        		else if(backgroundGenerator.isGenerated == 0) {
        			backgroundGenerator.addFrame(templat);
        		}
        		else {
        			//otherwise background has been generated, and receive a binary map
        			//this is processed for arm and then gun detection
        			//arm searches whole image, but gun detection only searches around the end of an arm
        			//currently only works for gun pointing to the right side of screen, and person standing six to eight feet away
        			isProcessing = 1;
        			vector< vector<int>> pic = backgroundGenerator.addFrameWithReturn(templat, 0, points);
        			*arm = armDetector.scanForArm(pic, 130, 21);
        			if(arm->size() > 1) {
        				int startX = (*arm)[0]+(*arm)[2]-65;
        				if(startX<0)startX=0;
        				int startY = (*arm)[1]-30;
        				if(startY<0)startY=0;
        				*gun = weaponDetector.scanForGun(pic, &templat, startX, startY, 41, 34);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunNear(pic, &templat, startX, startY, 47, 41);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunFar(pic, &templat, startX, startY, 35, 29);
        				/*if(gun->size() == 0) *gun = weaponDetector.scanForGunDown(pic, &templat, startX, startY, 41, 34);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunNearUp(pic, &templat, startX, startY, 47, 41);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunFarDown(pic, &templat, startX, startY, 35, 29);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunUp(pic, &templat, startX, startY, 41, 34);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunNearDown(pic, &templat, startX, startY, 47, 41);
        				if(gun->size() == 0) *gun = weaponDetector.scanForGunFarUp(pic, &templat, startX, startY, 35, 29); */      				      				
        			}
        			isProcessing = 0;
        		}
        	}
    	}
    }
    
    
};


