#pragma once

#include "ofMain.h"
#include "facade.h"
#include "ofxOsc.h"

class receiver {
public:
	

	facade			*app;

	ofxOscReceiver oscReceiver;
	int port;

	receiver()
	{

	}

	void setup(facade *app, int port)
	{
		 this->app = app;
		this->port = port;

		oscReceiver.setup(port);
	}


	void update()
	{
		while(oscReceiver.hasWaitingMessages()){
			ofxOscMessage m;
			oscReceiver.getNextMessage(&m);
			onData(m);
		}
	}
	
	
	void onData(ofxOscMessage m)
	{
		string name = m.getAddress();
		cout << "onData: " << name << endl;
		
		if(name == "RENDER")
		{
			app->doRender = m.getArgAsInt32(0);; 
		}
		else if(name == "PARTICLES")
		{
			float total =  m.getArgAsFloat(0);
			for(int i=app->physics.numberOfParticles(); i < total; i++) app->addRandomParticle();
			for(int i=app->physics.numberOfParticles(); i > total; i--) app->killRandomParticle();
		}
		else if(name == "PROBABILITY")
		{
			app->probability = m.getArgAsFloat(0);
		}
		else if(name == "SPRING")
		{
			app->setMouseSpring(m.getArgAsInt32(0));
		}
		else if(name == "ATTRACT")
		{
			app->setMouseAttract(m.getArgAsInt32(0));
		}
		else if(name == "STRENGHT")
		{ 
			app->setStrength(m.getArgAsFloat(0), m.getArgAsFloat(1));
		}
		else if(name == "ORBIT")
		{
			app->setOrbit(m.getArgAsFloat(0), m.getArgAsFloat(1));
		}
		else if(name == "SHAKE")
		{
			app->addRandomForce(FORCE_AMOUNT);
		}
		else if(name == "RESTART")
		{
			app->initScene(); 
		}
		else if(name == "FADE")
		{
			cout << m.getArgAsFloat(0) << endl;
			app->canvasFade =  m.getArgAsFloat(0);
		}
		else if(name == "BLACKOUT")
		{
			app->blackout =  m.getArgAsFloat(0);
		}
		else if(name == "COLLISION")
		{
			app->setCollision( m.getArgAsInt32(0));
		}
		else if(name == "ROTATION SPEED")
		{
			app->rotSpeed = m.getArgAsFloat(0);
		}
		else if(name == "CENTER")
		{
			app->rotSpeed = m.getArgAsFloat(0);
		}
		else if(name == "MASS")
		{
			app->setMass(m.getArgAsFloat(0), m.getArgAsFloat(1)); 
		}
		else if(name == "GRAVITY")
		{
			app->setGravity(m.getArgAsFloat(0)); 
		}
		else if(name == "COLOR")
		{
			app->particleColor.r = m.getArgAsFloat(0);
			app->particleColor.g = m.getArgAsFloat(1);
			app->particleColor.b = m.getArgAsFloat(2);
		}
		else if(name == "SPACE VARIATION")
		{
			app->colorSpaceVariation = m.getArgAsFloat(0);
		}
		else if(name == "SCALE")
		{
			app->scale = m.getArgAsFloat(0);
		}
	}
};