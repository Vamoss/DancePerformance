#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define TOTAL_PLAYERS 4
#define WIDTH 2304
#define HEIGHT 1024


class config
{
public:
	static void setup(string xmlPath);

	static ofRectangle window;
	static bool fullscreen;

	static bool synthesizerEnabled;
	static string synthesizerIp;
	static int synthesizerPort;
	
	static bool useLog;
};
