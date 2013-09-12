#include "dancePerformance.h"

//--------------------------------------------------------------
void dancePerformance::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(30);
	ofBackground(13, 104, 107);
	
	server::setup("127.0.0.1", 11999);

	m_panel.setup();
	m_canvas.setup();
	m_kinect.setup();
	m_color.setup();
	m_gesture.setup();
}

//--------------------------------------------------------------
void dancePerformance::update() {
	
	m_color.update();
	m_canvas.update();

	if(m_canvas.propagateFade){
		m_panel.setParticles(m_canvas.propagatePercent);
	}
}

//--------------------------------------------------------------
void dancePerformance::draw() {

}

void dancePerformance::keyPressed(int key){
	m_panel.keyPressed(key);
	m_canvas.keyPressed(key);
	m_kinect.keyPressed(key);
	m_color.keyPressed(key);
	m_gesture.keyPressed(key);
	
	if (key == 'f') ofToggleFullscreen();
}

void dancePerformance::keyReleased(int key){
	
}

void dancePerformance::mouseMoved(int x, int y ){

}

void dancePerformance::mouseDragged(int x, int y, int button){

}

void dancePerformance::mousePressed(int x, int y, int button){
	m_color.mousePressed(x, y, button);
	m_canvas.mousePressed(x, y, button);
}

void dancePerformance::mouseReleased(int x, int y, int button){
	m_color.mouseReleased(x, y, button);
	m_canvas.mouseReleased(x, y, button);
}

void dancePerformance::windowResized(int w, int h){

}

//--------------------------------------------------------------
void dancePerformance::exit() {

}