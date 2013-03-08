/*
 *  Particle.h
 *  PortoHoje
 *
 *  Created by Carlos on 22/05/12.
 *  Copyright 2012 SuperUber. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "Magnet.h"


class Particle {
public:
	
	float size;
	
	ofPoint destiny;
	float x;
	float y;
	
	float vx;
	float vy;
	
	float accel;
	float destAccel;
	
	float orbit;
	float destOrbit;
	
	float counter;
	
	Magnet *magnet;
	bool hasMagnet;
	
	Particle(){
		size = ofRandom(0.5, 2.0);
		
		counter = ofRandom(PI*2);
		vx = 0.0;
		vy = 0.0;
		destAccel = NORMAL_SPEED;
		accel = NORMAL_SPEED;
		orbit = ofRandom(MIN_ORBIT, MAX_ORBIT);
		destOrbit = orbit;
		
		hasMagnet = false;
	}
};

