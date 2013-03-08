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
	initSetting.grabDepth = true;
	initSetting.grabAudio = true;
	initSetting.grabLabel = true;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = true;
	initSetting.grabLabelCv = true;
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
	bRecord = false;
	bPlayback = false;
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	
	bDrawVideo = false;
	bDrawDepthLabel = false;
	bDrawSkeleton = false;
	bDrawCalibratedTexture = false;

	ofSetFrameRate(60);
	
	calibratedTexture.allocate(kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight(), GL_RGB);

	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo(kinect.getVideoResolution());
	depthDraw_ = ofxKinectNuiDrawTexture::createTextureForDepth(kinect.getDepthResolution());
	labelDraw_ = ofxKinectNuiDrawTexture::createTextureForLabel(kinect.getDepthResolution());
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton();
	kinect.setVideoDrawer(videoDraw_);
	kinect.setDepthDrawer(depthDraw_);
	kinect.setLabelDrawer(labelDraw_);
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
}

//--------------------------------------------------------------
void testApp::update() {
	kinectSource->update();
	if(bRecord){
		kinectRecorder.update();
	}
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif

	//PARTICLE
	width = ofGetWidth();
	height = ofGetHeight();
	
	physics.update();
	
	
	/*// Update the magnets
	for(int i = 0; i < 6; ++i){
		for(int j = 0; j < 20; ++j){
			if(kinect.skeletonPoints[i][0].z > 0){
				movingMagnets[(i*20) + j]->x = kinect.skeletonPoints[i][j].x;
				movingMagnets[(i*20) + j]->y = kinect.skeletonPoints[i][j].y;
			}else{
				movingMagnets[(i*20) + j]->x = 999999;
				movingMagnets[(i*20) + j]->y = 999999;
			}
		}
	}*/
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(100, 100, 100);
	ofSetColor(255);
	// Draw video only
	if(bDrawVideo){
		// draw video images from kinect camera
		kinect.drawVideo(0, 0, 1024, 768);
	// Draw depth + users label only
	}else if(bDrawDepthLabel){
		ofEnableAlphaBlending();

		// draw depth images from kinect depth sensor
		kinect.drawDepth(0, 0, 1024, 768);
		// draw players' label images on video images
		kinect.drawLabel(0, 0, 1024, 768);
		ofDisableAlphaBlending();
	// Draw skeleton only
	}else if(bDrawSkeleton){
		kinect.drawSkeleton(0, 0, 1024, 768);	// draw skeleton images on video images
	// Draw calibrated image only
	}else if(bDrawCalibratedTexture){
		ofPushMatrix();
		drawCalibratedTexture();	// draw calibrated images coodinates to depth images
		ofPopMatrix();
	}else{
		if(!bPlayback){
			// draw video images from kinect camera
			kinect.drawVideo(20, 20, 400, 300);
			ofEnableAlphaBlending();
			// draw depth images from kinect depth sensor
			kinect.drawDepth(20, 340, 400, 300);
			// draw players' label images on video images
			kinect.drawLabel(20, 340, 400, 300);
			ofDisableAlphaBlending();
			// draw skeleton images on video images
			kinect.drawSkeleton(20, 20, 400, 300);

#ifdef USE_TWO_KINECTS
			kinect2.draw(440, 20, 400, 300);
			kinect2.drawDepth(440, 340, 400, 300);
#endif
		}else{
			kinectPlayer.draw(20, 20, 400, 300);
			ofEnableAlphaBlending();
			kinectPlayer.drawDepth(20, 340, 400, 300);
			kinectPlayer.drawLabel(20, 340, 400, 300);

			ofDisableAlphaBlending();
			kinectPlayer.drawSkeleton(20, 20, 400, 300);
		}
	}

	ofPushMatrix();
	ofTranslate(35, 35);
	ofFill();
	if(bRecord) {
		ofSetColor(255, 0, 0);
		ofCircle(0, 0, 10);
	}
	if(bPlayback) {
		ofSetColor(0, 255, 0);
		ofTriangle(-10, -10, -10, 10, 10, 0);
	}
	ofPopMatrix();

	stringstream kinectReport;
	if(bPlugged && !kinect.isOpened() && !bPlayback){
		ofSetColor(0, 255, 0);
		kinectReport << "Kinect is plugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 300);
	}else if(!bPlugged){
		ofSetColor(255, 0, 0);
		kinectReport << "Kinect is unplugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 300);
	}

	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "fps: " << ofGetFrameRate() << "  Kinect Nearmode: " << kinect.isNearmode() << endl
				 << "press 'c' to close the stream and 'o' to open it again, stream is: " << kinect.isOpened() << endl
				 << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
				 << "press LEFT and RIGHT to change the far clipping distance: " << farClipping << " mm" << endl
				 << "press '+' and '-' to change the near clipping distance: " << nearClipping << " mm" << endl
				 << "press 'r' to record and 'p' to playback, record is: " << bRecord << ", playback is: " << bPlayback << endl
				 << "press 'v' to show video only: " << bDrawVideo << ",      press 'd' to show depth + users label only: " << bDrawDepthLabel << endl
				 << "press 's' to show skeleton only: " << bDrawSkeleton << ",   press 'q' to show point cloud sample: " << bDrawCalibratedTexture;
	ofDrawBitmapString(reportStream.str(), 20, 648);
	
	for(int i = 0; i < 6; ++i){
		for(int j = 0; j < 20; ++j){
			if(kinect.skeletonPoints[i][0].z > 0){
				ofSetColor(255, 0, 0);
				ofCircle(kinect.skeletonPoints[i][j].x*2, kinect.skeletonPoints[i][j].y*2, 3);
				ofSetColor(255);
				ofDrawBitmapString(ofToString(j), kinect.skeletonPoints[i][j].x*2, kinect.skeletonPoints[i][j].y*2);
			}
		}
	}

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
		glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, width/2);
		glColor3f(1, 1, 1);				glVertex3f(width/2, -height, width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, -width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, -width/2);
		
		// back wall
		glColor3f(.9, 0.9, 0.9);		glVertex3f(width/2, height+1, -width/2);
		glColor3f(1, 1, 1);				glVertex3f(width/2, -height, -width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, -width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, -width/2);
		
		// left wall
		glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, -width/2);
		glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, -width/2);
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(-width/2, -height, width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(-width/2, height+1, width/2);
		
		// front wall
		glColor3f(0.95, 0.95, 0.95);	glVertex3f(width/2, -height, width/2);
		glColor3f(.85, 0.85, 0.85);		glVertex3f(width/2, height+1, width/2);
		glColor3f(.9, 0.9, 0.9);		glVertex3f(-width/2, height+1, width/2);
		glColor3f(1, 1, 1);				glVertex3f(-width/2, -height, width/2);
		
		// floor
		glColor3f(.8, 0.8, 0.8);
		glVertex3f(width/2, height+1, width/2);
		glVertex3f(width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, -width/2);
		glVertex3f(-width/2, height+1, width/2);
		glEnd();
		
//		glEnable(GL_LIGHTING);
		
		// draw springs
		glColor4f(0.5, 0.5, 0.5, 0.5);
		for(int i=0; i<physics.numberOfSprings(); i++) {
			msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physics.getSpring(i);
			msa::physics::Particle3D *a = spring->getOneEnd();
			msa::physics::Particle3D *b = spring->getTheOtherEnd();
			ofVec3f vec = b->getPosition() - a->getPosition();
			float dist = vec.length();
			float angle = acos( vec.z / dist ) * RAD_TO_DEG;
			if(vec.z <= 0 ) angle = -angle;
			float rx = -vec.y * vec.z;
			float ry =  vec.x * vec.z;
			
			glPushMatrix();
			glTranslatef(a->getPosition().x, a->getPosition().y, a->getPosition().z);
			glRotatef(angle, rx, ry, 0.0);
			float size  = ofMap(spring->getStrength(), SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH, SPRING_MIN_WIDTH, SPRING_MAX_WIDTH);
			
			glScalef(size, size, dist);
			glTranslatef(0, 0, 0.5);
            ofBox(1);
			glPopMatrix();
		}
		
		
		//particles
		glAlphaFunc(GL_GREATER, 0.5);
		
		ofEnableNormalizedTexCoords();
		ballImage.getTextureReference().bind();
		for(int i=0; i<physics.numberOfParticles(); i++) {
			msa::physics::Particle3D *p = physics.getParticle(i);
			if(p->isFixed()) glColor4f(1, 0, 0, 1);
			else glColor4f(1, 1, 1, 1);

			glEnable(GL_ALPHA_TEST);
			
			// draw ball
			glPushMatrix();
			glTranslatef(p->getPosition().x, p->getPosition().y, p->getPosition().z);
			glRotatef(180-rot, 0, 1, 0);

			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
			glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
			glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());
			glEnd();
			glPopMatrix();
			
			glDisable(GL_ALPHA_TEST);
			
			float alpha = ofMap(p->getPosition().y, -height * 1.5, height, 0, 1);
			if(alpha>0) {
				glPushMatrix();
				glTranslatef(p->getPosition().x, height, p->getPosition().z);
				glRotatef(-90, 1, 0, 0);
				glColor4f(0, 0, 0, alpha * alpha * alpha * alpha);
//				ofCircle(0, 0, p->getRadius());
				float r = p->getRadius() * alpha;
				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex2f(-r, -r);
				glTexCoord2f(1, 0); glVertex2f(r, -r);
				glTexCoord2f(1, 1); glVertex2f(r, r);
				glTexCoord2f(0, 1); glVertex2f(-r, r);
				glEnd();
				glPopMatrix();
			}
			
		}
		ballImage.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();
		
		
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
void testApp::drawCalibratedTexture(){
	int offsetX = -400;
	int offsetY = -300;
	glTranslatef(512, 386, 0);
	calibratedTexture.loadData(kinect.getCalibratedVideoPixels());
	for(int y = 0; y < kinect.getDepthResolutionHeight(); y++){
		for(int x = 0; x < kinect.getDepthResolutionWidth(); x++){
			float distance = kinect.getDistanceAt(x, y);
			if(distance > 500 && distance < 1500){
				glPushMatrix();
				float radius = (1500 - distance);
				ofSetColor(kinect.getCalibratedColorAt(x, y));
				ofRotateY(mRotationY);
				ofRotateX(mRotationX);
				glTranslatef(x * 2.5 + offsetX, y * 2.5 + offsetY, radius);
				ofBox(5);
				glPopMatrix();
			}else{
				glPushMatrix();
				ofSetColor(kinect.getCalibratedColorAt(x, y));
				ofRotateY(mRotationY);
				ofRotateX(mRotationX);
				ofRect(x * 2.5 + offsetX, y * 2.5 + offsetY, 5, 5);
				glPopMatrix();
			}
		}
	}
}


