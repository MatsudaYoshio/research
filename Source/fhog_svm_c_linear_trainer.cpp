#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <dlib/svm.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/svm_threaded.h>
#include <dlib/opencv.h>

using namespace std;
using namespace dlib;

int main()
{
	const string path = ""; // csv�t�@�C��������f�B���N�g���̃p�X

	using X_type = std::map<unsigned long, double>; // �P���f�[�^�̌^
	using kernel_type = sparse_linear_kernel<X_type>; // �J�[�l���̌^

	std::vector<X_type> X;
	std::vector<double> y;

	ifstream ifs(path + "fhog.csv"); // csv�t�@�C�����J��

	string str;
	int i;

	/* csv�t�@�C����������x�N�g���𓾂� */
	while (getline(ifs, str)) {
		i = 0;
		string token;

		/* 1�ڂ̗v�f�����ă��x���t�� */
		if (str[0] == '1') {
			y.emplace_back(+1);
			str = str.substr(2);
		}
		else {
			y.emplace_back(-1);
			str = str.substr(3);
		}

		X_type feature_vector;

		istringstream stream(str);
		while (getline(stream, token, ',')) {
			double f = stod(token);
			feature_vector.insert(make_pair(i++, f));
		}
		X.emplace_back(feature_vector);
	}
	ifs.close();

	svm_c_linear_trainer<kernel_type> linear_trainer;

	randomize_samples(X, y);
	
	/* ����������s�� */
	/*
	cout << "doing cross validation" << endl;
	for (double c = 0.1; c <= 100; c += 0.2) {
		linear_trainer.set_c(c);
		cout << "c: " << c;
		
		matrix<double, 1, 2> cv = cross_validate_trainer_threaded(linear_trainer, X, y, 10, 4); // ���񏈗��ōs��
		cout << "     cross validation accuracy: "
			<< "precision is " << cv(0) << ", recall is " << cv(1) << ", F-score is " << 2*cv(0)*cv(1)/(cv(0)+cv(1))<< endl;
	}
	*/
	
	linear_trainer.be_verbose(); // �P���ڍׂ̏o�͂�����
	linear_trainer.set_c(0.7);

	decision_function<kernel_type> df = linear_trainer.train(X, y);

	// �e�X�g
	/*
	array2d<rgb_pixel> img_test;
	load_image(img_test, "");
	array2d<matrix<double, 31, 1>> fhog;
	extract_fhog_features(img_test, fhog);

	X_type feature_vector;
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.insert(make_pair(j++, fhog[r][c](i)));
			}
		}
	}

	cout << "test result : " << df(feature_vector) << endl;
	*/

	serialize(path + "linear_svm_function.dat") << df; // �P�����ʂ��t�@�C���ɏo��

	return 0;
}