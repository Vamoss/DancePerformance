/*
 *  Magnet.h
 *  PortoHoje
 *
 *  Created by Carlos on 22/05/12.
 *  Copyright 2012 SuperUber. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"


class Magnet {
public:
	float orbit;
	float x;
	float y;
	ofPoint destiny;
	float attraction;
	int id;
	bool repel;
	
	Magnet(){
		orbit = 100;
		attraction = 1.0;
		repel = true;
	}
};