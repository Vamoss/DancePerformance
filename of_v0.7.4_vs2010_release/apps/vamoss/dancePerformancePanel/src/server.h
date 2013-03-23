#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class server {
public:

	static void setup(string ip, int port);
	static void send(string msg);

private:
	static string ip;
	static int port;
	static ofxOscSender sender;
};