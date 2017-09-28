/* 画像からfhog特徴量抽出して、csvファイルを作成 */

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <cstdio>

#include <Windows.h>

#include <opencv2/opencv.hpp>

#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/svm_threaded.h>
#include <dlib/opencv.h>

using namespace std;
using namespace cv;
using namespace dlib;

int main() {
	const string path = ""; // 訓練データのディレクトリパス

	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;
	hFind = FindFirstFileA((path + "*.jpg").c_str(), &win32fd);

	string filename;
	array2d<rgb_pixel> img;
	array2d<matrix<double, 31, 1>> fhog;

	ofstream ofs(path + "fhog.csv");

	//int n = 1; // 確認のためのカウンタ

	do {
		filename = win32fd.cFileName;

		//printf("[%d] %s\n", n++, filename.c_str()); // 確認

		load_image(img, path + filename);

		extract_fhog_features(img, fhog);

		/* ラベル付け */
		if (filename.substr(0, 8) == "positive") { // ファイル名の先頭が"positive"ならば正のデータ
			ofs << +1;
		}
		else {
			ofs << -1;
		}

		for (int r = 0; r < fhog.nr(); ++r) {
			for (int c = 0; c < fhog.nc(); ++c) {
				for (int i = 0; i < 31; ++i) {
					ofs << "," << fhog[r][c](i);
				}
			}
		}
		ofs << endl;

	} while (FindNextFileA(hFind, &win32fd));
	
	FindClose(hFind);
	ofs.close();

	return 0;
}
