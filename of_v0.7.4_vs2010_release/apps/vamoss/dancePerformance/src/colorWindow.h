#pragma once


#include "ofMain.h"
#include "ofxFensterManager.h"
#include "colorUI.h"

class colorWindow: public ofxFensterListener {
public:
	colorWindow() {

	}

	void update() {
		m_panel.update();
	}

	void draw() {
		m_panel.draw();
	}

	void mousePressed(int x, int y, int button)
	{
		m_panel.mousePressed(x, y, button);
	}

	void mouseReleased(int x, int y, int button)
	{
		m_panel.mouseReleased(x, y, button);
	}

	
	colorUI m_panel;
};