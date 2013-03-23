#include "server.h"

ofxOscSender server::sender = ofxOscSender();
string server::ip = "";
int server::port = 0;


void server::setup(string ip, int port)
{
	server::ip = ip;
	server::port = port;

	server::sender.setup(ip, port);
}

void server::send(string msg)
{
	ofxOscMessage m;
	m.setAddress(msg);
	sender.sendMessage(m);
}