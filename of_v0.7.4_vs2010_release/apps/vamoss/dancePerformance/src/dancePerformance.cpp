#include "dancePerformance.h"

//--------------------------------------------------------------
void dancePerformance::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofxFenster* win=ofxFensterManager::get()->createFenster(1024, 0, 455, 768, OF_WINDOW);
	win->addListener(&m_panelWindow);
	win->setBackgroundColor(13, 104, 107);
	win->setWindowTitle("Control Panel");
	m_panelWindow.m_panel.setup(&m_facade);
	

	win=ofxFensterManager::get()->createFenster(1024+455, 0, 455, 768, OF_WINDOW);
	win->addListener(&m_colorWindow);
	win->setBackgroundColor(13, 104, 107);
	win->setWindowTitle("Color Panel");
	m_colorWindow.m_panel.setup(&m_facade);

	//IF the following code is uncommented, all the following windows should be created on the second display, if there is one available
	/*ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
	ofxDisplay* disp = displays[0];
	if(displays.size() > 1)
		disp = displays[1];
	ofxFensterManager::get()->setActiveDisplay(disp);*/

}

//--------------------------------------------------------------
void dancePerformance::update() {

	m_facade.update();
	
}

//--------------------------------------------------------------
void dancePerformance::draw() {
	
	m_facade.draw();
}

		

void dancePerformance::keyPressed(int key){
	m_facade.keyPressed(key);
	
    m_panelWindow.m_panel.keyPressed(key);
	
    m_colorWindow.m_panel.keyPressed(key);
	
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
	//TODO
	//ofxFenster nao esta chamando este evento
	m_facade.windowResized(w, h);
}

//--------------------------------------------------------------
void dancePerformance::exit() {

}