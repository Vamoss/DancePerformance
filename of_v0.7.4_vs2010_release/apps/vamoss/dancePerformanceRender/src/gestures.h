#pragma once

#include "ofxKinectNui.h"
#include "MSAPhysics3D.h"
#include "ofMain.h"
#include "server.h"
#include "NanoSoftSensor.h"

struct gesture {
  float value;
  float lastValue;
  float min;
  float max;
  string name;
  bool enabled;
  bool inverted;
  NanoSoftSensor stabilizer;
};

class gestures {
public:

	void setup()
	{
		headX = new gesture();
		headX->name = "/headX";
		list.push_back(headX);
		
		headY = new gesture();
		headY->name = "/headY";
		list.push_back(headY);
		
		headZ = new gesture();
		headZ->name = "/headZ";
		list.push_back(headZ);
		
		handDist = new gesture();
		handDist->name = "/handDist";
		list.push_back(handDist);
		
		handLeftY = new gesture();
		handLeftY->name = "/handLeftY";
		list.push_back(handLeftY);
		
		handLeftVel = new gesture();
		handLeftVel->name = "/handLeftVel";
		list.push_back(handLeftVel);
		
		handRightY = new gesture();
		handRightY->name = "/handRightY";
		list.push_back(handRightY);
		
		handRightVel = new gesture();
		handRightVel->name = "/handRightVel";
		list.push_back(handRightVel);
		
		footDist = new gesture();
		footDist->name = "/footDist";
		list.push_back(footDist);
		
		footLeftY = new gesture();
		footLeftY->name = "/footLeftY";
		list.push_back(footLeftY);
		
		footRightY = new gesture();
		footRightY->name = "/footRightY";
		list.push_back(footRightY);

		for(int i=0; i<list.size(); i++) {
			list[i]->min = 0.1;
			list[i]->max = 1;
			list[i]->value = 0.5;
			list[i]->lastValue = 0.5;
			list[i]->enabled = false;
			list[i]->inverted = false;
		}
	}


	void updateGestures(vector<msa::physics::Particle3D *> bone)
	{
		updateValue(headX, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().x);
		updateValue(headY, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().y);
		updateValue(headZ, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().z);
		updateValue(handDist, ofDist(bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y));
		updateValue(handLeftY, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y);
		updateValue(handRightY, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y);
		updateValue(footDist, ofDist(bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().y));
		updateValue(footLeftY, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().y);
		updateValue(footRightY, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().y);
		
		updateVelocity(handLeftVel, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y/100);
		updateVelocity(handRightVel, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y/100);
		
		for(int i=0; i<list.size(); i++) {
			if(list[i]->enabled) {
				server::send(list[i]->name, list[i]->value);
			}
		}
	}
	
	gesture * headX;
	gesture * headY;
	gesture * headZ;
	gesture * handDist;
	gesture * handLeftY;
	gesture * handLeftVel;
	gesture * handRightY;
	gesture * handRightVel;
	gesture * footDist;
	gesture * footLeftY;
	gesture * footRightY;

private:
	void updateValue(gesture * gest, float value)
	{
		gest->value = ofMap(value, gest->min, gest->max, 0.0f, 1.0f, true);
		if(gest->inverted) gest->value = 1 - gest->value;
		gest->lastValue = value;
	}

	void updateVelocity(gesture * gest, float value)
	{
		gest->value = ofMap(abs(value - gest->lastValue), gest->min, gest->max, 0.0f, 1.0f, true);
		if(gest->inverted) gest->value = 1 - gest->value;
		gest->lastValue = value;
	}

	vector<gesture *> list;
};