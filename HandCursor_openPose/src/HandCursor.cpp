#include "HandCursor.h"
#include <stdexcept>
#include <math.h>
#include <ctime>

using namespace std;
using namespace dlib;
using namespace cv;
using namespace param;

const char* HandCursor::model_path = "C:/Users/matsuda/workspace/machine_learning_data/hand/20170813/linear_svm_function.dat"; // �w�K���f���̃p�X

random_device HandCursor::rd;
std::mt19937 HandCursor::mt(HandCursor::rd());

/* �J�[�\���̐F */
const ofColor HandCursor::cursor_color_list[] = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange };
uniform_int_distribution<int> HandCursor::random_color(0, 7);

/* Scalar�^�̐F */
const Scalar HandCursor::CV_RED = Scalar(0, 0, 255);
const Scalar HandCursor::CV_BLUE = Scalar(255, 0, 0);
const Scalar HandCursor::CV_ORANGE = Scalar(76, 183, 255);

DEFINE_int32(logging_level, 3, "The logging level. Integer in the range [0, 255]. 0 will output any log() message, while"
	" 255 will not output any. Current OpenPose library messages are in the range 0-4: 1 for"
	" low priority messages and 4 for important ones.");
// Producer
DEFINE_string(image_path, "C:/openpose-1.2.1/examples/media/COCO_val2014_000000000192.jpg", "Process the desired image.");
// OpenPose
DEFINE_string(model_pose, "COCO", "Model to be used. E.g. `COCO` (18 keypoints), `MPI` (15 keypoints, ~10% faster), "
	"`MPI_4_layers` (15 keypoints, even faster but less accurate).");
DEFINE_string(model_folder, "C:/openpose-1.2.1/models/", "Folder path (absolute or relative) where the models (pose, face, ...) are located.");
DEFINE_string(net_resolution, "-1x368", "Multiples of 16. If it is increased, the accuracy potentially increases. If it is"
	" decreased, the speed increases. For maximum speed-accuracy balance, it should keep the"
	" closest aspect ratio possible to the images or videos to be processed. Using `-1` in"
	" any of the dimensions, OP will choose the optimal aspect ratio depending on the user's"
	" input value. E.g. the default `-1x368` is equivalent to `656x368` in 16:9 resolutions,"
	" e.g. full HD (1980x1080) and HD (1280x720) resolutions.");
DEFINE_string(output_resolution, "-1x-1", "The image resolution (display and output). Use \"-1x-1\" to force the program to use the"
	" input image resolution.");
DEFINE_int32(num_gpu_start, 0, "GPU device start number.");
DEFINE_double(scale_gap, 0.3, "Scale gap between scales. No effect unless scale_number > 1. Initial scale is always 1."
	" If you want to change the initial scale, you actually want to multiply the"
	" `net_resolution` by your desired initial scale.");
DEFINE_int32(scale_number, 1, "Number of scales to average.");

HandCursor::HandCursor() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0), track_id(0), f(this->frequency, this->mincutoff, this->beta, this->dcutoff), f2(this->frequency, 0.5, 1.5, this->dcutoff) {
	this->face_detector = get_frontal_face_detector();

	deserialize(this->model_path) >> df; // �t�@�C������w�K�ς݂̃��f����ǂݍ���

	this->frame = Mat(Size(CAMERA_W, CAMERA_H), CV_8UC3);

	//this->writer.open("hand_detect_5.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 15, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	++this->frame_count;

	/* fps��\�� */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	this->frame = this->cap.get_image(); // �J��������摜���擾

	this->body_part_detect();

	//assign_image(this->org_image_buffer.get_push_position(), cv_image<bgr_pixel>(this->frame));
	//this->org_image_buffer.forward_offset();

	//assign_image(this->gs_image_buffer.get_push_position(), this->org_image_buffer.get_read_position());
	//this->gs_image_buffer.forward_offset();

	//this->new_thread_face_detect();
	//if (!this->face_dets.empty()) {
	//	this->new_thread_hand_detect();
	//}

	//printf("frame count : %ld\n", this->frame_count);
	this->show_detect_window(); // ���o�`�F�b�N�p�̃E�B���h�E��\��
}

void HandCursor::exit() {
	this->stop_flag = true;
}

void HandCursor::modulate_cursor(const int& user_id) {
	try {
		this->inverse_transform_point(this->track_data.at(user_id).transformed_cursor_point, this->track_data.at(user_id).cursor_point);
	}
	catch (std::out_of_range&) {}

}

