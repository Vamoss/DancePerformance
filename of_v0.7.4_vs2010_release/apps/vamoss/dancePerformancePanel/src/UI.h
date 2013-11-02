#pragma once

#include "ofxUI.h"

class UI {
    
public:
    
	ofxUICanvas		*gui1;
	
	virtual void setup(){};
	virtual void update(){};
	virtual void draw(){};
	virtual void keyPressed(int key){};
	virtual void mousePressed(int x, int y, int button){};
	virtual void mouseReleased(int x, int y, int button){};
};
