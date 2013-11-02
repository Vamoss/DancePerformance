#include "dancePerformance.h"

//--------------------------------------------------------------
void dancePerformance::setup() {
	
	config::setup("config.xml");

	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//window
	ofSetFrameRate(30);
	ofSetBackgroundColor(0, 0, 0);
	ofSetWindowPosition(config::window.x, config::window.y);
	ofSetWindowShape(config::window.width, config::window.height);
	ofSetFullscreen(config::fullscreen);

	//synthesizer
	if(config::synthesizerEnabled) server::setup(config::synthesizerIp, config::synthesizerPort);
	
	//facade
	m_facade.setup();

	//panel receiver
	m_receiver.setup(&m_facade, 11999);

}

//--------------------------------------------------------------
void dancePerformance::update() {
	
	m_receiver.update();

	m_facade.update();
	
}

//--------------------------------------------------------------
void dancePerformance::draw() {
	m_facade.draw();
}

		

void dancePerformance::keyPressed(int key){
	m_facade.keyPressed(key);
	
	if (key == 'f') ofToggleFullscreen();
}

void dancePerformance::keyReleased(int key){
	m_facade.keyReleased(key);
}

void dancePerformance::mouseMoved(int x, int y ){
	m_facade.mouseMoved(x, y);
}

void dancePerformance::mouseDragged(int x, int y, int button){
	m_facade.mouseDragged(x, y, button);
}

void dancePerformance::mousePressed(int x, int y, int button){
	m_facade.mousePressed(x, y, button);
}

void dancePerformance::mouseReleased(int x, int y, int button){
	m_facade.mouseReleased(x, y, button);
}

void dancePerformance::windowResized(int w, int h){
	m_facade.windowResized(w, h);
}

//--------------------------------------------------------------
void dancePerformance::exit() {

}