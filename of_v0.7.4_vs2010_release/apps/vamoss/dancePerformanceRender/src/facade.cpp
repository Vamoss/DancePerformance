#include "facade.h"


facade::facade(void)
{
#ifdef USE_KINECT
	//Kinect
	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = false;
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
	kinect.addKinectListener(this, &facade::kinectPlugged, &facade::kinectUnplugged);
	
	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	currentSkeletonIndex = -1;
#endif	

	//debug
	showFPS = false;

	//Particles
	mouseAttract	= false;
	mouseSpring		= false;
	doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
	doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
	doRender		= true;
	forceTimer		= false;


	rotation		= 0;
	mouseMass		= 1;

	gravity			= 0;
	
	min_mass		= 1;
	max_mass		= 3;
	
	min_bounce		= 0.2;
	max_bounce		= 0.4;
	
	min_strength = 0.07;
	max_strength = 0.5;

	min_width = 10;
	max_width = 30;

	scale = 3;
	y = 0;

	delay = 1;

	probability = 1;

	ballImage.loadImage("ball.png");
	
	width = ofGetWidth();
	height = ofGetHeight();
	
	//	physics.verbose = true;			// dump activity to log
	physics.setGravity(ofVec3f(0, gravity, 0));
	
	// set world dimensions, not essential, but speeds up collision
	physics.setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
	physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
	physics.setDrag(1);		// FIXTHIS
	//physics.enableCollision();
	
	initScene();
	
	//integration kinect + physics
	minZ = 0;
	maxZ = 0;
	
	initCanvas();
	blackout = 0;
	canvasFade = 125;

	//gestures
	gestureControl.setup();
}

void facade::update()
{

#ifdef USE_KINECT
	//kinect
	kinectSource->update();

	// Update kinect coords
	for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++i){
		if(kinect.skeletonPoints[i][0].z > 0){
			currentSkeletonIndex = i;
			//if(kinect.skeletonPoints[i][0].x<minZ) minZ = kinect.skeletonPoints[i][0].x;
			//if(kinect.skeletonPoints[i][0].x>maxZ) maxZ = kinect.skeletonPoints[i][0].x;
			//cout << minZ << " " << maxZ << endl;
			for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
				if(boneEnabled[j]){
					float destX = ofMap(kinect.skeletonPoints[i][j].x, 0, 310, -width/2, width/2) * scale;
					float destY = kinect.skeletonPoints[i][j].y * 3 * scale - ofMap(scale, 1, 2, -50, 800) + y;
					float destZ = ofMap(kinect.skeletonPoints[i][j].z, 0, 40000, width/2, -width/2);
					
					//delay
					destX += (destX - bone[j]->getPosition().x) * delay;
					destY += (destY - bone[j]->getPosition().y) * delay;
					destZ += (destZ - bone[j]->getPosition().z) * delay;

					bone[j]->moveTo(ofVec3f(destX, destY, destZ));
				}else{
					bone[j]->moveTo(ofVec3f(9999999, -9999999, 9999999));
				}
			}
		}
	}

	if(currentSkeletonIndex==-1){
		for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
			bone[j]->moveTo(ofVec3f(9999999, -9999999, 9999999));
		}
	}else{
		//gestures
		gestureControl.updateGestures(bone);
	}
#endif
	
	physics.update();
}

