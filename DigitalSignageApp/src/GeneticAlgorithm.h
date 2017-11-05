#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "GridForm.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 10; // �W�c�T�C�Y
	static constexpr double crossover_probablity = 0.8; // �����m��(��������������m��)
	static constexpr int crossover_pair_number = population_size*0.4; // ������K������y�A��

	/* �����̏��� */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0��1
	static std::uniform_int_distribution<int> random_indivisual; // �W�c����̂�I�ԗ���
	static std::uniform_real_distribution<double> random_0to1; // 0����1�̊Ԃ̎���

	vector<GridForm> population; // �W�c
	pair<int, int> crossover_pair_index; // ��������y�A�̃C���f�b�N�X

	void initialize(); // ������
	void crossover(); // ����
public:
	GeneticAlgorithm();
	void operator()(GridForm& best_individual);
};

#endif