/******************************************************************/
/**
 * @file	testApp.cpp
 * @brief	Example for ofxKinectNui addon
 * @note
 * @todo
 * @bug	
 *
 * @author	sadmb
 * @date	Oct. 28, 2011
 */
/******************************************************************/
#include "testApp.h"
#include "ofxKinectNuiDraw.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = true;
	initSetting.grabDepth = false;
	initSetting.grabAudio = false;
	initSetting.grabLabel = false;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = false;
	initSetting.grabLabelCv = false;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_320x240;
	kinect.init(initSetting);
	kinect.open();
//	kinect.open(true); // when you want to use near mode (default is false)

	kinect.addKinectListener(this, &testApp::kinectPlugged, &testApp::kinectUnplugged);
	
#ifdef USE_TWO_KINECTS
	// watch out that only the first kinect can grab label and skeleton.
	kinect2.init(true, true, false, false, false, false, false, true);
	kinect2.open();
#endif
	ofSetVerticalSync(true);

	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();

	ofSetFrameRate(60);
	
	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo(kinect.getVideoResolution());
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton();
	kinect.setVideoDrawer(videoDraw_);
	kinect.setSkeletonDrawer(skeletonDraw_);



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

	
	minZ = 0;
	maxZ = 0;
}

//--------------------------------------------------------------
void testApp::update() {
	kinectSource->update();
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif

	//PARTICLE
	width = ofGetWidth();
	height = ofGetHeight();
	
	physics.update();
	
	

	// Update kinect coords
	float scale = 3;
	for(int i = 0; i < 6; ++i){
		for(int j = 0; j < 20; ++j){
			if(kinect.skeletonPoints[i][0].z > 0){
				currentSkeletonIndex = i;
				//if(kinect.skeletonPoints[i][0].x<minZ) minZ = kinect.skeletonPoints[i][0].x;
				//if(kinect.skeletonPoints[i][0].x>maxZ) maxZ = kinect.skeletonPoints[i][0].x;
				//cout << minZ << " " << maxZ << endl;
				float x = ofMap(kinect.skeletonPoints[i][j].x, 0, 310, -width/2, width/2);
				//float y = ofMap(kinect.skeletonPoints[i][j].y, 0, 40000, width/2, -width/2);
				float z = ofMap(kinect.skeletonPoints[i][j].z, 0, 40000, width/2, -width/2);
				bone[(i*20) + j]->moveTo(ofVec3f(x, (kinect.skeletonPoints[i][j].y-ofGetMouseY()) * scale, z));
			}else{
				bone[(i*20) + j]->moveTo(ofVec3f(9999999, 9999999, 9999999));
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(100, 100, 100);
	ofSetColor(255);

	//Particles
	if(doRender) {
		
		ofEnableAlphaBlending();
		glEnable(GL_DEPTH_TEST);
		
		glPushMatrix();
		
		glTranslatef(width/2, 0, -width / 3);		// center scene
		static float rot = 0;
		glRotatef(rot, 0, 1, 0);			// rotate scene
		rot += rotSpeed;						// slowly increase rotation (to get a good 3D view)
		
		if(forceTimer) {
			float translateMax = forceTimer;
			glTranslatef(ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax));
			forceTimer--;
		}
		
		
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		// draw right wall
		glColor3f(.1, 0.1, 0.1);		glVertex3f(width/2, height+1, width/2);
		glColor3f(0, 0, 0);				glVertex3f(width/2, -height, width/2);
		glColor3f(0.05, 0.05, 0.05);	glVertex3f(width/2, -height, -width/2);
		glColor3f(.15, 0.15, 0.15);		glVertex3f(width/2, height+1, -width/2);
		
		// back wall
		glColor3f(.1, 0.1, 0.1);		glVertex3f(width/2, height+1, -width/2);
		glColor3f(0, 0, 0);				glVertex3f(width/2, -height, -width/2);
		glColor3f(0.05, 0.05, 0.05);	glVertex3f(-width/2, -height, -width/2);
		glColor3f(.15, 0.15, 0.15);		glVertex3f(-width/2, height+1, -width/2);
		
		// left wall
		glColor3f(.1, 0.1, 0.1);		glVertex3f(-width/2, height+1, -width/2);
		glColor3f(0, 0, 0);				glVertex3f(-width/2, -height, -width/2);
		glColor3f(0.05, 0.05, 0.05);	glVertex3f(-width/2, -height, width/2);
		glColor3f(.15, 0.15, 0.15);		glVertex3f(-width/2, height+1, width/2);
		
		// front wall
		glColor3f(0.05, 0.05, 0.05);	glVertex3f(width/2, -height, width/2);
		glColor3f(.15, 0.15, 0.15);		glVertex3f(width/2, height+1, width/2);
		glColor3f(.1, 0.1, 0.1);		glVertex3f(-width/2, height+1, width/2);
		glColor3f(0, 0, 0);				glVertex3f(-width/2, -height, width/2);
		
		// floor
		glColor3f(.2, 0.2, 0.2);
		glVertex3f(width/2, height+1, width/2);
		glVertex3f(width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, width/2);
		glEnd();
		
		// glEnable(GL_LIGHTING);
		
		//particles
		glAlphaFunc(GL_GREATER, 0.5);
		
		//ofEnableNormalizedTexCoords();
		//ballImage.getTextureReference().bind();
		for(int i=0; i<physics.numberOfParticles(); i++) {
			msa::physics::Particle3D *p = physics.getParticle(i);
			if(p->isFixed()) glColor4f(1, 0, 0, 1);
			else glColor4f(1, 1, 1, 1);

			glEnable(GL_ALPHA_TEST);
			
			// draw ball
			glPushMatrix();
			glTranslatef(p->getPosition().x, p->getPosition().y, p->getPosition().z);
			glRotatef(180-rot, 0, 1, 0);

			ofSphere(p->getRadius());
			/*glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
			glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());
			glEnd();
			glPopMatrix();*/
			
			glDisable(GL_ALPHA_TEST);
			
			float alpha = ofMap(p->getPosition().y, -height * 1.5, height, 0, 1);
			if(alpha>0) {
				glPushMatrix();
				glTranslatef(p->getPosition().x, height, p->getPosition().z);
				glRotatef(-90, 1, 0, 0);
				glColor4f(255, 255, 255, alpha * alpha * alpha * alpha);
				float r = p->getRadius() * alpha;
				ofCircle(0, 0, 0, p->getRadius());
				/*glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(-r, -r);
				glTexCoord2f(1, 0); glVertex2f(r, -r);
				glTexCoord2f(1, 1); glVertex2f(r, r);
				glTexCoord2f(0, 1); glVertex2f(-r, r);
				glEnd();*/
				glPopMatrix();
			}
			
		}
		//ballImage.getTextureReference().unbind();
		//ofDisableNormalizedTexCoords();
		
		
		glPopMatrix();
	}
	

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(0, 0, 0, 1);
	ofDrawBitmapString(" FPS: " + ofToString(ofGetFrameRate(), 2)
                + " | Number of particles: " + ofToString(physics.numberOfParticles(), 2)
                + " | Number of springs: " + ofToString(physics.numberOfSprings(), 2)
                + " | Mouse Mass: " + ofToString(mouseNode.getMass(), 2)
                + "\nLook at source code keyPressed to see keyboard shortcuts"
			   , 20, 15);
}


//--------------------------------------------------------------
void testApp::exit() {
	if(videoDraw_) {
		videoDraw_->destroy();
		videoDraw_ = NULL;
	}
	if(skeletonDraw_) {
		delete skeletonDraw_;
		skeletonDraw_ = NULL;
	}

	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);


#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	/*switch(key){
	case 'v': // draw video only
	case 'V':
		bDrawVideo = !bDrawVideo;
		if(bDrawVideo){
			bDrawCalibratedTexture = false;
			bDrawSkeleton = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'd': // draw depth + users label only
	case 'D':
		bDrawDepthLabel = !bDrawDepthLabel;
		if(bDrawDepthLabel){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawSkeleton = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 's': // draw skeleton only
	case 'S':
		bDrawSkeleton = !bDrawSkeleton;
		if(bDrawSkeleton){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'q': // draw point cloud example
	case 'Q':
		bDrawCalibratedTexture = !bDrawCalibratedTexture;
		if(bDrawCalibratedTexture){
			bDrawVideo = false;
			bDrawDepthLabel = false;
			bDrawSkeleton = false;
			glEnable(GL_DEPTH_TEST);
		}
		break;
	case 'o': // open stream
	case 'O':
		kinect.open();
		break;
	case 'c': // close stream
	case 'C':
		kinect.close();
		break;
	case 'r': // record stream
	case 'R':
		if(!bRecord){
			startRecording();
		}else{
			stopRecording();
		}
		break;
	case 'p': // playback recorded stream
	case 'P':
		if(!bPlayback){
			startPlayback();
		}else{
			stopPlayback();
		}
		break;
	case OF_KEY_UP: // up the kinect angle
		angle++;
		if(angle > 27){
			angle = 27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_DOWN: // down the kinect angle
		angle--;
		if(angle < -27){
			angle = -27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_LEFT: // increase the far clipping distance
		if(farClipping > nearClipping + 10){
			farClipping -= 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case OF_KEY_RIGHT: // decrease the far clipping distance
		if(farClipping < 4000){
			farClipping += 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case '+': // increase the near clipping distance
		if(nearClipping < farClipping - 10){
			nearClipping += 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	case '-': // decrease the near clipping distance
		if(nearClipping >= 10){
			nearClipping -= 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	}*/


	//particles
	switch(key) {
		case 'a': 
			toggleMouseAttract();
			break;
		case 'p': 
			for(int i=0; i<100; i++) addRandomParticle(); 
			break;
		case 'P': 
			for(int i=0; i<100; i++) 
				killRandomParticle(); break;
		case 's': 
			addRandomSpring(); 
			break;
		case 'S': 
			killRandomSpring(); 
			break;
		case 'c': 
			physics.isCollisionEnabled() ? physics.disableCollision() : physics.enableCollision(); 
			break;
		case 'C': 
			killRandomConstraint(); 
			break;
		case 'r': 
			doRender ^= true; 
			break;
		case 'f': 
			addRandomForce(FORCE_AMOUNT); break;
		case 'F': 
			addRandomForce(FORCE_AMOUNT * 3); 
			break;
		case 'l': 
			lockRandomParticles(); 
			break;
		case 'u': 
			unlockRandomParticles(); 
			break;
		case ' ': 
			initScene(); 
			break;
		case 'x': 
			doMouseXY = true; 
			break;
		case 'z': 
			doMouseYZ = true; 
			break;
		case ']': 
			rotSpeed += 0.01f; 
			break;
		case '[': 
			rotSpeed -= 0.01f; 
			break;
		case '+': 
			mouseNode.setMass(mouseNode.getMass() +0.1); 
			for(int i = 0; i < 6; ++i){
				for(int j = 0; j < 20; ++j){
					bone[(i*20) + j]->setMass(bone[(i*20) + j]->getMass() +0.1);
				}
			}
			break;
		case '-': 
			mouseNode.setMass(mouseNode.getMass() -0.1); 
			for(int i = 0; i < 6; ++i){
				for(int j = 0; j < 20; ++j){
					bone[(i*20) + j]->setMass(bone[(i*20) + j]->getMass() -0.1);
				}
			}
			break;
		case 'm': 
			bool collision = mouseNode.hasCollision();
			if(collision){
				mouseNode.disableCollision();
				for(int i = 0; i < 6; ++i){
					for(int j = 0; j < 20; ++j){
						bone[(i*20) + j]->disableCollision();
					}
				}
			}else{
				mouseNode.enableCollision();
				for(int i = 0; i < 6; ++i){
					for(int j = 0; j < 20; ++j){
						bone[(i*20) + j]->enableCollision();
					}
				}
			}
			break;
	}
}

void testApp::keyReleased  (int key){
	//particles
	switch(key) {
		case 'x': doMouseXY = false; break;
		case 'z': doMouseYZ = false; break;
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	mRotationY = (x - 512) / 5;
	mRotationX = (384 - y) / 5;

	//particles
	static int oldMouseX = -10000;
	static int oldMouseY = -10000;
	int velX = x - oldMouseX;
	int velY = y - oldMouseY;
	if(doMouseXY) mouseNode.moveBy(ofVec3f(velX, velY, 0));
	if(doMouseYZ) mouseNode.moveBy(ofVec3f(velX, 0, velY));
	oldMouseX = x;
	oldMouseY = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	//particles
	switch(button) {
		case 0:	doMouseXY = true; mouseMoved(x, y); break;
		case 2: doMouseYZ = true; mouseMoved(x, y); break;
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	//particles
	doMouseXY = doMouseYZ = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::kinectPlugged(){
	bPlugged = true;
}

//--------------------------------------------------------------
void testApp::kinectUnplugged(){
	bPlugged = false;
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


	for(int i = 0; i < 6; ++i){
		for(int j = 0; j < 20; ++j){
			msa::physics::Particle3D * joint = new msa::physics::Particle3D();
			physics.addParticle(joint);
			joint->makeFixed();
			joint->setMass(MIN_MASS);
			joint->moveTo(ofVec3f(0, 0, 0));
			joint->setRadius(NODE_MAX_RADIUS);

			bone.push_back(joint);
		}
	}
	
	
	
	// or tell the system to create and add particles
	/*physics.makeParticle(ofVec3f(-width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top left back and fix
	physics.makeParticle(ofVec3f( width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
	physics.makeParticle(ofVec3f(-width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
	physics.makeParticle(ofVec3f( width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix*/
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
		// loop through all particles and add attraction to the closest bone
		/*msa::physics::Particle3D * closestBone;
		float minDistance;
		float tempDistance;
		for(int i=0; i<physics.numberOfParticles(); i++) {
			closestBone = NULL;
			minDistance = 999999999;
			for(int j = 0; j < 6; ++j){
				for(int k = 0; k < 20; ++k){
					tempDistance = bone[(j*20) + k]->getPosition().distance(physics.getParticle(i)->getPosition());
					if(tempDistance<minDistance){
						closestBone = bone[(j*20) + k];
						minDistance = tempDistance;
					}
				}
			}
			if(closestBone!=NULL){
				physics.makeAttraction(closestBone, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
			}
		}*/

		int k = 0;
		for(int i=0; i<physics.numberOfParticles(); i++) {
			physics.makeAttraction(bone[(i%20) + (20*currentSkeletonIndex)], physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
			k++;
		}
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

