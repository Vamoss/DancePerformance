#pragma once

#include "ofxKinectNui.h"
#include "MSAPhysics3D.h"
#include "ofMain.h"

struct gesture {
  float value;
  float min;
  float max;
  string name;
  bool enabled;
};

class gestures {
public:

	void setup()
	{
		headY.name = "HEAD Y";
		list.push_back(headY);

		handDist.name = "HAND DIST";
		list.push_back(handDist);

		handLeftY.name = "HAND LEFT Y";
		list.push_back(handLeftY);

		handRightY.name = "HAND RIGHT Y";
		list.push_back(handRightY);

		footDist.name = "FOOT DIST";
		list.push_back(footDist);

		footLeftY.name = "FOOT LEFT Y";
		list.push_back(footLeftY);

		footRightY.name = "FOOT RIGHT Y";
		list.push_back(footRightY);

		spacePos.name = "SPACE POS";
		list.push_back(spacePos);

		for(int i=0; i<list.size(); i++) {
			list[0].min = 0;
			list[0].max = 1;
			list[0].value = 0.5;
			list[0].enabled = false;
		}
	}


	void updateGestures(vector<msa::physics::Particle3D *> bone)
	{
		updateValue(headY, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().y);
		updateValue(handDist, ofDist(bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y));
		updateValue(handLeftY, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y);
		updateValue(handRightY, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y);
		updateValue(footDist, ofDist(bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().y));
		updateValue(footLeftY, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().y);
		updateValue(footRightY, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().y);
		updateValue(spacePos, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().y);
	}

	float volume;
	gesture headY;
	gesture handDist;
	gesture handLeftY;
	gesture handRightY;
	gesture footDist;
	gesture footLeftY;
	gesture footRightY;
	gesture spacePos;

private:
	void updateValue(gesture gest, float value)
	{
		gest.value = ofMap(value, gest.min, gest.max, 0, 1, true);
	}

	vector<gesture> list;
};