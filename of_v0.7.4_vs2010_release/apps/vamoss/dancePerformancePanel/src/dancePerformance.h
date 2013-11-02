#pragma once

#include "ofMain.h"
#include "UI.h"
#include "panelUI.h"
#include "canvasUI.h"
#include "kinectUI.h"
#include "colorUI.h"
#include "gestureUI.h"
#include "recorderUI.h"
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
	
		vector<UI *> panels;
		panelUI * m_panel;
		canvasUI * m_canvas;
		kinectUI * m_kinect;
		colorUI * m_color;
		gestureUI * m_gesture;
		recorderUI * m_recorder;


		
};
