/*
 *  ofxRecorder.cpp
 *  Vamoss
 *
 *  Created by Vamoss on 23/10/13.
 *  Copyright 2013 vamoss.com.br. No rights reserved.
 *
 *  Objective:
 *  Create a simple way to save events and reproduce then in the equivalent time that it happened
 *
 * Adapted from Patricio González
 * https://github.com/patriciogonzalezvivo/ofxSubtitles/
 *
 */
#include "ofxRecorder.h"

ofxRecorder::ofxRecorder(){
	lastOutTime = 0;
}

void ofxRecorder::load(string path){
	string line;
	ifstream myfile (path.c_str());
	if (myfile.is_open())
	{
		//clear last values
		itens.clear();

		//add new values
		int separatorPos;
		string text, timeRawString;
		while ( myfile.good() )
		{
			getline (myfile,line);
			if(line=="") continue;

			separatorPos=line.find(' ',0);

			// Extract the time
			timeRawString=line.substr(0,separatorPos);

			vector <string> inTimeRAW = ofSplitString(timeRawString, ":");
			vector <string> inPresTimeRAW = ofSplitString(timeRawString, ",");
                                
			// Extract the seconds
			float time = ofToFloat(inTimeRAW[2].c_str()) + ( ofToFloat(inPresTimeRAW[1].c_str()) )*0.001;
			// Add the minutes
			time += 60 * ofToInt(inTimeRAW[1].c_str());
			// Add the hour
			time += 60 * 60 * ofToInt(inTimeRAW[0].c_str());

			//convert to millis
			time *= 1000;

			// Extract the TEXT
			text=line.substr(separatorPos+1);
                                
			add(text, time);
			}
		myfile.close();
	}
}

void ofxRecorder::save(string path){
        ofstream fs(ofToDataPath(path).c_str());

		int inIntSeg, inS, inM, inH;
		float inMSeg;
        for (int i = 0; i < itens.size() ; i++){
                inIntSeg = (int)(itens[i].time/1000);
                inMSeg = ((int)itens[i].time)%1000;
                
                inS =   inIntSeg % 60;
                inM = ((inIntSeg - inS )/ 60 )%60;
                inH = ( inIntSeg - inS - inM * 60)/3600;
                
                fs << inH << ":" << inM << ":" << inS << "," << inMSeg << " " << itens[i].text << endl;
        }
        fs.close();
}

void ofxRecorder::add(string text){
	add(text, ofGetElapsedTimeMillis());
}

void ofxRecorder::add(string text, double time){
	recordItem tempSub;
	tempSub.time	= time;
	tempSub.text	= text;
        
	//if it is the first
	if (itens.size() <= 0) itens.push_back(tempSub);
	//if it is the last
	else if ( time >= itens[itens.size()-1].time ) itens.push_back(tempSub);
	// else
	else {
		int size = itens.size();
		int foundIndex = size;
		for(int i = size-1; i > 0 && foundIndex==size; i--){
			if ( itens[i].time >= time ) foundIndex = i;
		}
		itens.insert(itens.begin()+foundIndex,tempSub);
	}
}

vector<recordItem> ofxRecorder::getItems(){
	return itens;
}

vector<recordItem> ofxRecorder::getItems(double _out){
	return getItems(lastOutTime, _out);
}

vector<recordItem> ofxRecorder::getItems(double _in, double _out){
	lastOutTime = _out;

	vector<recordItem> itensToReturn;
        
	for(int i = 0; i < itens.size(); i++){
		if ( itens[i].time > _in && itens[i].time <= _out){
				itensToReturn.push_back(itens[i]);
		}
	}
        
	return itensToReturn;
}