#pragma once
#ifndef ___Class_AdItem
#define ___Class_AdItem

#include "AppParameters.h"
#include "DrawFrame.h"

class AdItem {
private:
	static constexpr int frame_size{ 10 };

	const enum class STATE {
		NORMAL, CAHANGE
	};

	DrawFrame df, df_tmp;
	ofRectangle position;
	param::CONTENT_ID content_id, content_id_tmp;
	ofImage image, image_tmp;
	ofTrueTypeFont font;
	STATE state{ STATE::NORMAL };
	float alpha{ 0 };
public:
	void setup(const ofRectangle& position, param::CONTENT_ID content_id);
	void draw();

	void change(param::CONTENT_ID content_id);
};

#endif