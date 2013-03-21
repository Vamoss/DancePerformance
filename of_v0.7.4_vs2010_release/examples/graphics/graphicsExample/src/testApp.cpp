#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	counter = 0;
	ofBackground(255,255,255);

	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
}

//--------------------------------------------------------------
void testApp::update(){
	counter = counter + 0.033f;
}

//--------------------------------------------------------------
void testApp::draw(){

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,0,0);
	float radius = 50 + 10 * sin(counter);
	ofFill();
	ofCircle(110,80,radius);

	ofNoFill();
	ofSetHexColor(0xCCCCCC);
	ofRect(0,0,220, 160);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	ofSetCircleResolution((float)y/(float)ofGetHeight()*30);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}