#pragma once


#include "ofMain.h"
#include "ofxFensterManager.h"

class panelWindow: public ofxFensterListener {
public:
	panelWindow() {
		rotX = ofRandom(-20, 20);
		rotY = ofRandom(-10, 10);
	}

	void draw() {
		ofPushMatrix();
		ofPushStyle();
			ofSetColor(255);
			ofNoFill();
			ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5, 0);
			ofRotateX(rotX);
			ofRotateY(rotY);
			ofBox(0, 0, 0, 100);
		ofPopStyle();
		ofPopMatrix();
	}

	void mouseMoved(int x, int y) {
		rotY = ofMap(x, 0, ofGetWidth(), -20, 20);
		rotX = ofMap(y, 0, ofGetHeight(), 60, -60);
	}

	void dragEvent(ofDragInfo dragInfo) {
		cout << "GOT SOME FILES: "<<endl;
		std::vector<string>::iterator it = dragInfo.files.begin();
		while(it != dragInfo.files.end()) {
			cout << *it << endl;
			++it;
		}
	}


	float rotX;
	float rotY;
};