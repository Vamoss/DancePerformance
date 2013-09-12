#include "config.h"

ofRectangle config::window;
bool config::fullscreen;

bool config::useLog;


bool config::synthesizerEnabled;
string config::synthesizerIp;
int config::synthesizerPort;

void config::setup(string xmlPath)
{		
	ofxXmlSettings xml;
	if(xml.loadFile(xmlPath)){

		xml.pushTag("root", 0);

			window.x = xml.getAttribute("window", "x", 0, 0);
			window.y = xml.getAttribute("window", "y", 0, 0);
			window.width = xml.getAttribute("window", "width", 0, 0);
			window.height = xml.getAttribute("window", "height", 0, 0);
			fullscreen = xml.getAttribute("window", "fullscreen", "1", 0) == "1";

			useLog = xml.getValue("log", "1", 0) == "1";

			synthesizerEnabled = xml.getAttribute("synthesizer", "enable", "1", 0) == "1";
			synthesizerIp = xml.getAttribute("synthesizer", "ip", "192.168.0.23", 0);
			synthesizerPort = xml.getAttribute("synthesizer", "port", 10000, 0);
			
		xml.popTag();
	}else{
		//message = "unable to load mySettings.xml check data/ folder";
	}
};