void facade::draw()
{
	ofBackground(0, 0, 0);
	ofSetColor(255);

	//Particles
	if(doRender) {
		
		ofEnableAlphaBlending();
		
		// center scene
		glPushMatrix();
		glTranslatef(width/2, 0, -width / 3);
		glRotatef(rotation, 0, 1, 0);
		
		if(forceTimer) {
			float translateMax = forceTimer;
			glTranslatef(ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax), ofRandom(-translateMax, translateMax));
			forceTimer--;
		}

		
		/*
		// enable back-face culling (so we can see through the walls)
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);

		ofFill();
		
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
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		*/

		//canvas
		glRotatef(rotation, 0, -1, 0);
		if(config::useCanvas) {
			canvas.begin();

			ofSetColor(255, canvasFade);
			canvasTrace.draw(0,0);
			glTranslatef(width/2, 0, -width / 3);
		}
		glRotatef(rotation, 0, 1, 0);

		//particles
		//glAlphaFunc(GL_GREATER, 0.5);


		if(currentSkeletonIndex>=0 && false){
			ofPolyline pLine;
			ofPushStyle();
			ofSetColor(255, 0, 0);
			ofNoFill();
			ofSetLineWidth(4);
			// HEAD
			pLine.clear();
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SPINE]->getPosition().x, bone[NUI_SKELETON_POSITION_SPINE]->getPosition().y, bone[NUI_SKELETON_POSITION_SPINE]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HEAD]->getPosition().x, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().y, bone[NUI_SKELETON_POSITION_HEAD]->getPosition().z);
			pLine.draw();
	
			// BODY_LEFT
			pLine.clear();
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SHOULDER_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_SHOULDER_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_SHOULDER_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_ELBOW_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_ELBOW_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_ELBOW_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_WRIST_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_WRIST_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_WRIST_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_HAND_LEFT]->getPosition().z);
			pLine.draw();

			// BODY_RIGHT
			pLine.clear();
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_SHOULDER_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_SHOULDER_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_SHOULDER_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_SHOULDER_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_ELBOW_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_ELBOW_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_ELBOW_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_WRIST_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_WRIST_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_WRIST_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_HAND_RIGHT]->getPosition().z);
			pLine.draw();

			// LEG_LEFT
			pLine.clear();
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HIP_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_HIP_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_HIP_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_KNEE_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_KNEE_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_KNEE_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_ANKLE_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_ANKLE_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_ANKLE_LEFT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().y, bone[NUI_SKELETON_POSITION_FOOT_LEFT]->getPosition().z);
			pLine.draw();

			// LEG_RIGHT
			pLine.clear();
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().x, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().y, bone[NUI_SKELETON_POSITION_HIP_CENTER]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_HIP_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_HIP_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_HIP_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_KNEE_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_KNEE_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_KNEE_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_ANKLE_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_ANKLE_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_ANKLE_RIGHT]->getPosition().z);
			pLine.addVertex(bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().x, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().y, bone[NUI_SKELETON_POSITION_FOOT_RIGHT]->getPosition().z);
			pLine.draw();
			ofPopStyle();
		}

		ofEnableNormalizedTexCoords();
		ballImage.getTextureReference().bind();
		msa::physics::Particle3D *p;
		float alpha;
		int red, green, blue;
		for(int i=0; i<physics.numberOfParticles(); i++) {
			p = physics.getParticle(i);
			if(!p->isFixed()){
				
				red = particleColor.r*colorSpaceVariation + ofMap(p->getPosition().x, -width/2, width/2, 0, 255-(255*colorSpaceVariation));
				green = particleColor.g*colorSpaceVariation + ofMap(p->getPosition().y, -height, height, 0, 255-(255*colorSpaceVariation));
				blue = particleColor.b*colorSpaceVariation + ofMap(p->getPosition().z, -width/2, width/2, 0, 255-(255*colorSpaceVariation));
					
				// draw ball
				glPushMatrix();
					glTranslatef(p->getPosition().x, p->getPosition().y, p->getPosition().z);
					glRotatef(180-rotation, 0, 1, 0);
					ofSetColor(red, green, blue);
					drawParticle(p->getRadius());
				glPopMatrix();
			
				//draw shadow
				alpha = ofMap(p->getPosition().y, -height * 1.5, height, 0, 1);
				if(alpha>0) {
					glPushMatrix();
						glTranslatef(p->getPosition().x, height, p->getPosition().z);
						glRotatef(-90, 1, 0, 0);
						ofSetColor(red, green, blue, alpha * alpha * alpha * alpha * 255);
						drawParticle(p->getRadius() * alpha);
					glPopMatrix();
				}
			}
			
		}
		ballImage.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();

		if(config::useCanvas) {
			canvas.end();
		}
		glPopMatrix();

		
		
		
		if(config::useCanvas) {
			canvasTrace.begin();

				/*glColorMask(FALSE, FALSE, FALSE, TRUE);
				glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glColorMask(TRUE, TRUE, TRUE, TRUE);*/

				ofSetColor(255);
				canvas.draw(0,0);
			canvasTrace.end();
		
			canvasTrace.draw(0,0);
		
			canvas.begin();
				ofClear(0);
			canvas.end();
		}
		
		if(blackout>0){
			ofSetColor(0,0,0,blackout);
			ofRect(0,0,0,width, height);
		}

		ofDisableAlphaBlending();
	}

	


	/*
	// HEAD
	pLine.clear();
	pLine.addVertex(src[NUI_SKELETON_POSITION_HIP_CENTER].x, src[NUI_SKELETON_POSITION_HIP_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_SPINE].x, src[NUI_SKELETON_POSITION_SPINE].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_SHOULDER_CENTER].x, src[NUI_SKELETON_POSITION_SHOULDER_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_HEAD].x, src[NUI_SKELETON_POSITION_HEAD].y);
	pLine.draw();
	
	// BODY_LEFT
	pLine.clear();
	pLine.addVertex(src[NUI_SKELETON_POSITION_SHOULDER_CENTER].x, src[NUI_SKELETON_POSITION_SHOULDER_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_SHOULDER_LEFT].x, src[NUI_SKELETON_POSITION_SHOULDER_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_ELBOW_LEFT].x, src[NUI_SKELETON_POSITION_ELBOW_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_WRIST_LEFT].x, src[NUI_SKELETON_POSITION_WRIST_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_HAND_LEFT].x, src[NUI_SKELETON_POSITION_HAND_LEFT].y);
	pLine.draw();

	// BODY_RIGHT
	pLine.clear();
	pLine.addVertex(src[NUI_SKELETON_POSITION_SHOULDER_CENTER].x, src[NUI_SKELETON_POSITION_SHOULDER_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x, src[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_ELBOW_RIGHT].x, src[NUI_SKELETON_POSITION_ELBOW_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_WRIST_RIGHT].x, src[NUI_SKELETON_POSITION_WRIST_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_HAND_RIGHT].x, src[NUI_SKELETON_POSITION_HAND_RIGHT].y);
	pLine.draw();

	// LEG_LEFT
	pLine.clear();
	pLine.addVertex(src[NUI_SKELETON_POSITION_HIP_CENTER].x, src[NUI_SKELETON_POSITION_HIP_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_HIP_LEFT].x, src[NUI_SKELETON_POSITION_HIP_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_KNEE_LEFT].x, src[NUI_SKELETON_POSITION_KNEE_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_ANKLE_LEFT].x, src[NUI_SKELETON_POSITION_ANKLE_LEFT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_FOOT_LEFT].x, src[NUI_SKELETON_POSITION_FOOT_LEFT].y);
	pLine.draw();

	// LEG_RIGHT
	pLine.clear();
	pLine.addVertex(src[NUI_SKELETON_POSITION_HIP_CENTER].x, src[NUI_SKELETON_POSITION_HIP_CENTER].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_HIP_RIGHT].x, src[NUI_SKELETON_POSITION_HIP_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_KNEE_RIGHT].x, src[NUI_SKELETON_POSITION_KNEE_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_ANKLE_RIGHT].x, src[NUI_SKELETON_POSITION_ANKLE_RIGHT].y);
	pLine.addVertex(src[NUI_SKELETON_POSITION_FOOT_RIGHT].x, src[NUI_SKELETON_POSITION_FOOT_RIGHT].y);
	pLine.draw();*/

	

	//glDisable(GL_BLEND);

	
	if(showFPS)
	{
		glColor4f(1, 1, 1, 1);
		ofDrawBitmapString(" FPS: " + ofToString(ofGetFrameRate(), 2)
					+ " | particles: " + ofToString(physics.numberOfParticles(), 2)
					+ " | springs: " + ofToString(physics.numberOfSprings(), 2)
				   , 20, 15);
	}
}


