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
		else if(name == "ROTATION")
		{
			app->rotation = m.getArgAsFloat(0);
		}
		else if(name == "MASS")
		{
			app->setMass(m.getArgAsFloat(0), m.getArgAsFloat(1)); 
		}
		else if(name == "GRAVITY")
		{
			app->setGravity(m.getArgAsFloat(0)); 
		}
		else if(name == "BOUNCE")
		{
			app->setBounce(m.getArgAsFloat(0), m.getArgAsFloat(1)); 
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
		else if(name == "DELAY")
		{
			app->delay = 1 - m.getArgAsFloat(0);
		}
		else if(name == "Y")
		{
			app->y = m.getArgAsFloat(0);
		}
		else if(name == "SCALE")
		{
			app->scale = m.getArgAsFloat(0);
		}
		else if(name == "HEAD")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HEAD);
		}
		else if(name == "SHOULDER LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_SHOULDER_LEFT);
		}
		else if(name == "SHOULDER CENTER")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_SHOULDER_CENTER);
		}
		else if(name == "SHOULDER RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_SHOULDER_RIGHT);
		}
		else if(name == "ELBOW LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_ELBOW_LEFT);
		}
		else if(name == "ELBOW RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_ELBOW_RIGHT);
		}
		else if(name == "WRIST LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_WRIST_LEFT);
		}
		else if(name == "WRIST RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_WRIST_RIGHT);
		}
		else if(name == "HAND LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HAND_LEFT);
		}
		else if(name == "HAND RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HAND_RIGHT);
		}
		else if(name == "SPINE")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_SPINE);
		}
		else if(name == "HIP LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HIP_LEFT);
		}
		else if(name == "HIP CENTER")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HIP_CENTER);
		}
		else if(name == "HIP RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_HIP_RIGHT);
		}
		else if(name == "KNEE LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_KNEE_LEFT);
		}
		else if(name == "KNEE RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_KNEE_RIGHT);
		}
		else if(name == "ANKLE LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_ANKLE_LEFT);
		}
		else if(name == "ANKLE RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_ANKLE_RIGHT);
		}
		else if(name == "FOOT LEFT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_FOOT_LEFT);
		}
		else if(name == "FOOT RIGHT")
		{
			toggleJoint(m.getArgAsInt32(0), NUI_SKELETON_POSITION_FOOT_RIGHT);
		}	
		else if(name == "/rightVolume")
		{
			app->particleColor.r = m.getArgAsFloat(0);
		}
		else if(name == "/leftVolume")
		{
			app->particleColor.r = m.getArgAsFloat(0);
		}
		
	}

private:

	void toggleJoint(bool state, int index)
	{
		app->boneEnabled[index] = state;
	}
};