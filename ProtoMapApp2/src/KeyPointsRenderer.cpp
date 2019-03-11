#include "KeyPointsRenderer.h"

using namespace param;

/*
それぞれの骨格点の描画設定（BODY25モデル）

それぞれで表示するかしないかでtrueかfalse
色と透明度を指定できる

例えば、
make_pair(true, ofColor(ofColor::deepPink, 100)),
だったら、この骨格点をofColor::deepPinkの色で、アルファ値100で表示する
*/
const array<pair<bool, ofColor>, KeyPointsRenderer::BODY25_key_points_num> KeyPointsRenderer::points_state = {
	/* (0)Nose */ make_pair(true, ofColor(ofColor::mediumVioletRed, 180)),
	/* (1)Neck */ make_pair(true, ofColor(ofColor::red, 180)),
	/* (2)RShoulder */ make_pair(true, ofColor(ofColor::orangeRed, 180)),
	/* (3)RElbow */ make_pair(true, ofColor(ofColor::goldenRod, 180)),
	/* (4)RWrist */ make_pair(true, ofColor(ofColor::yellow, 180)),
	/* (5)LShoulder */ make_pair(true, ofColor(ofColor::chartreuse, 180)),
	/* (6)LElbow */ make_pair(true, ofColor(ofColor::lime, 180)),
	/* (7)LWrist */ make_pair(true, ofColor(ofColor::green, 180)),
	/* (8)MidHip */ make_pair(true, ofColor(ofColor::red, 180)),
	/* (9)RHip */ make_pair(true, ofColor(ofColor::mediumSpringGreen, 180)),
	/* (10)RKnee */ make_pair(true, ofColor(ofColor::aqua, 180)),
	/* (11)RAnkle */ make_pair(true, ofColor(ofColor::cyan, 180)),
	/* (12)LHip */ make_pair(true, ofColor(ofColor::dodgerBlue, 180)),
	/* (13)LKnee */ make_pair(true, ofColor(ofColor::royalBlue, 180)),
	/* (14)LAnkle */ make_pair(true, ofColor(ofColor::blue, 180)),
	/* (15)REye */ make_pair(false, ofColor(ofColor::deepPink, 180)),
	/* (16)LEye */ make_pair(false, ofColor(ofColor::purple, 180)),
	/* (17)REar */ make_pair(false, ofColor(ofColor::fuchsia, 180)),
	/* (18)LEar */ make_pair(false, ofColor(ofColor::blueViolet, 180)),
	/* (19)LBigToe */ make_pair(false, ofColor(ofColor::blue, 180)),
	/* (20)LSmallToe */ make_pair(false, ofColor(ofColor::blue, 180)),
	/* (21)LHeel */ make_pair(false, ofColor(ofColor::blue, 180)),
	/* (22)RBigToe */ make_pair(false, ofColor(ofColor::cyan, 180)),
	/* (23)RSmallToe */ make_pair(false, ofColor(ofColor::cyan, 180)),
	/* (24)RHeel */ make_pair(false, ofColor(ofColor::cyan, 180))
};

