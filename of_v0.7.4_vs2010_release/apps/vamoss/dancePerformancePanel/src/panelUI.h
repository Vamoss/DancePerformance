#pragma once

#include "server.h"
#include "ofxUI.h"
#include "config.h"
#include "UI.h"

#undef OFX_UI_FONT_SMALL_SIZE
#define OFX_UI_FONT_SMALL_SIZE 4

#undef OFX_UI_FONT_MEDIUM_SIZE
#define OFX_UI_FONT_MEDIUM_SIZE 4

#undef OFX_UI_GLOBAL_WIDGET_SPACING
#define OFX_UI_GLOBAL_WIDGET_SPACING 40

#undef OFX_UI_FONT_RESOLUTION
#define OFX_UI_FONT_RESOLUTION 40

class panelUI : public UI {
    
public:
    
    void setup()
    {
        setGUI1();
        
        gui1->loadSettings("GUI/settings.xml");
        gui1->setVisible(true);
    }
    
    void setGUI1()
    {   
		gui1 = new ofxUICanvas(0, 0, config::columnWidth, 768);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setWidgetSpacing(config::UISpace);
		gui1->setName("PanelUI");
		gui1->setWidgetSpacing(15);
        gui1->addWidgetDown(new ofxUILabel("DANCE PERFORMANCE", OFX_UI_FONT_LARGE)); 
        gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 
        
		gui1->addSpacer(2);
        gui1->addWidgetDown(new ofxUILabel("PHYSICS", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUIToggle( config::UIHeight,		config::UIHeight, false, "COLLISION")); 	
        gui1->addWidgetDown(new ofxUISlider(config::UIWidth,		config::UIHeight, 0.0,3000.0, 50, "PARTICLES"));	
        gui1->addWidgetDown(new ofxUISlider(config::UIWidth,		config::UIHeight, 0.0, 1.0, 1.0, "PROBABILITY"));
        gui1->addWidgetDown(new ofxUIToggle( config::UIHeight,		config::UIHeight, false, "SPRING"));
        gui1->addWidgetRight(new ofxUIToggle( config::UIHeight,		config::UIHeight, false, "ATTRACT")); 
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth,	config::UIHeight,	0.0,1.0, 0.07, 0.5, "STRENGHT"));
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth,	config::UIHeight,	0.0,100.0, 10.0, 30.0, "ORBIT"));
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth,	config::UIHeight, -0.9, 5.0, 1.0, 1.5, "MASS"));	
        gui1->addWidgetDown(new ofxUISlider(config::UIWidth-60,		config::UIHeight, -3.0, 3.0, 0.0, "GRAVITY"));
        gui1->addWidgetRight(new ofxUILabelButton( 50, false, "CENTER", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUIRangeSlider(config::UIWidth,	config::UIHeight, 0, 2.0, 0.2, 0.4, "BOUNCE"));	
		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth,	false, "SHAKE", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth,	false, "RESTART", OFX_UI_FONT_MEDIUM));

		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth,	false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
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
    }
    
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName(); 
        int kind = e.widget->getKind(); 
        cout << "got event from: " << name << endl; 	
        
        if(name == "PARTICLES")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
        }
		else if(name == "PROBABILITY")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
		}
        else if(name == "SPRING")
        {
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			server::send(name, toggle->getValue());
        }
        else if(name == "ATTRACT")
        {
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			server::send(name, toggle->getValue());
        }
		else if(name == "STRENGHT")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget; 
			server::send(name, slider->getScaledValueLow(), slider->getScaledValueHigh());
		}
		else if(name == "ORBIT")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget;
			server::send(name, slider->getScaledValueLow(), slider->getScaledValueHigh());
		}
        else if(name == "SHAKE")
        {
			server::send(name);
        }
        else if(name == "RESTART")
        {	
			server::send(name);
		}
		else if(name == "COLLISION")
		{
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
			server::send(name, toggle->getValue());
		}
		else if(name == "MASS")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget; 
			server::send(name, slider->getScaledValueLow(), slider->getScaledValueHigh());
		}
		else if(name == "GRAVITY")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			server::send(name, slider->getScaledValue());
		}
		else if(name == "BOUNCE")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget; 
			server::send(name, slider->getScaledValueLow(), slider->getScaledValueHigh());
		}
		else if(name == "CENTER")
		{
			((ofxUISlider *)gui1->getWidget("GRAVITY"))->setValue(0);
			server::send("GRAVITY", 0.0f);
		}
        else if(name == "SAVE")
        {
            save();
        }
	}

	void setParticles(float percent)
	{
		ofxUISlider *slider = (ofxUISlider *) gui1->getWidget("PARTICLES");
		if(percent != slider->getValue()){
			slider->setValue(percent * slider->getMax());
			server::send("PARTICLES", slider->getScaledValue());
		}
	}
    
    void save()
    {
        gui1->saveSettings("GUI/settings.xml");
    }
    
};
