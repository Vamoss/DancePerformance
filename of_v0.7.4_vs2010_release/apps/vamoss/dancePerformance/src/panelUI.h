#pragma once

#include "facade.h"
#include "ofxUI.h"

class panelUI {
    
public:
    
	//TODO
	//substitute key press
	//canvas alpha
	//spring tension
	//
    
	ofxUICanvas		*gui1;
    
    facade			*app;

    void setup(facade *app)
    {
        this->app = app;

        setGUI1();
        
        gui1->loadSettings("GUI/settings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {
        float dim = 16; 
        float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
        float length = 455-xInit; 
        
        gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight()); 
        gui1->addWidgetDown(new ofxUILabel("DANCE PERFORMANCE", OFX_UI_FONT_LARGE)); 
        gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 
        
        gui1->addWidgetDown(new ofxUISpacer(length-xInit, 2)); 
        gui1->addWidgetDown(new ofxUILabel("PHYSICS", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 2000.0, 50, "PARTICLES"));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "SPRING"));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "ATTRACT")); 
        gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SHAKE", OFX_UI_FONT_MEDIUM)); 	
        
        gui1->addWidgetDown(new ofxUISpacer(length-xInit, 2));
        gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        
        ofAddListener(gui1->newGUIEvent,this,&panelUI::guiEvent);
    }
    
    void keyPressed(int key)
    {   
        switch (key) 
        {
            case ' ':
                gui1->toggleVisible();
                break;	
            case 's':
                save();
                break;
        }

		//particles
	switch(key) {
		case 'a': 
			app->toggleMouseAttract();
			break;
		case 'p': 
			for(int i=0; i<100; i++) app->addRandomParticle(); 
			break;
		case 'P': 
			for(int i=0; i<100; i++) 
				app->killRandomParticle(); break;
		case 's': 
			app->toggleMouseSpring(); 
			break;
		case 'c': 
			app->physics.isCollisionEnabled() ? app->physics.disableCollision() : app->physics.enableCollision(); 
			break;
		case 'C': 
			app->killRandomConstraint(); 
			break;
		case 'r': 
			app->doRender ^= true; 
			break;
		case 'f': 
			app->addRandomForce(FORCE_AMOUNT); break;
		case 'F': 
			app->addRandomForce(FORCE_AMOUNT * 3); 
			break;
		case 'l': 
			app->lockRandomParticles(); 
			break;
		case 'u': 
			app->unlockRandomParticles(); 
			break;
		case '1': 
			app->initScene(); 
			break;
		case 'x': 
			app->doMouseXY = true; 
			break;
		case 'z': 
			app->doMouseYZ = true; 
			break;
		case ']': 
			app->rotSpeed += 0.01f; 
			break;
		case '[': 
			app->rotSpeed -= 0.01f; 
			break;
		case '+': 
#ifdef USE_KINECT
			for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++i){
				for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
					app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->setMass(app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->getMass() +0.1);
				}
			}
#else
			app->mouseNode.setMass(app->mouseNode.getMass() +0.1);
#endif
			break;
		case '-': 
#ifdef USE_KINECT
			for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++i){
				for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
					app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->setMass(app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->getMass() -0.1);
				}
			}
#else
			app->mouseNode.setMass(app->mouseNode.getMass() -0.1); 
#endif
			break;
		case 'm': 
			bool collision = app->mouseNode.hasCollision();
			if(collision){
#ifdef USE_KINECT
				for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++i){
					for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
						app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->disableCollision();
					}
				}
#else
				app->mouseNode.disableCollision();
#endif

			}else{
#ifdef USE_KINECT
				for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; ++i){
					for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; ++j){
						app->bone[(i*kinect::nui::SkeletonData::POSITION_COUNT) + j]->enableCollision();
					}
				}
			}
#else
				app->mouseNode.enableCollision();
#endif
			break;
	}
    }
    
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName(); 
        int kind = e.widget->getKind(); 
        cout << "got event from: " << name << endl; 	
        
        if(name == "PARTICLES")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget; 
			for(int i=app->physics.numberOfParticles(); i < slider->getScaledValue(); i++) app->addRandomParticle();
			for(int i=app->physics.numberOfParticles(); i > slider->getScaledValue(); i--) app->killRandomParticle();
        }
        else if(name == "SPRING")
        {
			app->toggleMouseSpring();
        }
        else if(name == "ATTRACT")
        {
			app->toggleMouseAttract();
        }
        else if(name == "SHAKE")
        {
			app->addRandomForce(FORCE_AMOUNT);
        }
        else if(name == "SAVE")
        {
            save();
        }
	}
    
    void save()
    {
        gui1->saveSettings("GUI/settings.xml");
    }
    
};
