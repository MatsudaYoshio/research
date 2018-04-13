#pragma once
#ifndef ___Class_Pin
#define ___Class_Pin

#include "ofMain.h"
#include "AppParameters.h"

class Pin {
private:
	using triangle_points = struct {
		ofPoint p1, p2, p3;
	};

	static constexpr double size_ratio{ 0.35 };
	static constexpr double radius{ 45 };
	static constexpr double big_radius{ 80 };
	static constexpr int font_size{ 40 };
	static constexpr int big_font_size{ 70 };
	static constexpr int progress_threshold{ 100 };

	const enum class STATE {
		NORMAL, POINT
	};

	STATE state{ STATE::NORMAL };
	int pin_number;
	param::CONTENT_ID content_id;
	triangle_points tp, big_tp;
	ofPoint cp, big_cp, fp, big_fp;
	ofTrueTypeFont font, big_font;
	ofColor color;
	float alpha{ ofColor::limit() };
	int progress{ 0 };

	void draw(const ofPoint& cp, int r, const triangle_points& tp, const ofTrueTypeFont& f, const ofPoint& fp) const;
public:
	void setup(param::CONTENT_ID content_id);
	void update();
	void draw() const;

	bool is_inside(int x, int y);
	void point();
	void reset_state();
};

#endif