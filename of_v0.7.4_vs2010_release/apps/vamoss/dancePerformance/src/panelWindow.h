#pragma once


#include "ofMain.h"
#include "ofxFensterManager.h"
#include "panelUI.h"
#include "colorUI.h"

class panelWindow: public ofxFensterListener {
public:
	panelWindow() {

	}

	void update() {
		m_color.update();
	}

	void draw() {
		m_panel.draw();
		m_color.draw();
	}

	void mousePressed(int x, int y, int button)
	{
		m_color.mousePressed(x, y, button);
	}

	void mouseReleased(int x, int y, int button)
	{
		m_color.mouseReleased(x, y, button);
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
	colorUI m_color;
};