void facade::drawParticle(float r)
{	
	//ofCircle(0,0,r);
			
	//ofSphere(r);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-r, -r);
	glTexCoord2f(1, 0); glVertex2f(r, -r);
	glTexCoord2f(1, 1); glVertex2f(r, r);
	glTexCoord2f(0, 1); glVertex2f(-r, r);
	glEnd();
}


//--------------------------------------------------------------
void facade::keyPressed (int key) {
	if(key==' '){
		showFPS = !showFPS;
	}
}

void facade::keyReleased  (int key){
	//particles
	switch(key) {
		case 'x': doMouseXY = false; break;
		case 'z': doMouseYZ = false; break;
	}
	
}

//--------------------------------------------------------------
void facade::mouseMoved(int x, int y) {
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
void facade::mouseDragged(int x, int y, int button){

	//particles
	switch(button) {
		case 0:	doMouseXY = true; mouseMoved(x, y); break;
		case 2: doMouseYZ = true; mouseMoved(x, y); break;
	}
}

//--------------------------------------------------------------
void facade::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void facade::mouseReleased(int x, int y, int button){
	//particles
	doMouseXY = doMouseYZ = false;
}

//--------------------------------------------------------------
void facade::windowResized(int w, int h){
	width = w;
	height = h;
	
	physics.setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));

	initCanvas();
}

