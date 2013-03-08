/******************************************************************/
/**
 * @file	testApp.h
 * @brief	Example for ofxKinectNui addon
 * @note
 * @todo
 * @bug	
 *
 * @author	sadmb
 * @date	Oct. 28, 2011
 */
/******************************************************************/
#pragma once

#include "ofxKinectNui.h"
#include "ofMain.h"
#include "ofxKinectNuiPlayer.h"
#include "ofxKinectNuiRecorder.h"
#include "particles/ParticleManager.h"

class ofxKinectNuiDrawTexture;
class ofxKinectNuiDrawSkeleton;

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

#define HEAD




//Particles
#include "MSAPhysics3D.h"

#define	SPRING_MIN_STRENGTH		0.005
#define SPRING_MAX_STRENGTH		0.1

#define	SPRING_MIN_WIDTH		1
#define SPRING_MAX_WIDTH		3

#define NODE_MIN_RADIUS			5
#define NODE_MAX_RADIUS			15

#define MIN_MASS				1
#define MAX_MASS				3

#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.9

#define	FIX_PROBABILITY			10		// % probability of a particle being fixed on creation

#define FORCE_AMOUNT			10

#define EDGE_DRAG				0.98

#define	GRAVITY					1

#define MAX_ATTRACTION			10
#define MIN_ATTRACTION			3

#define SECTOR_COUNT			1		// currently there is a bug at sector borders


class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		/**
		 * @brief	example for adjusting video images to depth images
		 * @note	inspired by akira's video http://vimeo.com/17146552
		 */
		void drawCalibratedTexture();
		void exit();
		void drawCircle3f(int n, int radius, ofVec3f cur);
		

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void kinectPlugged();
		void kinectUnplugged();
		
		void startRecording();
		void stopRecording();
		void startPlayback();
		void stopPlayback();

		ofxKinectNui kinect;

#ifdef USE_TWO_KINECTS
		ofxKinectNui kinect2;
#endif
		ofxKinectNuiPlayer kinectPlayer;
		ofxKinectNuiRecorder kinectRecorder;

		ofxBase3DVideo* kinectSource;

		ofTexture calibratedTexture;

		bool bRecord;
		bool bPlayback;
		bool bDrawVideo;
		bool bDrawDepthLabel;
		bool bDrawSkeleton;
		bool bDrawCalibratedTexture;
		bool bPlugged;
		bool bUnplugged;
		
		unsigned short nearClipping;
		unsigned short farClipping;
		int angle;
		
		int mRotationX, mRotationY;

		// Please declare these texture pointer and initialize when you want to draw them
		ofxKinectNuiDrawTexture*	videoDraw_;
		ofxKinectNuiDrawTexture*	depthDraw_;
		ofxKinectNuiDrawTexture*	labelDraw_;
		ofxKinectNuiDrawSkeleton*	skeletonDraw_;
		
		//PARTICLES
		void initScene();
		void addRandomParticle();
		void addRandomSpring();
		void killRandomParticle();
		void killRandomSpring();
		void killRandomConstraint();
		void toggleMouseAttract();
		void addRandomForce(float f);
		void lockRandomParticles();
		void unlockRandomParticles();



		bool				mouseAttract;
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
};
