#include "KeyPointsRenderer.h"

using namespace param;

/*
   それぞれの骨格点の描画設定（順番はCOCOモデルの順番）

   それぞれで表示するかしないかでtrueかfalse
   色と透明度を指定できる

   例えば、
   make_pair(true, ofColor(ofColor::deepPink, 100)),
   だったら、この骨格点をofColor::deepPinkの色で、アルファ値100で表示する
*/
const array<pair<bool, ofColor>, KeyPointsRenderer::COCO_key_points_num> KeyPointsRenderer::points_state = {
	/* (0)Nose */ make_pair(true, ofColor(ofColor::red, 180)),
	/* (1)Neck */ make_pair(true, ofColor(ofColor::orangeRed, 180)),
	/* (2)RShoulder */ make_pair(true, ofColor(ofColor::goldenRod, 180)),
	/* (3)RElbow */ make_pair(true, ofColor(ofColor::yellow, 180)),
	/* (4)RWrist */ make_pair(true, ofColor(ofColor::greenYellow, 180)),
	/* (5)LShoulder */ make_pair(true, ofColor(ofColor::lightGreen, 180)),
	/* (6)LElbow */ make_pair(true, ofColor(ofColor::green, 180)),
	/* (7)LWrist */ make_pair(true, ofColor(ofColor::lightSeaGreen, 180)),
	/* (8)RHip */ make_pair(true, ofColor(ofColor::skyBlue, 180)),
	/* (9)RKnee */ make_pair(true, ofColor(ofColor::blueViolet, 180)),
	/* (10)RAnkle */ make_pair(true, ofColor(ofColor::blueSteel, 180)),
	/* (11)LHip */ make_pair(true, ofColor(ofColor::blue, 180)),
	/* (12)LKnee */ make_pair(true, ofColor(ofColor::darkBlue, 180)),
	/* (13)LAnkle */ make_pair(true, ofColor(ofColor::darkRed, 180)),
	/* (14)REye */ make_pair(false, ofColor(ofColor::purple, 180)),
	/* (15)LEye */ make_pair(false, ofColor(ofColor::pink, 180)),
	/* (16)REar */ make_pair(false, ofColor(ofColor::deepPink, 180)),
	/* (17)LEar */ make_pair(false, ofColor(ofColor::dodgerBlue, 180))
};

/*
それぞれの骨格点の描画設定（順番はkey_points_pairsで定義した順）
設定項目は骨格点と同様
*/
const array<pair<bool, ofColor>, KeyPointsRenderer::COCO_key_points_pairs_num> KeyPointsRenderer::lines_state = {
	/* (0)Nose - (1)Neck */ make_pair(true, ofColor(ofColor::blue, 100)),
	/* (0)Nose - (14)REye */ make_pair(false, ofColor(ofColor::blue, 100)),
	/* (0)Nose - (15)LEye */ make_pair(false, ofColor(ofColor::purple, 100)),
	/* (1)Neck - (2)RShoulder */ make_pair(true, ofColor(ofColor::red, 100)),
	/* (1)Neck - (5)LShoulder */ make_pair(true, ofColor(ofColor::brown, 100)),
	/* (1)Neck - (8)RHip */ make_pair(true, ofColor(ofColor::lightGreen, 100)),
	/* (1)Neck - (11)LHip */ make_pair(true, ofColor(ofColor::skyBlue, 100)),
	/* (2)RShoulder - (3)RElbow */ make_pair(true, ofColor(ofColor::goldenRod, 100)),
	/* (3)RElbow - (4)RWrist */ make_pair(true, ofColor(ofColor::yellow , 100)),
	/* (5)LShoulder - (6)LElbow */ make_pair(true, ofColor(ofColor::greenYellow, 100)),
	/* (6)LElbow - (7)LWrist */ make_pair(true, ofColor(ofColor::green, 100)),
	/* (8)RHip - (9)RKnee */ make_pair(true, ofColor(ofColor::lightSeaGreen, 100)),
	/* (9)RKnee - (10)RAnkle */ make_pair(true, ofColor(ofColor::lightBlue, 100)),
	/* (11)LHip - (12)LKnee */ make_pair(true, ofColor(ofColor::cadetBlue, 100)),
	/* (12)LKnee - (13)LAnkle */ make_pair(true, ofColor(ofColor::blue, 100)),
	/* (14)REye - (16)REar */ make_pair(false, ofColor(ofColor::purple, 100)),
	/* (15)LEye - (17)LEar */ make_pair(false, ofColor(ofColor::deepPink, 100))
};

