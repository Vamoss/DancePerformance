#pragma once


#include "ofxKinectNui.h"
#include "MSAPhysics3D.h"

#define HEAD



//Particles

#define	SPRING_MIN_STRENGTH		0.07
#define SPRING_MAX_STRENGTH		0.5

#define	SPRING_MIN_WIDTH		10
#define SPRING_MAX_WIDTH		30

#define NODE_MIN_RADIUS			3
#define NODE_MAX_RADIUS			6

#define MIN_MASS				1
#define MAX_MASS				3

#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.4

#define	FIX_PROBABILITY			10		// % probability of a particle being fixed on creation

#define FORCE_AMOUNT			10

#define	GRAVITY					0

#define MAX_ATTRACTION			2
#define MIN_ATTRACTION			1

#define SECTOR_COUNT			1		// currently there is a bug at sector borders

class facade
{
public:
	facade(void);

	void setup();
	void update();
	void draw();
		

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	//Kinect
	void kinectPlugged();
	void kinectUnplugged();
		
	ofxKinectNui kinect;
	ofxBase3DVideo* kinectSource;

	bool bPlugged;
	bool bUnplugged;
		
	unsigned short nearClipping;
	unsigned short farClipping;
	int angle;



		
	//PARTICLES
	void initScene();
	void addRandomParticle();
	void killRandomParticle();
	void killRandomConstraint();
	void toggleMouseAttract();
	void toggleMouseSpring();
	void addRandomForce(float f);
	void lockRandomParticles();
	void unlockRandomParticles();
	void drawParticle(float r);
	
	bool				mouseAttract;
	bool				mouseSpring;
	bool				doMouseXY;		// pressing left mmouse button moves mouse in XY plane
	bool				doMouseYZ;		// pressing right mouse button moves mouse in YZ plane
	bool				doRender;
	int					forceTimer;

	float				rotSpeed;
	float				mouseMass;

	int			width;
	int			height;

	msa::physics::World3D		physics;
	msa::physics::Particle3D	mouseNode;

	ofImage					ballImage;


	//integration kinect + physics
	vector<msa::physics::Particle3D *> bone;
	int currentSkeletonIndex;

	float minZ;
	float maxZ;
		
	ofFbo canvas;
	ofFbo canvasTrace;
};

