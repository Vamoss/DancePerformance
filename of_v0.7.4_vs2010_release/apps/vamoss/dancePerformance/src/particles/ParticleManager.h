#pragma once


#define		NORMAL_SPEED		2000
#define		ORBIT_SPEED			2000
#define		MIN_ORBIT			30
#define		MAX_ORBIT			90


#include "ofMain.h"
#include "Magnet.h"
#include "Particle.h"

class ParticleManager {
public:
	
	
	int MAGNETS_AT_START;
	int PARTICLES_TOTAL;
	float MAGNETS_REPEL;//Minimal distances between magnets
	float DECAY;//velocity
	float BORDER;
	
	list<Particle*> particles;
	list<Magnet*> magnets;
	
	bool verbose;
	
	
	
	void setup() {
		MAGNETS_AT_START = 20;
		PARTICLES_TOTAL = 12000;
		MAGNETS_REPEL = 100.0;
		DECAY = 0.98;
		BORDER = -30;
		
		//createMagnets();
		createParticles();
		
		verbose = true;
	}
	
	Magnet* createMagnet(float _x, float _y) {
		Magnet *m;
		m = new Magnet();
		m->destiny.x = 
		m->x = _x;
		m->destiny.y = 
		m->y = _y;
		
		magnets.push_back(m);
		
		return m;
	}
	
	void createMagnets() {
		float _x;
		float _y;
		
		for (int i = 0; i < MAGNETS_AT_START; i++) {
			_x = ofRandom(ofGetWidth());
			_y = ofRandom(ofGetHeight());
			
			createMagnet(_x, _y);
		}
	}
	
	void createParticles() {
		for (int i = 0; i < PARTICLES_TOTAL; i++) {
			Particle *p;
			p = new Particle();
			p->destiny.x = 
			p->x = ofRandom(ofGetWidth());
			p->destiny.y = 
			p->y = ofRandom(ofGetHeight());
			
			particles.push_back(p);
		}
	}
	