//--------------------------------------------------------------
void facade::kinectPlugged(){
	bPlugged = true;
}

//--------------------------------------------------------------
void facade::kinectUnplugged(){
	bPlugged = false;
}


void facade::initScene() {
	// clear all particles and springs etc
	physics.clear();
	
	// you can add your own particles to the physics system
	physics.addParticle(&mouseNode);
	mouseNode.makeFixed();
	mouseNode.setMass(min_mass);
	mouseNode.moveTo(ofVec3f(0, 0, 0));
	mouseNode.setRadius(NODE_MAX_RADIUS*3);


#ifdef USE_KINECT
	for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
		msa::physics::Particle3D * joint = new msa::physics::Particle3D();
		physics.addParticle(joint);
		joint->makeFixed();
		joint->setMass(min_mass);
		joint->moveTo(ofVec3f(0, 0, 0));
		joint->setRadius(j==3 ? NODE_MAX_RADIUS*6 : NODE_MAX_RADIUS*3);

		bone.push_back(joint);
		boneEnabled.push_back(true);
	}
#endif
}




void facade::addRandomParticle() {
	float posX		= ofRandom(-width/2, width/2);
	float posY		= ofRandom(0, height);
	float posZ		= ofRandom(-width/2, width/2);
	float mass		= ofRandom(min_mass, max_mass);
	float bounce	= ofRandom(min_bounce, max_bounce);
	float radius	= ofMap(mass, min_mass, max_mass, NODE_MIN_RADIUS, NODE_MAX_RADIUS);
	
	// physics.makeParticle returns a particle pointer so you can customize it
	msa::physics::Particle3D *p = physics.makeParticle(ofVec3f(posX, posY, posZ));
	
	// and set a bunch of properties (you don't have to set all of them, there are defaults)
	p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
	
	// add an attraction to the mouseNode
#ifdef USE_KINECT
	if(mouseAttract) physics.makeAttraction(bone[physics.numberOfParticles()%kinect::nui::SkeletonData::POSITION_COUNT], p, ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
	if(mouseSpring && canIGo()) physics.makeSpring(bone[physics.numberOfParticles()%kinect::nui::SkeletonData::POSITION_COUNT], p, ofRandom(min_strength, max_strength), ofRandom(min_width, max_width));
	if(mouseSpring && canIGo()) physics.makeSpring(bone[(physics.numberOfParticles()+1)%kinect::nui::SkeletonData::POSITION_COUNT], p, ofRandom(min_strength, max_strength), ofRandom(min_width, max_width));
#else
	if(mouseAttract && canIGo()) physics.makeAttraction(&mouseNode, p, ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
	if(mouseSpring && canIGo()) physics.makeSpring(&mouseNode, p, ofRandom(min_strength, max_strength), ofRandom(min_width, max_width));
#endif
}


void facade::killRandomParticle() {
	msa::physics::Particle3D *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
	if(p && !p->isFixed()) p->kill();
}




void facade::setMouseSpring(bool s) {
	mouseSpring = s;
	if(mouseSpring){
#ifdef USE_KINECT
		int k = 0;
		for(int i=0; i<physics.numberOfParticles() && currentSkeletonIndex>-1; i++) {
			msa::physics::Particle3D *a = physics.getParticle(i);
			msa::physics::Particle3D *b = bone[i%kinect::nui::SkeletonData::POSITION_COUNT];
			if(canIGo()) physics.makeSpring(a, b, ofMap(i, 0, physics.numberOfParticles(), min_strength, max_strength), ofRandom(min_width, max_width));
			k++;
		}
#else
		for(int i=0; i<physics.numberOfParticles(); i++) {
			msa::physics::Particle3D *a = physics.getParticle(i);
			msa::physics::Particle3D *b = &mouseNode;
			if(canIGo()) physics.makeSpring(a, b, ofMap(i, 0, physics.numberOfParticles(), min_strength, max_strength), ofRandom(min_width, max_width));
		}
#endif
	}else{
		for(int i=physics.numberOfSprings(); i>0; i--) {
			msa::physics::Spring3D *s = physics.getSpring(i);
			if(s) s->kill();
		}
	}
}




void facade::setGravity(float g) {
	gravity = g;
	physics.setGravity(ofVec3f(0, gravity, 0));
}




void facade::setBounce(float min, float max) {
	min_bounce = min;
	max_bounce - max;
		
	for(int i=0; i<physics.numberOfParticles(); i++) {
		physics.getParticle(i)->setBounce(ofRandom(min_bounce, max_bounce));
	}
}





void facade::setMass(float min, float max) {
	min_mass = min;
	max_mass = max;

	for(int i=0; i<physics.numberOfParticles(); i++) {
		physics.getParticle(i)->setMass(ofRandom(min_mass, max_mass));
	}
	/*
#ifdef USE_KINECT
	for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
		bone[j]->setMass(mass);
	}
#else
	mouseNode.setMass(m);
#endif*/
}


void facade::setMouseAttract(bool a) {
	mouseAttract = a;
	if(mouseAttract) {
		// loop through all particles and add attraction to the closest bone
		/*msa::physics::Particle3D * closestBone;
		float minDistance;
		float tempDistance;
		for(int i=0; i<physics.numberOfParticles(); i++) {
			closestBone = NULL;
			minDistance = 999999999;
			for(int j = 0; j < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++j){
				for(int k = 0; k < kinect::nui::SkeletonData::POSITION_COUNT; ++k){
					tempDistance = bone[(j*kinect::nui::SkeletonData::POSITION_COUNT) + k]->getPosition().distance(physics.getParticle(i)->getPosition());
					if(tempDistance<minDistance){
						closestBone = bone[(j*kinect::nui::SkeletonData::POSITION_COUNT) + k];
						minDistance = tempDistance;
					}
				}
			}
			if(closestBone!=NULL){
				physics.makeAttraction(closestBone, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
			}
		}*/

		int k = 0;
		for(int i=0; i<physics.numberOfParticles() && currentSkeletonIndex>-1; i++) {
#ifdef USE_KINECT
			if(canIGo()) physics.makeAttraction(bone[i%kinect::nui::SkeletonData::POSITION_COUNT], physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
			k++;
#else
			if(canIGo()) physics.makeAttraction(&mouseNode, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
#endif		
		}
	} else {
		// loop through all existing attractsions and delete them
		for(int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
	}
}



void facade::setCollision(bool c) {
	if(c){
		physics.enableCollision();
#ifdef USE_KINECT
		for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
			bone[j]->enableCollision();
		}
#else
		mouseNode.enableCollision();
#endif
	}else{
		physics.disableCollision();
#ifdef USE_KINECT
		for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
			bone[j]->disableCollision();
		}
#else
		mouseNode.disableCollision();
#endif
	}
}

void facade::setStrength(float min, float max) {
	min_strength = min;
	max_strength = max;

	for(int i=physics.numberOfSprings(); i>0; i--) {
		msa::physics::Spring3D *s = physics.getSpring(i);
		if(s) {
			s->setStrength(ofMap(i, 0, physics.numberOfSprings(), min, max));
		}
	}
}

void facade::setOrbit(float min, float max) {
	min_width = min;
	max_width = max;

	for(int i=physics.numberOfSprings(); i>0; i--) {
		msa::physics::Spring3D *s = physics.getSpring(i);
		if(s) {
			s->setRestLength(ofMap(i, 0, physics.numberOfSprings(), min, max));
		}
	}
}

void facade::addRandomForce(float f) {
	forceTimer = f;
	for(int i=0; i<physics.numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics.getParticle(i);
		if(p->isFree()) p->addVelocity(ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
	}
}

void facade::initCanvas() {
	if(config::useCanvas){
		cout << "initCanvas" << endl;
		canvas.allocate(width, height);
		canvasTrace.allocate(width, height);
		canvasTrace.begin();
			ofSetColor(0);
			ofRect(0,0,width, height);
		canvasTrace.end();
	}else{
		cout << "do not initCanvas" << endl;
	}
}

bool facade::canIGo()
{
	return ofRandom(1.0)<=probability;
}