const array<pair<int, int>, KeyPointsRenderer::COCO_key_points_pairs_num> KeyPointsRenderer::key_points_pairs = {
	make_pair(0, 1),
	make_pair(0, 14),
	make_pair(0, 15),
	make_pair(1, 2),
	make_pair(1, 5),
	make_pair(1, 8),
	make_pair(1, 11),
	make_pair(2, 3),
	make_pair(3, 4),
	make_pair(5, 6),
	make_pair(6, 7),
	make_pair(8, 9),
	make_pair(9, 10),
	make_pair(11, 12),
	make_pair(12, 13),
	make_pair(14, 16),
	make_pair(15, 17)
};

void KeyPointsRenderer::setup(op::Array<float>* const raw_key_points) {
	ofSetLineWidth(this->line_size);
	this->raw_key_points = raw_key_points;
}

void KeyPointsRenderer::draw() {
	this->transform_points();

	for (int i = 0; i < this->people_num; ++i) {
		/* 骨格の線分を描画 */
		for (int j = 0; j < this->COCO_key_points_pairs_num; ++j) {
			if (this->lines_state[j].first) {
				ofSetColor(this->lines_state[j].second);
				try {
					ofDrawLine(this->transformed_key_points.at({ i, this->key_points_pairs[j].first }), this->transformed_key_points.at({ i,this->key_points_pairs[j].second }));
				}
				catch (std::out_of_range&) {}
			}
		}

		/* 骨格点を描画 */
		for (int j = 0; j < this->COCO_key_points_num; ++j) {
			if (this->points_state[j].first) {
				ofSetColor(this->points_state[j].second);
				try {
					ofDrawCircle(this->transformed_key_points.at({ i, j }), this->point_size);
				}
				catch (std::out_of_range&) {}
			}
		}
	}
}

void KeyPointsRenderer::draw(const map<pair<int, int>, ofPoint>& transformed_key_points) const {
	for (const auto& p : transformed_key_points) {
		/* 骨格の線分を描画 */
		for (int j = 0; j < this->COCO_key_points_pairs_num; ++j) {
			if (this->lines_state[j].first) {
				ofSetColor(this->lines_state[j].second);
				try {
					ofDrawLine(transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }), transformed_key_points.at({ p.first.first,this->key_points_pairs[j].second }));
				}
				catch (std::out_of_range&) {}
			}
		}

		/* 骨格点を描画 */
		for (int j = 0; j < this->COCO_key_points_num; ++j) {
			if (this->points_state[j].first) {
				ofSetColor(this->points_state[j].second);
				try {
					ofDrawCircle(transformed_key_points.at({ p.first.first, j }), this->point_size);
				}
				catch (std::out_of_range&) {}
			}
		}
	}
}

void KeyPointsRenderer::transform_points() {
	this->transformed_key_points.clear();

	this->people_num = this->raw_key_points->getSize(0);
	for (int i = 0; i < this->people_num; ++i) {
		for (int j = 0; j < this->COCO_key_points_num; ++j) {
			if (this->raw_key_points->at({ i, j, 0 }) != 0.0 && this->raw_key_points->at({ i, j, 1 }) != 0.0) {
				this->transformed_key_points.emplace(make_pair(i, j), ofPoint(DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*this->raw_key_points->at({ i, j, 0 }), RESOLUTION_RATIO_HEIGHT * this->raw_key_points->at({ i, j, 1 })));
			}
		}
	}
}