#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandPointer.h"

class SceneManager {
private:
	unordered_map<string, BaseScene*> scenes;
	string current_scene;
public:
	void setup(HandPointer* hp);
	void update();
	void draw();
	void pointed(pair<string, int> &id);
	~SceneManager();
};

#endif