#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "config.h"

class server {
public:

	static void setup(string ip, int port);
	
	static void send(string name);
	static void send(string name, int value1, int value2=-999999, int value3=-999999);
	static void send(string name, float value1, float value2=-999999, float value3=-999999);
	static void send(string name, string value1, string value2="", string value3="");

private:
	static string ip;
	static int port;
	static ofxOscSender sender;
};