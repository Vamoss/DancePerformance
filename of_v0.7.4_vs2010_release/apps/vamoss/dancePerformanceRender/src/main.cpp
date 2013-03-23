#include "dancePerformance.h" // Needs to be in front of ofMain.h
#include "ofMain.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp( new dancePerformance());

}