/*
それぞれの骨格点の描画設定（順番はkey_points_pairsで定義した順）
設定項目は骨格点と同様
*/
const array<pair<bool, ofColor>, KeyPointsRenderer::BODY25_key_points_pairs_num> KeyPointsRenderer::lines_state = {
	/* (0)Nose - (1)Neck */ make_pair(true, ofColor(ofColor::mediumVioletRed, 100)),
	/* (0)Nose - (15)REye */ make_pair(false, ofColor(ofColor::deepPink, 100)),
	/* (0)Nose - (16)LEye */ make_pair(false, ofColor(ofColor::purple, 100)),
	/* (1)Neck - (2)RShoulder */ make_pair(true, ofColor(ofColor::orangeRed, 100)),
	/* (1)Neck - (5)LShoulder */ make_pair(true, ofColor(ofColor::chartreuse, 100)),
	/* (1)Neck - (8)MidHip */ make_pair(true, ofColor(ofColor::red, 100)),
	/* (2)RShoulder - (3)RElbow */ make_pair(true, ofColor(ofColor::goldenRod, 100)),
	/* (3)RElbow - (4)RWrist */ make_pair(true, ofColor(ofColor::yellow , 100)),
	/* (5)LShoulder - (6)LElbow */ make_pair(true, ofColor(ofColor::lime, 100)),
	/* (6)LElbow - (7)LWrist */ make_pair(true, ofColor(ofColor::green, 100)),
	/* (8)MidHip - (9)RHip */ make_pair(true, ofColor(ofColor::mediumSpringGreen, 100)),
	/* (8)MidHip - (12)LHip */ make_pair(true, ofColor(ofColor::dodgerBlue, 100)),
	/* (9)RHip - (10)RKnee */ make_pair(true, ofColor(ofColor::aqua, 100)),
	/* (10)RKnee - (11)RAnkle */ make_pair(true, ofColor(ofColor::cyan, 100)),
	/* (11)RAnkle - (22)RBigToe */ make_pair(false, ofColor(ofColor::cyan, 100)),
	/* (11)RAnkle - (24)RHeel */ make_pair(false, ofColor(ofColor::cyan, 100)),
	/* (12)LHip - (13)LKnee */ make_pair(true, ofColor(ofColor::royalBlue, 100)),
	/* (13)LKnee - (14)LAnkle */ make_pair(true, ofColor(ofColor::blue, 100)),
	/* (14)LAnkle - (19)LBigToe */ make_pair(false, ofColor(ofColor::blue, 100)),
	/* (14)LAnkle - (21)LHeel */ make_pair(false, ofColor(ofColor::blue, 100)),
	/* (15)REye - (17)REar */ make_pair(false, ofColor(ofColor::fuchsia, 100)),
	/* (16)LEye - (18)LEar */ make_pair(false, ofColor(ofColor::blueViolet, 100)),
	/* (19)LBigToe - (20)LSmallToe */ make_pair(false, ofColor(ofColor::blue, 100)),
	/* (22)RBigToe - (23)RSmallToe */ make_pair(false, ofColor(ofColor::cyan, 100))
};

const array<pair<int, int>, KeyPointsRenderer::BODY25_key_points_pairs_num> KeyPointsRenderer::key_points_pairs = {
	make_pair(0, 1),
	make_pair(0, 15),
	make_pair(0, 16),
	make_pair(1, 2),
	make_pair(1, 5),
	make_pair(1, 8),
	make_pair(2, 3),
	make_pair(3, 4),
	make_pair(5, 6),
	make_pair(6, 7),
	make_pair(8, 9),
	make_pair(8, 12),
	make_pair(9, 10),
	make_pair(10, 11),
	make_pair(11, 22),
	make_pair(11, 24),
	make_pair(12, 13),
	make_pair(13, 14),
	make_pair(14, 19),
	make_pair(14, 21),
	make_pair(15, 17),
	make_pair(16, 18),
	make_pair(19, 20),
	make_pair(22, 23)
};

void KeyPointsRenderer::setup(HandCursor* hc) {
	ofSetLineWidth(this->line_size);
	this->hc = hc;
	this->raw_key_points = &hc->pose_key_points;
}

