#pragma once

#include "server.h"
#include "ofxUI.h"
#include "ofxRecorder.h"
#include "ofxOsc.h"
#include "UI.h"

class recorderUI : public UI {
    
public:

	bool			isMouseDown;

	enum			states {PLAYING, RECORDING, NONE};
	int				state;

	ofxUISlider *	sliderTimer;
	ofxUILabel *	labelTimer;

	ofxRecorder		recorder;
	ofSoundPlayer	player;

	double lastValue;

	vector <UI *> panels;

	string timeStamp;

    void setup()
    {
        setGUI1();
        
		player.loadSound("audio.mp3");
		player.setPosition(0);

        gui1->loadSettings("GUI/recorderSettings.xml");
        gui1->setVisible(true);
		lastValue = -1;

		state = NONE;

		ofAddListener(server::onData, this, &recorderUI::onServerData);

		timeStamp = "";
    }

	void setupPanelsToRecord(vector <UI *> panels){
		this->panels = panels;
	}
    
    void setGUI1()
    {
		gui1 = new ofxUICanvas((config::columnWidth + config::columnSpace)*3, 498, config::columnWidth, 270);
		gui1->setWidgetSpacing(config::UISpace);
		gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
		gui1->setName("RecorderUI");
		gui1->setWidgetSpacing(15);
        gui1->addWidgetDown(new ofxUILabel("RECORDER", OFX_UI_FONT_MEDIUM));
        sliderTimer = new ofxUISlider(config::UIWidth, config::UIHeight, 0.0, 1.0, 0.0, "TIMER");
		gui1->addWidgetDown(sliderTimer);
		sliderTimer->setIncrement(0.001);
        labelTimer = new ofxUILabel("00:00:00", OFX_UI_FONT_MEDIUM);
		gui1->addWidgetDown(labelTimer);
        gui1->addWidgetDown(new ofxUILabelButton( 90, false, "PLAY", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetRight(new ofxUILabelButton( 90, false, "STOP", OFX_UI_FONT_MEDIUM));
        gui1->addWidgetRight(new ofxUILabelButton( 90, false, "RECORD", OFX_UI_FONT_MEDIUM));
        
		gui1->addWidgetDown(new ofxUILabelButton( 90, false, "SAVE", OFX_UI_FONT_MEDIUM));
		gui1->addWidgetRight(new ofxUILabelButton( 90, false, "LOAD", OFX_UI_FONT_MEDIUM));
        
        ofAddListener(gui1->newGUIEvent,this,&recorderUI::guiEvent);
    }

	void update()
	{	
		if(state==PLAYING || state==RECORDING)
		{
			int currentPosition = player.getPositionMS();
			if(state==PLAYING)
			{
				vector<recordItem> itens = recorder.getItems(lastValue, currentPosition);
				for(int i = 0; i < itens.size(); i++){
					server::send(stringToOsc(itens[i].text));
				}
			}
			labelTimer->setLabel(millisecondsToTimestamp(currentPosition));
			sliderTimer->setValue(player.getPosition());
			lastValue = currentPosition;
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
        
        if(name == "PLAY" && !isMouseDown)
        {
			startPlaying();
        }
		else if(name == "STOP" && !isMouseDown)
        {
			stop();
        }
		else if(name == "RECORD" && !isMouseDown)
		{
			startRecording();
		}
		else if(name == "TIMER")
		{
			ofxUISlider *slider = (ofxUISlider *) e.widget; 
			player.setPosition(slider->getValue());
		}
		else if(name == "LOAD" && !isMouseDown)
        {
			stop();

			ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png"); 
			if (openFileResult.bSuccess){
				recorder.load(openFileResult.getPath());
			}
        }
		else if(name == "SAVE" && !isMouseDown)
        {
			stop();

			ofFileDialogResult saveFileResult = ofSystemSaveDialog("recorder_" + ofGetTimestampString() + ".txt", "Save your file");
			if (saveFileResult.bSuccess){
				recorder.save(saveFileResult.filePath);
			}
        }
	}
    
    void save()
    {
        gui1->saveSettings("GUI/recorderSettings.xml");
    }

	void mousePressed(int x, int y, int button)
	{
		isMouseDown=true;
	}

	void mouseReleased(int x, int y, int button)
	{
		isMouseDown=false;
	}
    
	void onServerData(ofxOscMessage & message)
	{
		if(state==RECORDING) {
			//cout << (lastValue > 0 ? player.getPositionMS() : 0) << endl;
			recorder.add(oscToString(message), lastValue > 0 ? player.getPositionMS() : 0);
		}else{
			//cout << (lastValue > 0 ? player.getPositionMS() : 0) << oscToString(message) << endl;
		}
	}
    
	string oscToString(ofxOscMessage message)
	{
		string encoded;
		string separator = "||";
		encoded = message.getAddress();
		for ( int i=0; i< message.getNumArgs(); ++i )
		{
			encoded += separator;
			if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
				encoded += "i" + separator + ofToString(message.getArgAsInt32( i ));
			else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
				encoded += "l" + separator + ofToString(message.getArgAsInt64( i ));
			else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
				encoded += "f" + separator + ofToString(message.getArgAsFloat( i ));
			else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
				encoded += "s" + separator + message.getArgAsString( i );
		}
		return encoded;
	}
    
	ofxOscMessage stringToOsc(string message)
	{
		string separator = "||";
		vector <string> decoded = ofSplitString(message, separator);
		ofxOscMessage ofMessage;

		ofMessage.setAddress( decoded[0] );
		for ( int i=1; i<decoded.size(); i+=2)
		{
			if ( decoded[i]=="i" )
				ofMessage.addIntArg( ofToInt(decoded[i+1]) );
			else if ( decoded[i]=="l" )
				ofMessage.addInt64Arg( ofToInt(decoded[i+1]) );
			else if ( decoded[i]=="f" )
				ofMessage.addFloatArg( ofToFloat(decoded[i+1]) );
			else if ( decoded[i]=="s" )
				ofMessage.addStringArg( decoded[i+1] );
		}

		return ofMessage;
	}

	void startPlaying()
	{
		state = PLAYING;
		player.play();
		lastValue = -1;
	}

	void startRecording(){
		recorder.itens.clear();
		state = RECORDING;
		lastValue = -1;

		//record the first state of all panels
		//extracted from ofxUICanvas.loadSpecificWidgetData
		for(int i=0; i<panels.size(); i++){
			if(panels[i] != (UI *)this){
				vector<ofxUIWidget*> widgets = panels[i]->gui1->getWidgets();
				ofxUIWidget *widget;
				
				for(int j = 0; j < widgets.size(); j++)
				{
					widget = widgets[j];
					
					//ignore mapping widgets
					if(widget->getName()=="Y" || widget->getName()=="SCALE") continue;

					int kind = widget->getKind();        
					switch (kind) 
					{
						case OFX_UI_WIDGET_IMAGETOGGLE:    
						case OFX_UI_WIDGET_MULTIIMAGETOGGLE: 
						case OFX_UI_WIDGET_LABELTOGGLE:                
						case OFX_UI_WIDGET_TOGGLE:
						{
							server::send(widget->getName(), ((ofxUIToggle *)widget)->getValue());
						}
							break;

						case OFX_UI_WIDGET_MULTIIMAGESLIDER_H:
						case OFX_UI_WIDGET_MULTIIMAGESLIDER_V:                 
						case OFX_UI_WIDGET_IMAGESLIDER_H:
						case OFX_UI_WIDGET_IMAGESLIDER_V:                
						case OFX_UI_WIDGET_BILABELSLIDER:    
						case OFX_UI_WIDGET_CIRCLESLIDER:               
						case OFX_UI_WIDGET_MINIMALSLIDER:
						case OFX_UI_WIDGET_SLIDER_H:
						case OFX_UI_WIDGET_SLIDER_V:
						{
							server::send(widget->getName(), ((ofxUISlider *)widget)->getScaledValue());
						}
							break;
                
						case OFX_UI_WIDGET_RSLIDER_H:
						case OFX_UI_WIDGET_RSLIDER_V:
						{
							server::send(widget->getName(), ((ofxUIRangeSlider *)widget)->getScaledValueLow(), ((ofxUIRangeSlider *)widget)->getScaledValueHigh());
						}
							break;
                
						case OFX_UI_WIDGET_NUMBERDIALER:
						{
							server::send(widget->getName(), ((ofxUINumberDialer *)widget)->getValue());
						}
							break;
                
						case OFX_UI_WIDGET_2DPAD:
						{
							server::send(widget->getName(), ((ofxUI2DPad *)widget)->getScaledValue().x, ((ofxUI2DPad *)widget)->getScaledValue().y);
						}
							break;
                
						case OFX_UI_WIDGET_TEXTINPUT:
						{
							server::send(widget->getName(), ((ofxUITextInput *)widget)->getTextString());
						}
							break;                
                
						case OFX_UI_WIDGET_ROTARYSLIDER:
						{
							server::send(widget->getName(), ((ofxUIRotarySlider *)widget)->getScaledValue());
						}
							break;
                
						case OFX_UI_WIDGET_IMAGESAMPLER:
						{
							//ignore
						}
							break;
                
						default:
							break;
					}
				}
			}
		}
		player.play();
	}

	void stop()
	{
		state = NONE;
		player.stop();
		lastValue = -1;
	}
	
	int inIntSeg, inS, inM, inH;
	float inMSeg;
	string millisecondsToTimestamp(int millis)
	{
            inIntSeg = (int)(millis/1000);
            inMSeg = ((int)millis)%1000;
                
            inS =   inIntSeg % 60;
            inM = ((inIntSeg - inS )/ 60 )%60;
            inH = ( inIntSeg - inS - inM * 60)/3600;
                
            return ofToString(inH) + ":" + ofToString(inM) + ":" + ofToString(inS) + "," + ofToString(inMSeg);
	}
};
