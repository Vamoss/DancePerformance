#pragma once

#include "facade.h"
#include "ofxUI.h"

#undef OFX_UI_GLOBAL_WIDGET_SPACING
#define OFX_UI_GLOBAL_WIDGET_SPACING 40

class colorUI {
    
public:
    
	ofxUICanvas		*gui1;
    
    facade			*app;

    void setup(facade *app)
    {
        this->app = app;

        setGUI1();
        
        gui1->loadSettings("GUI/colorSettings.xml");
        gui1->setVisible(true);
    
		velRed = velGreen = velBlue = 1;
		playRed = playGreen = playBlue = false;
		isMouseDown = false;
    }
    
    void setGUI1()
    {
        float dim = 16; 
        float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
        float length = 455-xInit; 
        
        gui1 = new ofxUICanvas(465, 0, length+xInit, ofGetHeight());
		gui1->setName("ColorUI");
		gui1->setWidgetSpacing(20);
        gui1->addWidgetDown(new ofxUILabel("DANCE PERFORMANCE", OFX_UI_FONT_LARGE)); 
        gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 
        
		gui1->addSpacer(2);
        gui1->addWidgetDown(new ofxUILabel("COLORS", OFX_UI_FONT_MEDIUM));

		redSlider = new ofxUISlider(length-xInit-80, dim, 0.0, 255.0, 255.0, "RED");
        gui1->addWidgetDown(redSlider);

		ofxUILabelButton * b1 = new ofxUILabelButton( 70, false, "PLAY 1", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b1);

		greenSlider = new ofxUISlider(length-xInit-80, dim, 0.0, 255.0, 255.0, "GREEN");
        gui1->addWidgetDown(greenSlider);

		ofxUILabelButton * b2 = new ofxUILabelButton( 70, false, "PLAY 2", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b2);

		blueSlider = new ofxUISlider(length-xInit-80, dim, 0.0, 255.0, 255.0, "BLUE");
        gui1->addWidgetDown(blueSlider);

		ofxUILabelButton * b3 = new ofxUILabelButton( 70, false, "PLAY 3", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b3);

        gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "RANDOM", OFX_UI_FONT_MEDIUM));
        
		gui1->addSpacer(2);
		gui1->addWidgetDown(new ofxUILabelButton( length-xInit, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&colorUI::guiEvent);
    }

	void update()
	{
		if(playRed)
		{
			redSlider->setValue(redSlider->getScaledValue()+velRed);
			if(redSlider->getScaledValue()>=255 || redSlider->getScaledValue()<=0) velRed *= -1;
			app->particleColor.r = redSlider->getScaledValue();
		}
		
		if(playGreen)
		{
			greenSlider->setValue(greenSlider->getScaledValue()+velGreen);
			if(greenSlider->getScaledValue()>=255 || greenSlider->getScaledValue()<=0) velGreen *= -1;
			app->particleColor.g = greenSlider->getScaledValue();
		}
		
		if(playBlue)
		{
			blueSlider->setValue(blueSlider->getScaledValue()+velBlue);
			if(blueSlider->getScaledValue()>=255 || blueSlider->getScaledValue()<=0) velBlue *= -1;
			app->particleColor.b = blueSlider->getScaledValue();
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
        cout << "got event from: " << name << " " << endl; 	
        
        if(name == "RED")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->particleColor.r = slider->getScaledValue();
		}
        else if(name == "GREEN")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->particleColor.g = slider->getScaledValue();
		}
        else if(name == "BLUE")
        {
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->particleColor.b = slider->getScaledValue();
        }
		else if((name == "PLAY 1" || name == "STOP 1") && isMouseDown)
        {
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			playRed = !playRed;
			if(playRed){
				button->setLabelText("STOP 1");
			}else{
				button->setLabelText("PLAY 1");
			}
        }
        else if((name == "PLAY 2" || name == "STOP 2") && isMouseDown)
        {
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			playGreen = !playGreen;
			if(playGreen){
				button->setLabelText("STOP 2");
			}else{
				button->setLabelText("PLAY 2");
			}
        }
        else if((name == "PLAY 3" || name == "STOP 3") && isMouseDown)
        {
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			playBlue = !playBlue;
			if(playBlue){
				button->setLabelText("STOP 3");
			}else{
				button->setLabelText("PLAY 3");
			}
        }
        else if(name == "RANDOM" && isMouseDown)
        {
			redSlider->setValue(ofRandom(255));
			greenSlider->setValue(ofRandom(255));
			blueSlider->setValue(ofRandom(255));
			
			app->particleColor.r = redSlider->getScaledValue();
			app->particleColor.g = greenSlider->getScaledValue();
			app->particleColor.b = blueSlider->getScaledValue();
        }
        else if(name == "SAVE")
        {
            save();
        }
	}
    
    void save()
    {
        gui1->saveSettings("GUI/colorSettings.xml");
    }
	
	void mousePressed(int x, int y, int button)
	{
		cout << "a" << endl;
		isMouseDown=true;
	}

	void mouseReleased(int x, int y, int button)
	{
		cout << "B" << endl;
		isMouseDown=false;
	}
    
	bool isMouseDown;
	int velRed, velGreen, velBlue;
	bool playRed, playGreen, playBlue;
	ofxUISlider * redSlider, * greenSlider, * blueSlider;
};
