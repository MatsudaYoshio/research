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
using namespace cv;

int main()
{
	const string path = ""; // csv�t�@�C��������f�B���N�g���p�X

	using X_type = matrix<double, 3752, 1>; // �����������̂܂܋L�q�A100*100pixel�̉摜�ɑ΂���fhog�����ʂ̎�����3752, 80x80��8*8*31��1984
	using kernel_type = radial_basis_kernel<X_type>; // �J�[�l���̌^

	std::vector<X_type> X;
	std::vector<double> y;

	ifstream ifs(path + "fhog.csv"); // csv�t�@�C�����J��

	string str;
	int i;
	X_type feature_vector;

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

		istringstream stream(str);
		while (getline(stream, token, ',')) {
			double f = stod(token);
			feature_vector(i++) = f;
		}
		X.emplace_back(feature_vector);
	}

	ifs.close();

	vector_normalizer<X_type> normalizer;

	normalizer.train(X);

	// ���񏈗��Ő��K��
	parallel_for(0, X.size(), [&](unsigned long i) {
		X[i] = normalizer(X[i]);
	});
	
	/*
	for (unsigned long i = 0; i < X.size(); ++i) {
		X[i] = normalizer(X[i]);
	}
	*/

	randomize_samples(X, y);

	svm_c_ekm_trainer<kernel_type> trainer;
	trainer.set_max_basis_size(200);  // �ő��ꐔ�̐ݒ�(�ݒ肵�Ȃ��ꍇ�͍ő�300)
	
	/* ����������s�� */
	/*
	cout << "doing cross validation" << endl;
	for (double gamma = 0.0001; gamma <= 0.002; gamma += 0.0005) {
		for (double C = 100; C <= 200; C += 50) {
			trainer.set_kernel(kernel_type(gamma));
			trainer.set_c(C);

			cout << "gamma: " << gamma << "    C: " << C;

			cout << "     cross validation accuracy: "
				<< cross_validate_trainer_threaded(trainer, X, y, 5, 4); // ���񏈗��ōs��
		}
	}
	*/

	trainer.be_very_verbose(); // �P�����̏ڍׂ��o��

	/* �p�����[�^�̐ݒ� */
	trainer.set_kernel(kernel_type(0.0005));
	trainer.set_c(200);

	using dec_funct_type = decision_function<kernel_type>;
	using funct_type = normalized_function<dec_funct_type>;

	funct_type learned_function;
	learned_function.normalizer = normalizer;
	learned_function.function = trainer.train(X, y);

	/* ���x�N�g���̐����o�� */
	cout << "\nnumber of support vectors in our learned_function is "
		<< learned_function.function.basis_vectors.size() << endl;

	/* �e�X�g */
	/*
	array2d<rgb_pixel> img_test;
	load_image(img_test, "");
	array2d<matrix<double, 31, 1>> fhog;
	extract_fhog_features(img_test, fhog);

	X_type feature_vector;
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector(j++) = fhog[r][c](i);
			}
		}
	}

	cout << "test result : " << learned_function(feature_vector) << endl;
	*/

	serialize(path + "ekm_svm_function.dat") << learned_function; // �P�����ʂ��t�@�C���ɏo��
}