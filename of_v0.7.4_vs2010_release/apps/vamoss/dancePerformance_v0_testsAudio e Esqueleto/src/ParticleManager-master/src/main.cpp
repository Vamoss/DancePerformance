#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	ofAppGlutWindow window;
    //window.setGlutDisplayString("rgba double samples>=4");
	
	ofSetupOpenGL(&window, 1280, 800, OF_WINDOW);
	
	ofRunApp(new testApp());
}
