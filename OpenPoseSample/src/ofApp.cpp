#include "ofApp.h"

using namespace param;

void ofApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofSetCircleResolution(256); // 円の精密度を設定

	this->kpr.setup(&this->hc.pose_key_points); // 骨格描画クラスの準備
}

void ofApp::update() {
	this->hc.update(); // 手カーソルの更新
}

void ofApp::draw() {
	// それぞれのコメントを外して確認してください

	/* 右手の操作のためのポインタの描画 */
	this->draw_pointer();

	/* 骨格の描画 */
	//this->kpr.draw();

	/* ユーザデータの表示 */
	//this->show_user_data();

	/* 生の（カメラ座標上の）骨格点情報を表示 */
	//this->show_key_points(this->hc.pose_key_points);

	///* ディスプレイ座標上の骨格点情報を表示 */
	//op::Array<float> new_key_points = this->hc.pose_key_points; // 新たな骨格点を入れる変数を用意
	//this->transform_key_points(new_key_points); // 変換する。手カーソルクラスのpose_key_points（カメラ座標上の骨格点）をディスプレイ座標に変換した骨格点を得る（new_key_pointsに代入）
	//this->show_key_points(new_key_points); // 表示
}

void ofApp::exit() {
	this->hc.exit(); // アプリ終了時に手カーソルクラスのexit関数を呼び出す
}

void ofApp::draw_pointer() const {
	for (const auto& ud : this->hc.user_data) {
		if (ud.second.state == HandCursor::STATE::INACTIVE) { // 状態がINACTIVEのとき、描画しないようにする（ユーザが手を下げると状態がINACTIVEになるから）。もし、手を下げているときも描画したい場合はこのif文を消す
			continue;
		}

		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 50);
		ofFill();
		ofSetColor(ud.second.cursor_color);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 45);
	}
}

void ofApp::show_key_points(const op::Array<float>& key_points) const {
	const auto people_num{ key_points.getSize(0) }; // 検出された人数

	for (int i = 0; i < people_num; ++i) {
		printf("user%d's key points:\n", i);

		///* 全部の骨格点を表示 */
		//for (int j = 0; j < 18; ++j) {
		//	if (key_points[{i, j, 0}] != 0.0 && key_points[{i, j, 1}] != 0.0) {
		//		printf("key point [%d] -> (%f,%f)\n", j, key_points[{i, j, 0}], key_points[{i, j, 1}]);
		//	}
		//}

		///* 特定の骨格点だけ表示（例えば右手首） */
		//if (key_points[RIGHT_WRIST_X(i)] != 0.0 && key_points[RIGHT_WRIST_Y(i)] != 0.0) {
		//	printf("key point [right wrist] -> (%f,%f)\n", key_points[RIGHT_WRIST_X(i)], key_points[RIGHT_WRIST_Y(i)]);
		//}

		/* 特定の骨格点だけ表示（例えば左目）*/
		if (key_points[LEFT_EYE_X(i)] != 0.0 && key_points[LEFT_EYE_Y(i)] != 0.0) {
			printf("key point [left eye] -> (%f,%f)\n", key_points[LEFT_EYE_X(i)], key_points[LEFT_EYE_Y(i)]);
		}

		puts("");
	}
}

void ofApp::show_user_data() const {
	puts("Show user data!!");
	for (const auto& ud : this->hc.user_data) {
		puts("------------------------------------");
		printf("user id : %lld\n", ud.first); // ユーザID
		printf("color id : %d\n", ud.second.cursor_color_id); // ユーザに割り当てられた色のID
		printf("image id : %d\n", ud.second.cursor_image_id); // ユーザに割り当てられた画像のID
		printf("cursor point : (%d, %d)\n", ud.second.cursor_point.x, ud.second.cursor_point.y); // カーソルの座標（右手首の座標を元にディスプレイ座標上に変換されたもの）
		printf("face point : (%d, %d)\n", ud.second.face_point.x, ud.second.face_point.y); // 顔の中心の座標（カメラ座標で、ディスプレイ座標上に変換されていないもの）
		printf("transformed face point : (%d, %d)\n", ud.second.transformed_face_point.x, ud.second.transformed_face_point.y); // 顔の中心の座標（ディスプレイ座標上に変換されたもの）
		printf("face size : %lf\n", ud.second.face_size); // 推定した顔の大きさ
		puts("------------------------------------");
		puts("");
	}
}

void ofApp::transform_key_points(op::Array<float>& key_points) const {
	const auto people_num{ this->hc.pose_key_points.getSize(0) }; // 検出された人数

	/* すべての骨格点をディスプレイ座標へと変換する */
	for (int i = 0; i < people_num; ++i) {
		for (int j = 0; j < 18; ++j) {
			if (this->hc.pose_key_points[{ i, j, 0 }] != 0.0 && this->hc.pose_key_points[{ i, j, 1 }] != 0.0) {
				key_points[{i, j, 0}] = DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*key_points[{ i, j, 0 }]; // x座標はこうやって変換する（x' = DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*x）
				key_points[{i, j, 1}] = RESOLUTION_RATIO_HEIGHT*key_points[{ i, j, 1 }]; // y座標はこうやって変換する（y' = RESOLUTION_RATIO_HEIGHT*y）
			}
		}
	}
}