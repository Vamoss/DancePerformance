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

void server::send(string name)
{
	ofxOscMessage m;
	m.setAddress(name);
	sender.sendMessage(m);
}

void server::send(string name, int value1, int value2, int value3)
{
	ofxOscMessage m;
	m.setAddress(name);
	if(value1!=-999999) m.addIntArg(value1);
	if(value2!=-999999) m.addIntArg(value2);
	if(value3!=-999999) m.addIntArg(value3);
	sender.sendMessage(m);
}

void server::send(string name, float value1, float value2, float value3)
{
	if(!config::synthesizerEnabled) return;

	ofxOscMessage m;
	m.setAddress(name);
	if(value1!=-999999) m.addFloatArg(value1);
	if(value2!=-999999) m.addFloatArg(value2);
	if(value3!=-999999) m.addFloatArg(value3);
	sender.sendMessage(m);
}

void server::send(string name, string value1, string value2, string value3)
{
	ofxOscMessage m;
	m.setAddress(name);
	if(value1!="") m.addStringArg(value1);
	if(value2!="") m.addStringArg(value2);
	if(value3!="") m.addStringArg(value3);
	sender.sendMessage(m);
}