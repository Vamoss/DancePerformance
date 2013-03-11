#include "dancePerformance.h" // Needs to be in front of ofMain.h
#include "ofMain.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	ofSetupOpenGL(ofxFensterManager::get(), 1024,768, OF_WINDOW);

	ofRunFensterApp( new dancePerformance());

}
