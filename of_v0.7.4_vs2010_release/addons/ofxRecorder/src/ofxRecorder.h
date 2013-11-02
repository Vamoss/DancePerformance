/*
 *  ofxRecorder.h
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
#pragma once

#include "ofMain.h"

typedef struct {
	double	time;
	string	text;
} recordItem;

class ofxRecorder{
public:
	vector<recordItem>	itens;

	ofxRecorder();
        
	void				load(string filePath);
	void				save(string filePath);
	
	/// add text to be saved
	/// if time is NULL, it will receive ofGetElapsedTimeMillis
	void				add(string text);
	void				add(string text, double time);
	
	// get all items
	vector<recordItem>	getItems();

	// get saved data from the last request until _out
	vector<recordItem>	getItems(double _out);

	/// get saved data in a period time
	vector<recordItem>	getItems(double _in, double _out);
	
	double lastOutTime;
};