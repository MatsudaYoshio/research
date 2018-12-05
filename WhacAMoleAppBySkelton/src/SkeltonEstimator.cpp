#include "SkeltonEstimator.h"

using namespace std;
using namespace cv;
using namespace param;

/* Scalar型の色 */
const Scalar SkeltonEstimator::CV_RED{ Scalar(0, 0, 255) };
const Scalar SkeltonEstimator::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar SkeltonEstimator::CV_GREEN{ Scalar(0, 255, 0) };
const Scalar SkeltonEstimator::CV_ORANGE{ Scalar(76, 183, 255) };
const Scalar SkeltonEstimator::CV_PURPLE{ Scalar(204, 0, 196) };

SkeltonEstimator::SkeltonEstimator() {
	this->image_buffer.emplace_front(this->cap.get_image()); // 最初の一枚を取り込んでおく

	/* 画像取得スレッド */
	thread frame_thread(&SkeltonEstimator::get_frame, this);
	frame_thread.detach();

	/* 動画撮影 */
	//this->writer.open("OpenPose_test.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 14, Size(CAMERA_WIDTH, CAMERA_HEIGHT), true);
}

void SkeltonEstimator::update() {
	///* fpsをコマンドラインに出力 */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	this->body_part_extractor(this->raw_pose_key_points, this->image_buffer.front()); // openPoseによる骨格推定(結果をraw_pose_key_pointsに保存)
	this->transform_points(); // 骨格点をディスプレイ座標に変換

	//this->show_detect_window(); // 動作確認用のウィンドウを表示
}

void SkeltonEstimator::exit() {
	this->stop_flag = true;
}

void SkeltonEstimator::get_frame() {
	while (!this->stop_flag) {
		this->image_buffer.emplace_front(this->cap.get_image());
	}
}

void SkeltonEstimator::show_detect_window() {
	this->view_frame = this->image_buffer.front(); // 最新フレームをコピー

	const int people_num{ this->raw_pose_key_points.getSize(0) }; // 検出された人数
	Concurrency::parallel_for(0, people_num, [&](int i) {
		Concurrency::parallel_for(0, BODY25_KEY_POINTS_NUM, [&](int j) {
			if (this->raw_pose_key_points[{i, j, 0}] != 0.0 && this->raw_pose_key_points[{i, j, 1}] != 0.0) {
				cv::circle(this->view_frame, Point(this->raw_pose_key_points[{i, j, 0}], this->raw_pose_key_points[{i, j, 1}]), 12, this->CV_RED, -1);
			}
		});
	});

	imshow("detect window", this->view_frame);

	/* 動画撮影 */
	//this->writer << this->view_frame;
}

void SkeltonEstimator::transform_points() {
	this->transformed_key_points.clear();

	this->people_num = this->raw_pose_key_points.getSize(0);
	for (int i = 0; i < this->people_num; ++i) {
		for (int j = 0; j < BODY25_KEY_POINTS_NUM; ++j) {
			if (this->raw_pose_key_points[{ i, j, 0 }] != 0.0 && this->raw_pose_key_points[{ i, j, 1 }] != 0.0) {
				this->transformed_key_points.emplace(make_pair(i, j), ofPoint(DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*this->raw_pose_key_points[{ i, j, 0 }], RESOLUTION_RATIO_HEIGHT*this->raw_pose_key_points[{ i, j, 1 }]));
			}
		}
	}
}
