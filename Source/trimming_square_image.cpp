#include <iostream>

#include <Windows.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Rect box;
bool drawing_box = false;

void draw_box(Mat* img, Rect rect) {
	rectangle(*img, Point2d(box.x, box.y), Point2d(box.x + box.width, box.y + box.height), Scalar(0x00, 0x00, 0xff));
}

void my_mouse_callback(int event, int x, int y, int flags, void* param) {

	Mat* image = static_cast<Mat*>(param);

	switch (event) {
	case EVENT_MOUSEMOVE:
		if (drawing_box) {
			box.width = x - box.x;
			box.height = y - box.y;

			/* 高さと幅を同じにすることで正方形しか切り取れないようにする */
			int r = (box.width + box.height) / 2;
			box.width = box.height = r;
		}
		break;

	case EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = Rect(x, y, 0, 0);
		break;

	case EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0) {
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0) {
			box.y += box.height;
			box.height *= -1;
		}
		draw_box(image, box);
		break;
	}

}

int main() {

	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;

	const string path_from = "";
	const string path_to = "";

	hFind = FindFirstFileA((path_from + "*.jpg").c_str(), &win32fd);

	Mat img_org, img_pro, img_tmp, img_trim;

	const string window_pro = "processing";
	const string window_res = "result";

	namedWindow(window_pro, CV_WINDOW_AUTOSIZE);

	constexpr int CV_WAITKEY_ENTER = 13;  // エンターキー
	constexpr int CV_WAITKEY_SPACE = 32; // スペースキー

	do {
		string filename = win32fd.cFileName;

		img_org = imread(path_from + filename);
		img_org.copyTo(img_tmp);
		img_org.copyTo(img_pro);

		setMouseCallback(window_pro, my_mouse_callback, (void *)&img_tmp);

		while (1) {

			img_tmp.copyTo(img_pro);

			if (drawing_box) {
				draw_box(&img_pro, box);
			}

			imshow(window_pro, img_pro);

			int key = waitKey(1);
			if (key == 'q') {
				return 0;
			}
			else if (key == 'z') { // トリミングをやり直す
				img_org.copyTo(img_tmp);
				destroyWindow(window_res);
			}
			else if (key == 'i') { // スルー
				cout << "ignore this image  " << path_from + filename << endl;
				break;
			}
			else if (key == CV_WAITKEY_ENTER) { // エンターキーが押されたら、トリミングした画像を別ウィンドウで表示
				img_trim = Mat(img_org, box).clone();
				imshow(window_res, img_trim);
			}
			else if (key == CV_WAITKEY_SPACE) { // スペースキーが押されたら、トリミングした画像を保存する
				img_trim = Mat(img_org, box).clone();
				imwrite(path_to + filename, img_trim);
				destroyWindow(window_res);
				cout << "saved image -> " << path_to + filename << endl;
				break;
			}
		}

	} while (FindNextFileA(hFind, &win32fd));

	CloseHandle(hFind);
	destroyAllWindows();

	return 0;
}