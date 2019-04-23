#pragma once
#ifndef ___Class_MapApp
#define ___Class_MapApp

#include "HandCursor.h"
#include "SceneManager.h"

class MapApp : public ofBaseApp {
private:
	HandCursor hc;
	SceneManager sm;
	
	ofstream ofs;
	string left_user_name, right_user_name;

	void initialize_image();
	void initialize_output_data_file();
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void exit();

	void write_data(const string& data);
};

#endif