void HandCursor::show_detect_window() {
	this->view_frame = this->frame;

	const auto numberPeopleDetected = this->points_tmp.getSize(0);
	cout << "detect num : " << numberPeopleDetected << endl;
	for (int i = 0; i < numberPeopleDetected; ++i) {
		cv::circle(this->view_frame, Point(this->points_tmp[{i, 0, 0}], this->points_tmp[{i, 0, 1}]), 20, this->CV_BLUE, -1);
		cv::circle(this->view_frame, Point(this->points_tmp[{i, 4, 0}], this->points_tmp[{i, 4, 1}]), 20, this->CV_RED, -1);
		cv::circle(this->view_frame, Point(this->points_tmp[{i, 7, 0}], this->points_tmp[{i, 7, 1}]), 20, this->CV_RED, -1);

		cout << this->points_tmp[{i, 0, 0}] << " " << this->points_tmp[{i, 0, 1}] << endl;
	}

	//parallel_for(0, this->hand_dets.size(), [&](int i) {
	//	cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), this->CV_RED, 5);
	//});

	//parallel_for(0, this->face_dets.size(), [&](int i) {
	//	cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), this->CV_BLUE, 5);
	//});

	//for (const auto &t : this->track_data) {
	//	cv::rectangle(view_frame, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_ORANGE, 5);
	//}

	imshow("detect window", view_frame);

	//this->writer << view_frame;
}

void HandCursor::body_part_detect() {
	const auto outputSize = op::flagsToPoint(FLAGS_output_resolution, "-1x-1");
	const auto netInputSize = op::flagsToPoint(FLAGS_net_resolution, "-1x368");
	const auto poseModel = op::flagsToPoseModel(FLAGS_model_pose);

	op::ScaleAndSizeExtractor scaleAndSizeExtractor(netInputSize, outputSize, FLAGS_scale_number, FLAGS_scale_gap);
	op::CvMatToOpInput cvMatToOpInput;
	op::CvMatToOpOutput cvMatToOpOutput;
	op::PoseExtractorCaffe poseExtractorCaffe{ poseModel, FLAGS_model_folder, FLAGS_num_gpu_start };
	op::OpOutputToCvMat opOutputToCvMat;
	op::FrameDisplayer frameDisplayer{ "OpenPose Tutorial - Example 1", outputSize };
	// Step 4 - Initialize resources on desired thread (in this case single thread, i.e. we init resources here)
	poseExtractorCaffe.initializationOnThread();

	// ------------------------- POSE ESTIMATION AND RENDERING -------------------------
	// Step 1 - Read and load image, error if empty (possibly wrong path)
	// Alternative: cv::imread(FLAGS_image_path, CV_LOAD_IMAGE_COLOR);
	cv::Mat inputImage = this->frame;
	if (inputImage.empty())
		op::error("Could not open or find the image: " + FLAGS_image_path, __LINE__, __FUNCTION__, __FILE__);
	const op::Point<int> imageSize{ inputImage.cols, inputImage.rows };
	// Step 2 - Get desired scale sizes
	std::vector<double> scaleInputToNetInputs;
	std::vector<op::Point<int>> netInputSizes;
	double scaleInputToOutput;
	op::Point<int> outputResolution;
	std::tie(scaleInputToNetInputs, netInputSizes, scaleInputToOutput, outputResolution)
		= scaleAndSizeExtractor.extract(imageSize);
	// Step 3 - Format input image to OpenPose input and output formats
	const auto netInputArray = cvMatToOpInput.createArray(inputImage, scaleInputToNetInputs, netInputSizes);
	auto outputArray = cvMatToOpOutput.createArray(inputImage, scaleInputToOutput, outputResolution);
	// Step 4 - Estimate poseKeypoints
	poseExtractorCaffe.forwardPass(netInputArray, imageSize, scaleInputToNetInputs);
	this->points_tmp.reset();
	this->points_tmp = poseExtractorCaffe.getPoseKeypoints();
	//this->points_tmp = poseKeypoints;
}

/* �猟�o */
void HandCursor::face_detect() {
	this->face_thread_flag = true;

	this->face_dets = this->face_detector(this->gs_image_buffer.get_read_position());

	if (!this->face_dets.empty()) { // ���o�����炪�����

		/* ���ɒǐՂ��Ă����̋߂��̊�͏��� */
		for (const auto& td : this->track_data) {
			for (auto fd = begin(this->face_dets); fd != end(this->face_dets);) {
				if (this->euclid_distance(td.second.face_point, center(*fd)) < 300) {
					fd = this->face_dets.erase(fd);
				}
				else {
					++fd;
				}
			}
		}

		this->new_thread_hand_detect(); // �茟�o�X���b�h���N��
	}

	this->face_thread_flag = false;
}

