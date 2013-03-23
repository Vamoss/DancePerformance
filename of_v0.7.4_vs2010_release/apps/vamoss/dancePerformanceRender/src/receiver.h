#pragma once

#include "ofMain.h"
#include "facade.h"
#include "ofxOsc.h"

class receiver {
public:
	

	facade			*app;

	ofxOscReceiver oscReceiver;
	int port;

	receiver()
	{

	}

	void setup(facade *app, int port)
	{
		 this->app = app;
		this->port = port;

		oscReceiver.setup(port);
	}


	void update()
	{
	
		// check for waiting messages
		while(oscReceiver.hasWaitingMessages()){
			// get the next message
			ofxOscMessage m;
			oscReceiver.getNextMessage(&m);
			cout << m.getAddress() << endl;

			/*// check for mouse moved message
			if(m.getAddress() == "/mouse/position"){
				// both the arguments are int32's
				//mouseX = m.getArgAsInt32(0);
				//mouseY = m.getArgAsInt32(1);
			}
			// check for mouse button message
			else if(m.getAddress() == "/mouse/button"){
				// the single argument is a string
				//mouseButtonState = m.getArgAsString(0);
			}
			else{
				// unrecognized message: display on the bottom of the screen
				string msg_string;
				msg_string = m.getAddress();
				msg_string += ": ";
				for(int i = 0; i < m.getNumArgs(); i++){
					// get the argument type
					msg_string += m.getArgTypeName(i);
					msg_string += ":";
					// display the argument - make sure we get the right type
					if(m.getArgType(i) == OFXOSC_TYPE_INT32){
						msg_string += ofToString(m.getArgAsInt32(i));
					}
					else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
						msg_string += ofToString(m.getArgAsFloat(i));
					}
					else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
						msg_string += m.getArgAsString(i);
					}
					else{
						msg_string += "unknown";
					}
				}
				// add to the list of strings to display
				/*msg_strings[current_msg_string] = msg_string;
				timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
				current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
				// clear the next line
				msg_strings[current_msg_string] = "";*/
			//}

		}
	}
	
	
	void onData(string message)
	{
		cout << "onData: " << message << endl;

	/*

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
		else if(name == "PROBABILITY")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->probability = slider->getScaledValue();
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
		else if(name == "ORBIT")
		{
			ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget; 
			app->setOrbit(slider->getScaledValueLow(), slider->getScaledValueHigh());
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
		else if(name == "BLACKOUT")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->blackout = slider->getScaledValue();
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
		else if(name == "CENTER")
		{
			app->rotSpeed = 0; 
			sliderRot->setValue(0);
		}
		else if(name == "MASS")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			app->setMass(slider->getScaledValue()); 
		}





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

	*/
	}
};