void KeyPointsRenderer::draw() {
	this->transform_points();

	for (int i = 0; i < this->people_num; ++i) {
		if (this->hc->personal_id2user_id[i] == NOT_USER) {
			/* 骨格の線分を描画 */
			for (int j = 0; j < this->BODY25_key_points_pairs_num; ++j) {
				if (this->lines_state[j].first) {
					ofSetColor(this->lines_state[j].second);
					try {
						ofPushMatrix();
						ofTranslate((this->transformed_key_points.at({ i, this->key_points_pairs[j].first }) + this->transformed_key_points.at({ i, this->key_points_pairs[j].second })) / 2);
						ofRotate(atan2(this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).y - this->transformed_key_points.at({ i, this->key_points_pairs[j].second }).y, this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).x - this->transformed_key_points.at({ i, this->key_points_pairs[j].second }).x) * 180 / M_PI);
						ofDrawEllipse(0, 0, ofDist(this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).x, this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).y, this->transformed_key_points.at({ i,this->key_points_pairs[j].second }).x, this->transformed_key_points.at({ i,this->key_points_pairs[j].second }).y), this->line_size);
						ofPopMatrix();
					}
					catch (std::out_of_range&) {}
				}
			}

			/* 骨格点を描画 */
			for (int j = 0; j < this->BODY25_key_points_num; ++j) {
				if (this->points_state[j].first) {
					ofSetColor(this->points_state[j].second);
					try {
						ofDrawCircle(this->transformed_key_points.at({ i, j }), this->point_size);
					}
					catch (std::out_of_range&) {}
				}
			}
		}
		else {
			/* 骨格の線分を描画 */
			for (int j = 0; j < this->BODY25_key_points_pairs_num; ++j) {
				if (this->lines_state[j].first) {
					ofSetColor(this->hc->user_data[this->hc->personal_id2user_id[i]].cursor_color);
					try {
						ofPushMatrix();
						ofTranslate((this->transformed_key_points.at({ i, this->key_points_pairs[j].first }) + this->transformed_key_points.at({ i, this->key_points_pairs[j].second })) / 2);
						ofRotate(atan2(this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).y - this->transformed_key_points.at({ i, this->key_points_pairs[j].second }).y, this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).x - this->transformed_key_points.at({ i, this->key_points_pairs[j].second }).x) * 180 / M_PI);
						ofDrawEllipse(0, 0, ofDist(this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).x, this->transformed_key_points.at({ i, this->key_points_pairs[j].first }).y, this->transformed_key_points.at({ i,this->key_points_pairs[j].second }).x, this->transformed_key_points.at({ i,this->key_points_pairs[j].second }).y), this->line_size);
						ofPopMatrix();
					}
					catch (std::out_of_range&) {}
				}
			}

			/* 骨格点を描画 */
			for (int j = 0; j < this->BODY25_key_points_num; ++j) {
				if (this->points_state[j].first) {
					ofSetColor(this->hc->user_data[this->hc->personal_id2user_id[i]].cursor_color);
					try {
						ofDrawCircle(this->transformed_key_points.at({ i, j }), this->point_size);
					}
					catch (std::out_of_range&) {}
				}
			}
		}
		
	}
}

//void KeyPointsRenderer::draw(const map<pair<int, int>, ofPoint>& transformed_key_points) const {
//	for (const auto& p : transformed_key_points) {
//		/* 骨格の線分を描画 */
//		for (int j = 0; j < this->BODY25_key_points_pairs_num; ++j) {
//			if (this->lines_state[j].first) {
//				ofSetColor(this->lines_state[j].second);
//				try {
//					ofPushMatrix();
//					ofTranslate((transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }) + transformed_key_points.at({ p.first.first, this->key_points_pairs[j].second })) / 2);
//					ofRotate(atan2(transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }).y - transformed_key_points.at({ p.first.first, this->key_points_pairs[j].second }).y, transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }).x - transformed_key_points.at({ p.first.first, this->key_points_pairs[j].second }).x) * 180 / M_PI);
//					ofDrawEllipse(0, 0, ofDist(transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }).x, transformed_key_points.at({ p.first.first, this->key_points_pairs[j].first }).y, transformed_key_points.at({ p.first.first,this->key_points_pairs[j].second }).x, transformed_key_points.at({ p.first.first,this->key_points_pairs[j].second }).y), this->line_size);
//					ofPopMatrix();
//				}
//				catch (std::out_of_range&) {}
//			}
//		}
//
//		/* 骨格点を描画 */
//		for (int j = 0; j < this->BODY25_key_points_num; ++j) {
//			if (this->points_state[j].first) {
//				ofSetColor(this->points_state[j].second);
//				try {
//					ofDrawCircle(transformed_key_points.at({ p.first.first, j }), this->point_size);
//				}
//				catch (std::out_of_range&) {}
//			}
//		}
//	}
//}

void KeyPointsRenderer::transform_points() {
	this->transformed_key_points.clear();

	this->people_num = this->raw_key_points->getSize(0);
	for (int i = 0; i < this->people_num; ++i) {
		for (int j = 0; j < this->BODY25_key_points_num; ++j) {
			if (this->raw_key_points->at({ i, j, 0 }) != 0.0 && this->raw_key_points->at({ i, j, 1 }) != 0.0) {
				this->transformed_key_points.emplace(make_pair(i, j), ofPoint(DISPLAY_W - RESOLUTION_RATIO_W*this->raw_key_points->at({ i, j, 0 }), RESOLUTION_RATIO_H * this->raw_key_points->at({ i, j, 1 })));
			}
		}
	}
}