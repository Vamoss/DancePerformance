#pragma once

#include "ofMain.h"
#include "panelUI.h"
#include "colorUI.h"
#include "server.h"

class dancePerformance : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void exit();
	
		panelUI m_panel;
		colorUI m_color;
		
};
