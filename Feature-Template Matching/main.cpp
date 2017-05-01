//
//  main.cpp
//  OpenVisionMac
//
//  Created by Walter Babcock on 1/18/17.
//  Copyright © 2017 Walter Babcock. All rights reserved.
//

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Manager.cpp"
#include <GLUT/glut.h>

using namespace std;
using namespace cv;

//vManager controls the anchoring, background subtraction, arm and gun detectors
//cap and image are openCV objects
//points will hold the x/y coordinates of the white pixels to be drawn over a black background

VisionManager vManager;
Mat image;
VideoCapture cap;
vector< vector<int> > points;
int gunFoundCount = 0;

//display is openGL's function which is called every time it draws on the screen
//if the vManager is still busy with the previous frame, everything is skipped
//otherwise old data is cleared, a new camera frame is captured, and sent to vManager for processing
//points, arm, and gun will be populated accordingly during this
//arm is drawn as a green block and gun is drawn as a red block if detected

void display(void) {
	if(vManager.isProcessing == 0) {
		points.clear();
		Mat frame;
    	cap >> frame;
    	uint8_t* pixelPtr = (uint8_t*)frame.data;
    	vector<int> arm;
    	vector<int> gun;
    	vManager.newCameraImage(frame.cols, frame.rows, frame.step, frame.channels(), pixelPtr, &points, &arm, &gun);

    	glClearColor(0, 0, 0, 0);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	glColor3d(1.0f, 1.0f, 1.0f);
    	glBegin(GL_POINTS);
    
    	for(int i = 0; i<points.size(); i++) {
    		glVertex2i(points[i][0], points[i][1]);
    	}
    
    	if(arm.size() > 1) {
    		glColor3d(0.0f, 1.0f, 0.0f);
    		for(int x = arm[0]; x<arm[2]+arm[0]; x++) {
    			for(int y = arm[1]; y<arm[3]+arm[1]; y++) {
    				glVertex2i(x, y);
    			}
    		}
    	}
    	else gunFoundCount = 0;
    	
    	if(gun.size() > 1) {
    		gunFoundCount++;
    		if(gunFoundCount > 0) {
    			glColor3d(1.0f, 0.0f, 0.0f);
    			for(int x = gun[0]; x<gun[2]+gun[0]; x++) {
    				for(int y = gun[1]; y<gun[3]+gun[1]; y++) {
    					glVertex2i(x, y);
    				}
    			}
    		}
    	}
    	else if(gunFoundCount > 0) {
    		int gunStartX = arm[2]+arm[0] - 15;
    		int gunStartY = arm[1];
    		glColor3d(1.0f, 0.0f, 0.0f);
    		for(int x = gunStartX; x<gunStartX+41; x++) {
    			for(int y = gunStartY; y<gunStartY+34; y++) {
    				glVertex2i(x, y);
    			}
    		}
    	}
    
    	glEnd();
    	glutSwapBuffers();
	}
	else {
		glClearColor(255, 255, 255, 0);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	glColor3d(0.0f, 0.0f, 0.0f);
    	glBegin(GL_POINTS);
    
    	for(int i = 0; i<points.size(); i++) {
    		glVertex2i(points[i][0], points[i][1]);
    	}
    	glEnd();
    	glutSwapBuffers();
	}
}

//openGL function called if the GUI changes shape
//shouldn't happen ideally

void reshape(int width, int height) {

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the coordinate system, with the origin in the top left
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);

}

//another openGL function
void idle(void) {

    glutPostRedisplay();
}

//first a camera frame is grabbed just to measure its size
//that size is used to properly set up the GUI
//then openGL loop takes over and will not return
//CV processing occurs upon openGL's display function... see above

int main(int argc, char **argv) {

	if(argc > 1) vManager.isMirrored = 1;

    cap.set(CV_CAP_PROP_CONVERT_RGB, 1);
    cap.open(0);
    Mat frame;
    cap >> frame;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(frame.cols*4/15, frame.rows*4/15);
    glutCreateWindow("Gun Vision");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    
    return 0;
}
