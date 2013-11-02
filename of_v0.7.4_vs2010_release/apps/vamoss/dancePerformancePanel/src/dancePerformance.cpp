#include "dancePerformance.h"

//--------------------------------------------------------------
void dancePerformance::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(30);
	ofBackground(13, 104, 107);
	
	server::setup("127.0.0.1", 11999);
	
	m_panel = new panelUI();
	m_canvas = new canvasUI();
	m_kinect = new kinectUI();
	m_color = new colorUI();
	m_gesture = new gestureUI();
	m_recorder = new recorderUI();
	
	panels.push_back(m_panel);
	panels.push_back(m_canvas);
	panels.push_back(m_kinect);
	panels.push_back(m_color);
	panels.push_back(m_gesture);
	panels.push_back(m_recorder);

	for(int i=0; i<panels.size(); i++){
		panels[i]->setup();
	}

	m_recorder->setupPanelsToRecord(panels);
}

//--------------------------------------------------------------
void dancePerformance::update() {
	
	for(int i=0; i<panels.size(); i++){
		panels[i]->update();
	}

	if(m_canvas->propagateFade){
		m_panel->setParticles(m_canvas->propagatePercent);
	}
}

//--------------------------------------------------------------
void dancePerformance::draw() {

}

void dancePerformance::keyPressed(int key){
	for(int i=0; i<panels.size(); i++){
		panels[i]->keyPressed(key);
	}
	
	if (key == 'f') ofToggleFullscreen();
}

void dancePerformance::keyReleased(int key){
	
}

void dancePerformance::mouseMoved(int x, int y ){

}

void dancePerformance::mouseDragged(int x, int y, int button){

}

void dancePerformance::mousePressed(int x, int y, int button){
	for(int i=0; i<panels.size(); i++){
		panels[i]->mousePressed(x, y, button);
	}
}

void dancePerformance::mouseReleased(int x, int y, int button){
	for(int i=0; i<panels.size(); i++){
		panels[i]->mouseReleased(x, y, button);
	}
}

void dancePerformance::windowResized(int w, int h){

}

//--------------------------------------------------------------
void dancePerformance::exit() {

}