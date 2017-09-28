/* 画像からhog特徴量抽出して、csvファイルを作成 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

#include <Windows.h>

#include <dlib/image_io.h>
#include <dlib/image_keypoint.h>

using namespace std;
using namespace dlib;

int main() {
	const string path = ""; // 訓練データのディレクトリパス

	/* hog特徴量計算のためのパラメータ */
	constexpr int cell_size = 8;
	constexpr int block_size = 2;
	constexpr int cell_stride = 1;
	constexpr int num_orientation_bins = 8;

	hog_image<cell_size, block_size, cell_stride, num_orientation_bins, hog_signed_gradient, hog_full_interpolation> hog;

	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;
	hFind = FindFirstFileA((path + "*.jpg").c_str(), &win32fd);

	string filename;
	array2d<rgb_pixel> img;
	
	ofstream ofs(path + "hog.csv");

	//int n = 1; // 確認のためのカウンタ

	do {
		filename = win32fd.cFileName;

		//printf("[%d] %s\n", n++, filename.c_str()); // 確認

		load_image(img, path + filename);

		hog.load(img);

		/* ラベル付け */
		if (filename.substr(0, 8) == "positive") { // ファイル名の先頭が"positive"ならば正のデータ
			ofs << +1;
		}
		else {
			ofs << -1;
		}

		for (int r = 0; r < hog.nr(); ++r) {
			for (int c = 0; c < hog.nc(); ++c) {
				for (int i = 0; i < hog.get_num_dimensions(); ++i) {
					ofs << "," << hog(r, c)(i);
				}
			}
		}
		ofs << endl;

	} while (FindNextFileA(hFind, &win32fd));

	FindClose(hFind);
	ofs.close();

	return 0;
}