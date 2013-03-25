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
        
        gui1->loadSettings("GUI/canvasSettings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {
        float dim = 16; 
        float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
        float length = 435-xInit; 
        
        gui1 = new ofxUICanvas(405, 0, length, ofGetHeight());
		gui1->setName("CanvasUI");
		gui1->setWidgetSpacing(20);
        gui1->addWidgetDown(new ofxUILabel("CANVAS", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "RENDER")); 
        sliderRot = new ofxUISlider(length-xInit-60, dim, -3.0, 3.0, 0.0, "ROTATION SPEED");
		sliderRot->setIncrement(0.1);
		gui1->addWidgetDown(sliderRot);
        gui1->addWidgetRight(new ofxUILabelButton( 50, false, "CENTER", OFX_UI_FONT_MEDIUM));
		gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, 125, "FADE"));	
		gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, 125, "BLACKOUT"));
        
		gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&canvasUI::guiEvent);
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
		}
		else if(name == "BLACKOUT")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
		}
		else if(name == "ROTATION SPEED")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
		}
		else if(name == "CENTER")
		{
			server::send("ROTATION SPEED", 0.0f);
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

	ofxUISlider * sliderRot;
    
};