	void update() {
		/*
		 * Update the magnets
		 */
		/*for (list<Magnet*>::iterator magnet=magnets.begin(); magnet != magnets.end(); magnet++)
		{
			//ease moviment
			(*magnet)->x += ((*magnet)->destiny.x - (*magnet)->x) * 0.009;
			(*magnet)->y += ((*magnet)->destiny.y - (*magnet)->y) * 0.009;
			
			//Check if the magnet is near of another magnet and repel
			if((*magnet)->repel) {
				for (list<Magnet*>::iterator otherMagnet=magnet; otherMagnet != magnets.end(); otherMagnet++)
				{
					if((*magnet) != (*otherMagnet)){
						if(ofDist((*magnet)->x, (*magnet)->y, (*otherMagnet)->x, (*otherMagnet)->y)<MAGNETS_REPEL){
							float angle = -atan2(((*magnet)->y-(*otherMagnet)->y),((*magnet)->x-(*otherMagnet)->x)) + PI/2;
							
							(*magnet)->x = sin(angle) * MAGNETS_REPEL + (*otherMagnet)->x;
							(*magnet)->y = cos(angle) * MAGNETS_REPEL + (*otherMagnet)->y;
						}
					}
				}
			}
		}*/
		
		/*
		 * Update the particles
		 */
		float currentDistance;
		float closestDistance;
		Magnet *closestMagnet;
		bool founded;
		
		for (list<Particle*>::iterator particle=particles.begin(); particle != particles.end(); particle++)
		{
			
			
			// updateCollision
			if ( (*particle)->x < BORDER &&  (*particle)->vx < 0) {
				(*particle)->vx = - (*particle)->vx;
			}
			
			if ( (*particle)->x > ofGetWidth()-BORDER &&  (*particle)->vx > 0){
				(*particle)->vx = - (*particle)->vx;
			}
			
			if ( (*particle)->y < BORDER &&  (*particle)->vy < 0){
				(*particle)->vy = - (*particle)->vy;
			}
				
			if ( (*particle)->y > ofGetHeight()-BORDER &&  (*particle)->vy > 0){
				(*particle)->vy = - (*particle)->vy;
			}
			
			
			closestDistance = 100;
			founded = false;
			
			
			
			// For each particle check who is the closesy magnet
			closestMagnet = (*magnets.begin());
			for (list<Magnet*>::iterator magnet=magnets.begin(); magnet != magnets.end(); magnet++)
			{			
				currentDistance = ofDist((*particle)->x, (*particle)->y, (*magnet)->x, (*magnet)->y) - ((*magnet)->orbit * closestMagnet->attraction / 2);
				
				if (currentDistance * (*magnet)->attraction < closestDistance) {
					founded = true;
					closestDistance = currentDistance;
					closestMagnet = (*magnet);
				}
			}
			
			
			
			// update orbit
			if(ofRandom(1)>0.95) {
				(*particle)->destOrbit = ofRandom(MIN_ORBIT, MAX_ORBIT);
			}
			(*particle)->orbit += ( (*particle)->destOrbit -  (*particle)->orbit) * 0.09;
			
			
			if(founded){
				/*// if close to target
				if (!(*particle)->hasMagnet || ofDist((*particle)->x, (*particle)->y, (*particle)->destiny.x, (*particle)->destiny.y) < 50 * closestMagnet->attraction)
				{*/
					(*particle)->destiny.x = closestMagnet->x + cos( (*particle)->counter ) * (*particle)->orbit;
					(*particle)->destiny.y = closestMagnet->y + sin( (*particle)->counter ) * (*particle)->orbit;
					(*particle)->destAccel = ORBIT_SPEED * closestMagnet->attraction;
					(*particle)->magnet = closestMagnet;
					(*particle)->hasMagnet = true;
				/*}
			}else{
				// if close to target
				if ((*particle)->hasMagnet || ofDist((*particle)->x, (*particle)->y, (*particle)->destiny.x, (*particle)->destiny.y) < 50)
				{
					//get random destination
					(*particle)->destiny.x = ofRandom(ofGetWidth());
					(*particle)->destiny.y = ofRandom(ofGetHeight());
				}
				(*particle)->destAccel = NORMAL_SPEED;
				(*particle)->hasMagnet = false;*/
			}
			
			
			//ease acceleration
			(*particle)->accel += ( (*particle)->destAccel -  (*particle)->accel) * 0.09;
			
			//ease velocity
			(*particle)->vx += ( (*particle)->destiny.x -  (*particle)->x) /  (*particle)->accel;
			(*particle)->vy += ( (*particle)->destiny.y -  (*particle)->y) /  (*particle)->accel;
			
			//apply decay
			(*particle)->vx *= DECAY;
			(*particle)->vy *= DECAY;
			
			//update position
			(*particle)->x +=  (*particle)->vx;
			(*particle)->y +=  (*particle)->vy;
			
			
			(*particle)->counter += 0.1;
		}
	}
	
	void draw () {
		ofEnableAlphaBlending();
		
		if(verbose) {
			// Render the magnets
			for (list<Magnet*>::iterator magnet=magnets.begin(); magnet != magnets.end(); magnet++)
			{
				ofSetColor(0,0,255,125);
				ofCircle((*magnet)->x, (*magnet)->y, 8);
			}
		}
		
		// Render the particles
		for (list<Particle*>::iterator particle=particles.begin(); particle != particles.end(); particle++)
		{
			ofSetColor(229,242,204,125);
			ofCircle((*particle)->x, (*particle)->y, (*particle)->size);
			
			//GLOW
			ofSetColor(229,242,204,5);
			ofCircle((*particle)->x, (*particle)->y, (*particle)->size*4);
		}
		
		
		ofDisableAlphaBlending();
	}
	
	void addMagnet(Magnet *magnet) {
		for (list<Magnet*>::iterator i=magnets.begin(); i != magnets.end(); i++)
			if(*i==magnet) break;
		
		magnets.push_back(magnet);
	}
	
	void removeMagnet(Magnet *magnet) {
		magnets.remove(magnet);
	}
	
	
	
};     

