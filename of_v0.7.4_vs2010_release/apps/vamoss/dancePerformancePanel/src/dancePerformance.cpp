#include "dancePerformance.h"

//--------------------------------------------------------------
void dancePerformance::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetBackgroundColor(13, 104, 107);
	
	server::setup("127.0.0.1", 11999);

	m_panel.setup();
	m_color.setup();
}

//--------------------------------------------------------------
void dancePerformance::update() {
	
	m_color.update();

}

//--------------------------------------------------------------
void dancePerformance::draw() {
	//m_panel.draw();
	//m_color.draw();
}

void dancePerformance::keyPressed(int key){
	m_panel.keyPressed(key);
	
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
}

void dancePerformance::mouseReleased(int x, int y, int button){
	m_color.mouseReleased(x, y, button);
}

void dancePerformance::windowResized(int w, int h){

}

//--------------------------------------------------------------
void dancePerformance::exit() {

}