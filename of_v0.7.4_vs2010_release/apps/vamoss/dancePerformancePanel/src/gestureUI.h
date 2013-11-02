#pragma once

#include "server.h"
#include "ofxUI.h"
#include "UI.h"

class gestureUI : public UI {

public:
    
    void setup()
    {
        setGUI1();
        
        gui1->loadSettings("GUI/gestureSettings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {   
		gui1 = new ofxUICanvas((config::columnWidth + config::columnSpace)*3, 0, config::columnWidth, 488);
		gui1->setWidgetSpacing(config::UISpace);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("GestureUI");
		gui1->setWidgetSpacing(15);
		gui1->addWidgetDown(new ofxUILabel("GESTURE", OFX_UI_FONT_MEDIUM));
		
		
		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 1.0, 0.0, "VOLUME"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HEAD X"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 0"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HEAD X"));

        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HEAD Y"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 1"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HEAD Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HEAD Z"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 2"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HEAD Z"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HAND DIST"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 3"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND DIST"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HAND LEFT Y"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 4"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND LEFT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 1000.0, 100.00, 1900.0, "VAL HAND LEFT VEL"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 5"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND LEFT VEL"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL HAND RIGHT Y"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 6"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND RIGHT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 1000.0, 100.00, 1900.0, "VAL HAND RIGHT VEL"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 7"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND RIGHT VEL"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL FOOT DIST"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 8"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "FOOT DIST"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL FOOT LEFT Y"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 9"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "FOOT LEFT Y"));
		
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth-150, config::UIHeight,	-2000.0, 2000.0, 100.00, 1900.0, "VAL FOOT RIGHT Y"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, false, "INV 10"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "FOOT RIGHT Y"));

		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
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
