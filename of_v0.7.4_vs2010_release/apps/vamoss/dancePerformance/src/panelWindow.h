#pragma once


#include "ofMain.h"
#include "ofxFensterManager.h"
#include "panelUI.h"

class panelWindow: public ofxFensterListener {
public:
	panelWindow() {

	}

	void draw() {
		m_panel.draw();
	}

	void mouseMoved(int x, int y) {

	}

	void dragEvent(ofDragInfo dragInfo) {
		cout << "GOT SOME FILES: "<<endl;
		std::vector<string>::iterator it = dragInfo.files.begin();
		while(it != dragInfo.files.end()) {
			cout << *it << endl;
			++it;
		}
	}

	
	panelUI m_panel;
};