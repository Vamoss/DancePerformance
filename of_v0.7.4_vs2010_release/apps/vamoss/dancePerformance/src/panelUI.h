#pragma once

#include "facade.h"
#include "ofxUI.h"

#undef OFX_UI_GLOBAL_WIDGET_SPACING
#define OFX_UI_GLOBAL_WIDGET_SPACING 40

class panelUI {
    
public:
    
	//TODO
	//substitute define vars
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
		gui1->setWidgetSpacing(20);
        gui1->addWidgetDown(new ofxUILabel("DANCE PERFORMANCE", OFX_UI_FONT_LARGE)); 
        gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 
        
		gui1->addSpacer(2);
        gui1->addWidgetDown(new ofxUILabel("PHYSICS", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "RENDER")); 
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "SPRING"));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "ATTRACT")); 
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "COLLISION")); 	
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit, dim,	0.0,1.0, 0.07, 0.5, "STRENGHT"));
        gui1->addWidgetDown(new ofxUISlider(length-xInit, dim, 0.0,2000.0, 50, "PARTICLES"));	
        gui1->addWidgetDown(new ofxUISlider(length-xInit, dim, -0.9, 5.0, 1.5, "MASS"));
		ofxUISlider * sliderRot = new ofxUISlider(length-xInit, dim, -3.0, 3.0, 0.0, "ROTATION SPEED");
		sliderRot->setIncrement(0.1);
		gui1->addWidgetDown(sliderRot);
        gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SHAKE", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "RESTART", OFX_UI_FONT_MEDIUM));

        gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, 125, "FADE"));	
        
		gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&panelUI::guiEvent);
    }

	void draw()
	{
		gui1->draw();
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
			case 'x': 
				app->doMouseXY = true; 
				break;
			case 'z': 
				app->doMouseYZ = true; 
				break;
		}
    }
    
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName(); 
        int kind = e.widget->getKind(); 
        cout << "got event from: " << name << endl; 	
        
        if(name == "RENDER")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			app->doRender = toggle->getValue(); 
		}
		else if(name == "PARTICLES")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget; 
			for(int i=app->physics.numberOfParticles(); i < slider->getScaledValue(); i++) app->addRandomParticle();
			for(int i=app->physics.numberOfParticles(); i > slider->getScaledValue(); i--) app->killRandomParticle();
        }
        else if(name == "SPRING")
        {
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			app->setMouseSpring(toggle->getValue());
        }
        else if(name == "ATTRACT")
        {
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			app->setMouseAttract(toggle->getValue());
        }
		else if(name == "STRENGHT")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget; 
			app->setStrength(slider->getScaledValueLow(), slider->getScaledValueHigh());
		}
        else if(name == "SHAKE")
        {
			app->addRandomForce(FORCE_AMOUNT);
        }
        else if(name == "RESTART")
        {
			app->initScene(); 
        }
		else if(name == "FADE")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->canvasFade = slider->getScaledValue();
		}
		else if(name == "COLLISION")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			app->setCollision(toggle->getValue());
		}
		else if(name == "ROTATION SPEED")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->rotSpeed = slider->getScaledValue(); 
		}
		else if(name == "MASS")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->setMass(slider->getScaledValue()); 
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
