#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(30);
	ofEnableSmoothing(); 
	
	ofBackground(0,0,0,255);
	
	

	//Particles
	mouseAttract	= false;
	doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
	doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
	doRender		= true;
	forceTimer		= false;


	rotSpeed		= 0;
	mouseMass		= 1;


	ballImage.loadImage("ball.png");
	
	width = ofGetWidth();
	height = ofGetHeight();
	
	//	physics.verbose = true;			// dump activity to log
	physics.setGravity(ofVec3f(0, GRAVITY, 0));
	
	// set world dimensions, not essential, but speeds up collision
	physics.setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
	physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
	physics.setDrag(1);		// FIXTHIS
	physics.enableCollision();
	
	initScene();
	
	// setup lighting
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0, height/2, 0.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	
	// enable back-face culling (so we can see through the walls)
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}


void testApp::update(){
	//PARTICLE
	pm.update();
	
	
	// Update the magnets
	for (list<Magnet*>::iterator magnet=movingMagnets.begin(); magnet != movingMagnets.end(); magnet++)
	{
		if (ofDist((*magnet)->x, (*magnet)->y, (*magnet)->destiny.x, (*magnet)->destiny.y) <= 5) {
			(*magnet)->destiny.x = ofRandom(1280.0);
			(*magnet)->destiny.y = ofRandom(800);
		}
	}
}



void testApp::draw(){
	pm.draw();
}




void testApp::keyPressed (int key) {
	switch (key) {
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'p':
			pm.verbose = !pm.verbose;
			break;
	}
}

void testApp::mouseDragged(int x, int y, int button) {
	mouseMagnet->destiny.x = 
	mouseMagnet->x = x;
	mouseMagnet->destiny.y = 
	mouseMagnet->y =y;
}
void testApp::mousePressed(int x, int y, int button) {
	mouseMagnet = pm.createMagnet(x, y);
	mouseMagnet->attraction = 0.2;
	mouseMagnet->repel = false;
}
void testApp::mouseReleased(int x, int y, int button) {
	pm.removeMagnet(mouseMagnet);
	delete mouseMagnet;
}



void testApp::initScene() {
	// clear all particles and springs etc
	physics.clear();
	
	// you can add your own particles to the physics system
	physics.addParticle(&mouseNode);
	mouseNode.makeFixed();
	mouseNode.setMass(MIN_MASS);
	mouseNode.moveTo(ofVec3f(0, 0, 0));
	mouseNode.setRadius(NODE_MAX_RADIUS);
	
	// or tell the system to create and add particles
	physics.makeParticle(ofVec3f(-width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top left back and fix
	physics.makeParticle(ofVec3f( width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
	physics.makeParticle(ofVec3f(-width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
	physics.makeParticle(ofVec3f( width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix
}


void testApp::addRandomParticle() {
	float posX		= ofRandom(-width/2, width/2);
	float posY		= ofRandom(0, height);
	float posZ		= ofRandom(-width/2, width/2);
	float mass		= ofRandom(MIN_MASS, MAX_MASS);
	float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
	float radius	= ofMap(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS, NODE_MAX_RADIUS);
	
	// physics.makeParticle returns a particle pointer so you can customize it
	msa::physics::Particle3D *p = physics.makeParticle(ofVec3f(posX, posY, posZ));
	
	// and set a bunch of properties (you don't have to set all of them, there are defaults)
	p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
	
	// add an attraction to the mouseNode
	if(mouseAttract) physics.makeAttraction(&mouseNode, p, ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
}

void testApp::addRandomSpring() {
	msa::physics::Particle3D *a = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
	msa::physics::Particle3D *b = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
	physics.makeSpring(a, b, ofRandom(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), ofRandom(10, width/2));
}


void testApp::killRandomParticle() {
	msa::physics::Particle3D *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
	if(p && p != &mouseNode) p->kill();
}

void testApp::killRandomSpring() {
	msa::physics::Spring3D *s = physics.getSpring( floor(ofRandom(0, physics.numberOfSprings())));
	if(s) s->kill();
}

void testApp::killRandomConstraint() {
	msa::physics::Constraint3D *c = physics.getConstraint(floor(ofRandom(0, physics.numberOfConstraints())));
	if(c) c->kill();
}


void testApp::toggleMouseAttract() {
	mouseAttract = !mouseAttract;
	if(mouseAttract) {
		// loop through all particles and add attraction to mouse
		// (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
		for(int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(&mouseNode, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
	} else {
		// loop through all existing attractsions and delete them
		for(int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
	}
}

void testApp::addRandomForce(float f) {
	forceTimer = f;
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		if(p->isFree()) p->addVelocity(ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
	}
}

void testApp::lockRandomParticles() {
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		if(ofRandom(0, 100) < FIX_PROBABILITY) p->makeFixed();
		else p->makeFree();
	}
	mouseNode.makeFixed();
}

void testApp::unlockRandomParticles() {
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		p->makeFree();
	}
	mouseNode.makeFixed();
}