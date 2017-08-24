#include <iostream>
#include <vector>
#include <random>

#include <Windows.h>

#include <opencv2/opencv.hpp>

#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

using namespace dlib;
using namespace std;

void get_jpg_files(std::vector<string> &v, const string path) {
	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;

	hFind = FindFirstFileA((path + "*.jpg").c_str(), &win32fd);

	do {
		v.emplace_back(win32fd.cFileName);
	} while (FindNextFileA(hFind, &win32fd));

	shuffle(begin(v), end(v), std::mt19937());
	
	CloseHandle(hFind);
}

int main() {

	const string path_from = ""; // �����`�ɐ��`�����摜�̃f�B���N�g���p�X
	const string path_to = ""; // ���ʉ摜��ۑ�����f�B���N�g���p�X
	const string path_bg = ""; // �w�i�摜�̃f�B���N�g���p�X
	const string path_ng = ""; // ���ꂽ���l�K�e�B�u�摜�̃f�B���N�g���p�X

	std::vector<string> org_files, bg_files, ng_files;
	get_jpg_files(org_files, path_from);
	get_jpg_files(bg_files, path_bg);
	get_jpg_files(ng_files, path_ng);

	/* �����̏��� */
	random_device rd;
	std::mt19937 mt(rd());

	uniform_int_distribution<int> rnd_bg_file(0, bg_files.size() - 1);

	constexpr int n = 10000; // �쐬������training data�̐�
	int c = 1;
	constexpr int output_img_size = 28; // �o�͉摜�̃T�C�Y(�����`)

	array2d<rgb_pixel> img_org, img_ng, img_bg, img_tmp, img_output(output_img_size, output_img_size);
	array2d<unsigned char> img_gs, img_bi;

	/* positive data�̍쐬 */
	while (c <= n / 2) {

		shuffle(begin(org_files), end(org_files), std::mt19937());
		
		for (auto org_file = begin(org_files); org_file != end(org_files) && c <= n / 2; ++org_file, ++c) {
			load_image(img_org, path_from + *org_file); // �I���W�i���摜��ǂݍ���
			assign_image(img_tmp, img_org);
			assign_image(img_gs, img_org);

			/* �����_���ɔw�i�摜��I�� */
			string bg_filename;
			do { // �I���W�i���摜���傫���w�i�摜��I��
				bg_filename = bg_files[rnd_bg_file(mt)];
				try {
					load_image(img_bg, path_bg + bg_filename);
				}
				catch (exception& e) {
					puts("error!! ignore this image.");
					DeleteFileA((path_bg + bg_filename).c_str());
					continue;
				}
			} while (img_bg.nr() < img_org.nr() || img_bg.nc() < img_org.nc());

			uniform_int_distribution<int> rnd_r(0, img_bg.nr() - img_org.nr());
			uniform_int_distribution<int> rnd_c(0, img_bg.nc() - img_org.nc());
			long start_r = rnd_r(mt);
			long start_c = rnd_c(mt);
			auto_threshold_image(img_gs, img_bi); // 2�l��

			for (long r = 0; r < img_org.nr(); ++r) {
				for (long c = 0; c < img_org.nc(); ++c) {
					if (img_bi[r][c] == img_bi[0][0]) {
						assign_pixel(img_tmp[r][c], img_bg[start_r + r][start_c + c]);
					}
				}
			}

			resize_image(img_tmp, img_output);

			string output_filename = "positive_image" + to_string(c) + ".jpg";

			printf("[%d] %s + %s\n", c, org_file->c_str(), bg_filename.c_str());

			save_jpeg(img_output, path_to + output_filename);
		}
	}
	
	/* negative data�̍쐬 */
	c = 1;

	// ���炩���ߗp�ӂ���negative data������
	for (auto ng_file = begin(ng_files); ng_file != end(ng_files); ++ng_file, ++c) {

		load_image(img_ng, path_ng + *ng_file);
		resize_image(img_ng, img_output);

		string output_filename = "negative_image" + to_string(c) + ".jpg";

		printf("[%d] %s\n", c + n / 2, output_filename.c_str());

		save_jpeg(img_output, path_to + output_filename);
	}

	while (c <= n / 2) {

		shuffle(begin(bg_files), end(bg_files), std::mt19937());

		for (auto bg_file = begin(bg_files); bg_file != end(bg_files) && c <= n / 2; ++bg_file, ++c) {

			try {
				load_image(img_bg, path_bg + *bg_file);
			}
			catch (exception& e) {
				puts("error!! ignore this image.");
				DeleteFileA((path_bg + *bg_file).c_str());
				continue;
			}

			uniform_int_distribution<int> rnd_r(0, img_bg.nr() - output_img_size);
			uniform_int_distribution<int> rnd_c(0, img_bg.nc() - output_img_size);
			long start_r = rnd_r(mt);
			long start_c = rnd_c(mt);

			extract_image_chip(img_bg, dlib::rectangle(start_c, start_r, start_c + output_img_size, start_r + output_img_size), img_output);

			string output_filename = "negative_image" + to_string(c) + ".jpg";

			printf("[%d] %s\n", c + n / 2, output_filename.c_str());

			save_jpeg(img_output, path_to + output_filename);
		}
	}

	return 0;
}