/* �茟�o */
void HandCursor::hand_detect() {
	this->hand_thread_flag = true;

	this->hand_dets.clear();

	std::vector<dlib::rectangle> face_dets_tmp = this->face_dets, hand_dets_tmp;
	std::vector<dlib::rectangle> windows; // �X���C�f�B���O�E�B���h�E�p�̋�`
	array2d<unsigned char> roi;

	/* ���ɒǐՂ��Ă����̋߂��̊�͏��� */
	for (const auto &td : this->track_data) {
		for (auto fd = begin(face_dets_tmp); fd != end(face_dets_tmp);) {
			if (this->euclid_distance(td.second.face_point, center(*fd)) < 300) {
				fd = face_dets_tmp.erase(fd);
			}
			else {
				++fd;
			}
		}
	}

	for (const auto &fd : face_dets_tmp) {
		//* ��̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬(�E�B���h�E�T�C�Y������傫���ĈقȂ�3���) */
		std::vector<std::vector<dlib::rectangle>> sliding_windows(3);
		SlidingWindows sw(fd.width(), fd.width() / 5, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		SlidingWindows sw2(fd.width() + 50, fd.width() / 5 + 10, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		SlidingWindows sw3(fd.width() + 100, fd.width() / 5 + 20, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		sliding_windows[0] = move(sw.get_windows());
		sliding_windows[1] = move(sw2.get_windows());
		sliding_windows[2] = move(sw3.get_windows());

		/* ��̎��ӂ̃X���C�f�B���O�E�B���h�E�Ŏ茟�o */
		for (const auto& sw : sliding_windows) {
			for (const auto& w : sw) {
				extract_image_chip(this->gs_image_buffer.get_read_position(), w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}
		}

		if (hand_dets_tmp.empty()) { // ���o���ꂽ�肪�Ȃ����
			continue;
		}

		/* ���ɒǐՂ��Ă����̋߂��̎������ */
		for (const auto& td : this->track_data) {
			for (auto hd = begin(hand_dets_tmp); hd != end(hand_dets_tmp);) {
				if (this->euclid_distance(center(td.second.hand), center(*hd)) < 300) {
					hd = hand_dets_tmp.erase(hd);
				}
				else {
					++hd;
				}
			}
		}

		if (hand_dets_tmp.empty()) { // ���ׂĂ̎��₪���ɒǐՂ��Ă����̋߂��̎�Ƃ��č폜���ꂽ��
			continue;
		}

		/* NonMaximumSuppression�ɂ����ďd������菜�� */
		this->nms(hand_dets_tmp, this->hand_dets);

		this->track_data[this->track_id].hand = this->hand_dets[0];
		this->track_data[this->track_id].face_rect = fd;
		this->track_data[this->track_id].face_point = center(fd);
		this->transform_point(this->track_data[this->track_id].face_point, this->track_data[this->track_id].transformed_face_point);

		correlation_tracker ct;
		ct.start_track(this->org_image_buffer.get_read_position(), this->hand_dets[0]);

		this->track_data[this->track_id].cursor_point = center(this->hand_dets[0]);
		this->transform_point(this->track_data[this->track_id].cursor_point, this->track_data[this->track_id].transformed_cursor_point);

		/* �J�[�\���̐F�����Ԃ�Ȃ��悤�ɑI�� */
		int color_id;
	SAME_COLOR:
		color_id = this->random_color(this->mt);
		for (const auto& t : this->track_data) {
			if (t.second.cursor_color_id == color_id) {
				goto SAME_COLOR;
			}
		}
		this->track_data[this->track_id].cursor_color = this->cursor_color_list[color_id];
		this->track_data[this->track_id].cursor_color_id = move(color_id);

		this->new_thread_tracking(ct, this->track_id++);
	}

	this->hand_thread_flag = false;
}

/* �ǐՎ��p�̎茟�o */
void HandCursor::hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int &user_id) {
	array2d<unsigned char> roi;
	for (const auto &w : sliding_windows) {
		extract_image_chip(this->gs_image_buffer.get_read_position(), w, roi);
		if (this->is_hand(roi)) {
			this->track_data[user_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
		}
	}
}

bool HandCursor::is_hand(array2d<unsigned char> &img) {
	array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
	resize_image(img, img_resize);

	fhog_type fhog;
	extract_fhog_features(img_resize, fhog);

	X_type feature_vec;
	this->fhog_to_feature_vector(feature_vec, fhog);

	return (df(feature_vec) > this->decision_ratio);
}

void HandCursor::tracking(correlation_tracker &ct, const int user_id) {
	int m;
	int dx, dy;
	drectangle past_pos, current_pos;
	const double dx_rate = static_cast<double>(CAMERA_W) / this->track_data[user_id].face_rect.width();
	const double dy_rate = static_cast<double>(CAMERA_H) / this->track_data[user_id].face_rect.height();
	OneEuroFilter dx_filter(120, 0.5, 1.6, 1.0), dy_filter(120, 0.5, 1.6, 1.0);

	while (1) {
		/* ���߂̃t���[���Ō��o������ȊO������ */
		m = 0;
		for (const auto &d : this->track_data[user_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->track_data[user_id].track_hand_dets.erase(begin(this->track_data[user_id].track_hand_dets), begin(this->track_data[user_id].track_hand_dets) + m);

		past_pos = ct.get_position(); // ���߃t���[���̎�̈ʒu�𓾂�

		ct.update(this->org_image_buffer.get_read_position()); // �ǐՈʒu�̍X�V

		/* ���݂̒ǐՈʒu(��`)�𓾂� */
		current_pos = ct.get_position();
		this->track_data[user_id].hand = current_pos;

		/* ���݂̒ǐՈʒu�̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬���Ď�����o */
		/* ���ӂƂ͒ǐՂ��Ă����̋�`1�����ӂ͈̔� */
		SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max(static_cast<int>(current_pos.left() - this->track_data[user_id].hand.width()), 0), std::min(static_cast<int>(current_pos.right() + this->track_data[user_id].hand.width()), CAMERA_W), std::max(static_cast<int>(current_pos.top() - this->track_data[user_id].hand.height()), 0), std::min(static_cast<int>(current_pos.bottom() + this->track_data[user_id].hand.height()), CAMERA_H));
		this->hand_detect(local_sw.get_windows(), user_id);

		/* ���݂̒ǐՈʒu�ƒ��O�̒ǐՈʒu�̍� */
		dx = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x����
		dy = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y����

		dx = dx_filter.filter(dx);
		dy = dy_filter.filter(dy);

		/* �J�[�\���̈ʒu���X�V */
		this->track_data[user_id].cursor_point = point(ofClamp(this->track_data[user_id].cursor_point.x() + dx_rate * dx, 0, CAMERA_W), ofClamp(this->track_data[user_id].cursor_point.y() + dy_rate * dy, 0, CAMERA_H)); // ���݂̒ǐՈʒu���瑊�ΓI�ɃJ�[�\���̈ʒu������
		this->transform_point(this->track_data[user_id].cursor_point, this->track_data[user_id].transformed_cursor_point);

		if (this->track_data[user_id].track_hand_dets.empty() || this->stop_flag) { // ���߃t���[���Ŏ肪���o����Ȃ�������ǐՂ���߂�
			this->track_data.erase(user_id);
			break;
		}
	}
}

void HandCursor::new_thread_tracking(correlation_tracker &ct, const int track_id) {
	void(HandCursor::*funcp)(correlation_tracker&, const int) = &HandCursor::tracking;
	thread th(funcp, this, ct, track_id);
	th.detach();
}

void HandCursor::new_thread_hand_detect() {
	if (!this->hand_thread_flag) {
		void(HandCursor::*funcp)() = &HandCursor::hand_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandCursor::new_thread_face_detect() {
	if (!this->face_thread_flag) {
		void(HandCursor::*funcp)() = &HandCursor::face_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandCursor::fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog) {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.emplace_hint(end(feature_vector), j++, fhog[r][c](i));
			}
		}
	}
}

double HandCursor::euclid_distance(const point& p1, const point& p2) const {
	return ofDist(p1.x(), p1.y(), p2.x(), p2.y());
}

void HandCursor::transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x();
	dst_point.y() = RESOLUTION_RATIO_H * src_point.y();
}

void HandCursor::inverse_transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = (DISPLAY_W - src_point.x()) / RESOLUTION_RATIO_W;
	dst_point.y() = src_point.y() / RESOLUTION_RATIO_H;
}