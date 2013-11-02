#pragma once

#include "server.h"
#include "ofxUI.h"
#include "UI.h"

class colorUI : public UI {
    
public:

    void setup()
    {
        setGUI1();
        
        gui1->loadSettings("GUI/colorSettings.xml");
        gui1->setVisible(true);
    
		colorSpeed = 1;
		velRed = velGreen = velBlue = colorSpeed;
		playRed = playGreen = playBlue = false;
		isMouseDown = false;
    }
    
    void setGUI1()
    {
        gui1 = new ofxUICanvas(config::columnWidth + config::columnSpace, 400, config::columnWidth, 367);
		gui1->setWidgetSpacing(config::UISpace);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("ColorUI");
		gui1->setWidgetSpacing(15);
        gui1->addWidgetDown(new ofxUILabel("COLORS", OFX_UI_FONT_MEDIUM));

		redSlider = new ofxUISlider(config::UIWidth-80, config::UIHeight, 0.0, 255.0, 255.0, "RED");
        gui1->addWidgetDown(redSlider);

		ofxUILabelButton * b1 = new ofxUILabelButton( 70, false, "PLAY 1", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b1);

		greenSlider = new ofxUISlider(config::UIWidth-80, config::UIHeight, 0.0, 255.0, 255.0, "GREEN");
        gui1->addWidgetDown(greenSlider);

		ofxUILabelButton * b2 = new ofxUILabelButton( 70, false, "PLAY 2", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b2);

		blueSlider = new ofxUISlider(config::UIWidth-80, config::UIHeight, 0.0, 255.0, 255.0, "BLUE");
        gui1->addWidgetDown(blueSlider);

		ofxUILabelButton * b3 = new ofxUILabelButton( 70, false, "PLAY 3", OFX_UI_FONT_MEDIUM);
        gui1->addWidgetRight(b3);

		gui1->addWidgetDown(new ofxUILabelButton( 90, false, "BLACK", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetRight(new ofxUILabelButton( 90, false, "WHITE", OFX_UI_FONT_MEDIUM));

		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 30.0, &colorSpeed, "PLAY SPEED"));

		gui1->addWidgetDown(new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 1.0, 0.0, "SPACE VARIATION"));

        gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth, false, "RANDOM", OFX_UI_FONT_MEDIUM));
        
		gui1->addWidgetDown(new ofxUILabelButton( config::UIWidth, false, "SAVE", OFX_UI_FONT_MEDIUM)); 	
        
        ofAddListener(gui1->newGUIEvent,this,&colorUI::guiEvent);
    }

	void update()
	{
		if(playRed)
		{
			velRed = velRed>0 ? colorSpeed : -colorSpeed;
			redSlider->setValue(redSlider->getScaledValue()+velRed);
			if(redSlider->getScaledValue()>=255 || redSlider->getScaledValue()<=0) velRed *= -1;
		}
		
		if(playGreen)
		{
			velGreen = velGreen>0 ? colorSpeed : -colorSpeed;
			greenSlider->setValue(greenSlider->getScaledValue()+velGreen);
			if(greenSlider->getScaledValue()>=255 || greenSlider->getScaledValue()<=0) velGreen *= -1;
		}
		
		if(playBlue)
		{
			velBlue = velBlue>0 ? colorSpeed : -colorSpeed;
			blueSlider->setValue(blueSlider->getScaledValue()+velBlue);
			if(blueSlider->getScaledValue()>=255 || blueSlider->getScaledValue()<=0) velBlue *= -1;
		}

		if(playRed || playGreen || playBlue){
			updateColor();
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
        
        if(name == "RED" || name == "GREEN" || name == "BLUE")
		{
			updateColor();
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
        else if(name == "BLACK" && isMouseDown)
        {
			redSlider->setValue(0);
			greenSlider->setValue(0);
			blueSlider->setValue(0);

			updateColor();
        }
        else if(name == "WHITE" && isMouseDown)
        {
			redSlider->setValue(255);
			greenSlider->setValue(255);
			blueSlider->setValue(255);

			updateColor();
        }
        else if(name == "RANDOM" && isMouseDown)
        {
			redSlider->setValue(ofRandom(255));
			greenSlider->setValue(ofRandom(255));
			blueSlider->setValue(ofRandom(255));
			
			updateColor();
        }
		else if(name == "SPACE VARIATION")
		{
			ofxUISlider *slider = (ofxUISlider*) e.widget; 
			server::send(name, slider->getScaledValue());
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

	void updateColor()
	{
		//ofBackground(redSlider->getScaledValue(), greenSlider->getScaledValue(), blueSlider->getScaledValue());
		server::send("COLOR", redSlider->getScaledValue(), greenSlider->getScaledValue(), blueSlider->getScaledValue());
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
	int velRed, velGreen, velBlue;
	bool playRed, playGreen, playBlue;
	ofxUISlider * redSlider, * greenSlider, * blueSlider;
	float colorSpeed;
};