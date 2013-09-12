#pragma once

#include "server.h"
#include "ofxUI.h"

#undef OFX_UI_GLOBAL_WIDGET_SPACING
#define OFX_UI_GLOBAL_WIDGET_SPACING 40

class gestureUI {
    
public:
    
	ofxUICanvas		*gui1;
    
    void setup()
    {
        setGUI1();
        
        gui1->loadSettings("GUI/gestureSettings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {
        float dim = 16; 
        float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
        float length = 435-xInit; 
        
		gui1 = new ofxUICanvas(405*3, 0, length, 768);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("GestureUI");
		gui1->setWidgetSpacing(15);
		gui1->addWidgetDown(new ofxUILabel("GESTURE", OFX_UI_FONT_MEDIUM));
		
		
		gui1->addWidgetDown(new ofxUISlider(length-xInit, dim, 0.0, 1.0, 0.0, "VOLUME"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HEAD X"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 0"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HEAD X"));

        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HEAD Y"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 1"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HEAD Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HEAD Z"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 2"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HEAD Z"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HAND DIST"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 3"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HAND DIST"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HAND LEFT Y"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 4"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HAND LEFT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL HAND RIGHT Y"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 5"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "HAND RIGHT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL FOOT DIST"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 6"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "FOOT DIST"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL FOOT LEFT Y"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 7"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "FOOT LEFT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit-150, dim,	0.0, 2000.0, 100.00, 1900.0, "VAL FOOT RIGHT Y"));
		gui1->addWidgetRight(new ofxUIToggle( dim, dim, false, "INV 8"));
        gui1->addWidgetRight(new ofxUIToggle( dim, dim, true, "FOOT RIGHT Y"));

		gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&gestureUI::guiEvent);
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
        
        if(kind == OFX_UI_WIDGET_SLIDER_H)
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget;
			server::send(name, slider->getScaledValue());
		}
		else if(kind == OFX_UI_WIDGET_RSLIDER_H)
        {	
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget;
			server::send(name, slider->getScaledValueLow(), slider->getScaledValueHigh());
        }
		else if(kind == OFX_UI_WIDGET_TOGGLE)
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			server::send(name, toggle->getValue());
		}
		else if(name == "SAVE")
        {
            save();
        }
		
	}
    
    void save()
    {
        gui1->saveSettings("GUI/gestureSettings.xml");
    }

	ofxUISlider * sliderRot;
    
};
