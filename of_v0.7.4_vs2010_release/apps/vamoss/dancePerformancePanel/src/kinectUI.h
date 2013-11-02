#pragma once

#include "server.h"
#include "ofxUI.h"
#include "UI.h"

class kinectUI : public UI {
    
public:
    
    void setup()
    {
        setGUI1();
        
        gui1->loadSettings("GUI/kinectSettings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {
		gui1 = new ofxUICanvas((config::columnWidth + config::columnSpace)*2, 0, config::columnWidth, 768);
		gui1->setWidgetSpacing(config::UISpace);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("KinectUI");
		gui1->setWidgetSpacing(15);
		gui1->addWidgetDown(new ofxUILabel("KINECT", OFX_UI_FONT_MEDIUM));
		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 1.0, 0.0, "DELAY"));
		((ofxUISlider *) gui1->getWidget("DELAY"))->setIncrement(0.01f);

		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, -500.0, 500.0, 3.0, "Y"));
		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 10.0, 3.0, "SCALE"));
		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth/2-10, false, "SELECT ALL", OFX_UI_FONT_MEDIUM)); 	
		gui1->addWidgetRight(new ofxUILabelButton( config::UIWidth/2-10, false, "UNSELECT ALL", OFX_UI_FONT_MEDIUM)); 	
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HEAD"));	
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "SHOULDER LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "SHOULDER CENTER"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "SHOULDER RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "ELBOW LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "ELBOW RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "WRIST LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "WRIST RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HAND RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "SPINE"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HIP LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HIP CENTER"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "HIP RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "KNEE LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "KNEE RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "ANKLE LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "ANKLE RIGHT"));
		gui1->addWidgetDown(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "FOOT LEFT"));
		gui1->addWidgetRight(new ofxUIToggle( config::UIHeight, config::UIHeight, true, "FOOT RIGHT"));

		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&kinectUI::guiEvent);
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
        
        if(name == "DELAY")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget;
			server::send(name, slider->getScaledValue());
		}
		else if(name == "Y")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget;
			server::send(name, slider->getScaledValue());
		}
		else if(name == "SCALE")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget;
			server::send(name, slider->getScaledValue());
		}
		else if(name == "SAVE")
        {
            save();
        }
		else if(name == "SELECT ALL" || name == "UNSELECT ALL")
        {
			bool select = name == "SELECT ALL";

			vector<ofxUIWidget*> toggles = gui1->getWidgetsOfType(OFX_UI_WIDGET_TOGGLE);
			for(vector<ofxUIWidget*>::iterator it = toggles.begin(); it != toggles.end(); it++) {
				ofxUIToggle* toggle = (ofxUIToggle *) *it;
				toggle->setValue(select);
				server::send(toggle->getName(), toggle->getValue());
			}
        }
		else
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
			server::send(name, toggle->getValue());
		}
		
	}
    
    void save()
    {
        gui1->saveSettings("GUI/kinectSettings.xml");
    }

	ofxUISlider * sliderRot;
    
};