//--------------------------------------------------------------
void testApp::exit() {
	if(calibratedTexture.bAllocated()){
		calibratedTexture.clear();
	}

	if(videoDraw_) {
		videoDraw_->destroy();
		videoDraw_ = NULL;
	}
	if(depthDraw_) {
		depthDraw_->destroy();
		depthDraw_ = NULL;
	}
	if(labelDraw_) {
		labelDraw_->destroy();
		labelDraw_ = NULL;
	}
	if(skeletonDraw_) {
		delete skeletonDraw_;
		skeletonDraw_ = NULL;
	}

	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);
	kinectPlayer.close();
	kinectRecorder.close();


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
			break;
		case '-': 
			mouseNode.setMass(mouseNode.getMass() -0.1); 
			break;
		case 'm': 
			mouseNode.hasCollision() ? mouseNode.disableCollision() : mouseNode.enableCollision();
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

//--------------------------------------------------------------
void testApp::startRecording(){
	if(!bRecord){
		// stop playback if running
		stopPlayback();

		kinectRecorder.setup(kinect, "recording.dat");
		bRecord = true;
	}
}

//--------------------------------------------------------------
void testApp::stopRecording(){
	if(bRecord){
		kinectRecorder.close();
		bRecord = false;
	}
}

//--------------------------------------------------------------
void testApp::startPlayback(){
	if(!bPlayback){
		stopRecording();
		kinect.close();

		// set record file and source
		kinectPlayer.setup("recording.dat");
		kinectPlayer.loop();
		kinectPlayer.play();
		kinectSource = &kinectPlayer;
		bPlayback = true;
	}
}

//--------------------------------------------------------------
void testApp::stopPlayback(){
	if(bPlayback){
		kinectPlayer.close();
		kinect.open();
		kinectSource = &kinect;
		bPlayback = false;
	}
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

