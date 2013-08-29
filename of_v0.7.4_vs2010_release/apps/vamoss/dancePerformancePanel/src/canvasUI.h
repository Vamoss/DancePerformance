#pragma once

#include "server.h"
#include "ofxUI.h"

#undef OFX_UI_GLOBAL_WIDGET_SPACING
#define OFX_UI_GLOBAL_WIDGET_SPACING 40

class canvasUI {
    
public:
    
	ofxUICanvas		*gui1;
    
    void setup()
    {
        setGUI1();
        
		playRotation = false;

        gui1->loadSettings("GUI/canvasSettings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {
        float dim = 16; 
        float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
        float length = 435-xInit; 
        
        gui1 = new ofxUICanvas(405, 0, length, 370);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("CanvasUI");
		gui1->setWidgetSpacing(15);
        gui1->addWidgetDown(new ofxUILabel("CANVAS", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "RENDER")); 
		gui1->addWidgetDown(new ofxUIRotarySlider(dim*3, 0, 360, 0.0, "ROTATION"));
        gui1->addWidgetDown(new ofxUILabelButton( 80, false, "CENTER", OFX_UI_FONT_MEDIUM));
        sliderRot = new ofxUISlider(length-xInit-160, dim, -3.0, 3.0, 0.0, "ROTATION SPEED");
		sliderRot->setIncrement(0.1);
		gui1->addWidgetDown(sliderRot);
        gui1->addWidgetRight(new ofxUILabelButton( 70, false, "PLAY", OFX_UI_FONT_MEDIUM));
		gui1->addWidgetDown(new ofxUISlider(length-xInit-80,dim, 0.0, 255.0, 125, "FADE"));	
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "PARTICLES")); 
		gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, 125, "BLACKOUT"));
        
		gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&canvasUI::guiEvent);
    }

	void update()
	{
		if(playRotation)
		{
			updateRotation(sliderRot->getScaledValue(), true);
		}
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
    }
    
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName(); 
        int kind = e.widget->getKind(); 
        cout << "got event from: " << name << endl; 	
        
        if(name == "RENDER")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			server::send(name, toggle->getValue());
		}
		else if(name == "FADE")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
			propagatePercent = slider->getValue();
		}else if(name == "PARTICLES")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			propagateFade = toggle->getValue();
		}
		else if(name == "BLACKOUT")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
		}
		else if(name == "ROTATION")
		{
			updateRotation();
		}
		else if((name == "PLAY" || name == "STOP") && isMouseDown)
        {
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			playRotation = !playRotation;
			if(playRotation){
				button->setLabelText("STOP");
			}else{
				button->setLabelText("PLAY");
			}
        }
		else if(name == "CENTER")
		{
			updateRotation(0);
		}
		else if(name == "SAVE")
        {
            save();
        }
	}
    
    void save()
    {
        gui1->saveSettings("GUI/canvasSettings.xml");
    }

	void updateRotation(float value = -9999.0f, bool relative = false)
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) gui1->getWidget("ROTATION");
		if(relative) {
			value += slider->getScaledValue();
			if(value>360) value = value - 360;
			if(value<0) value = 360 - value;
		}
		if(value!=-9999.0f) slider->setValue(value);
		server::send("ROTATION", slider->getScaledValue());
	}

	void mousePressed(int x, int y, int button)
	{
		isMouseDown=true;
	}

	void mouseReleased(int x, int y, int button)
	{
		isMouseDown=false;
	}
    
	bool isMouseDown;

	ofxUISlider * sliderRot;
	bool playRotation;

	bool propagateFade;
	float propagatePercent;
    
};
