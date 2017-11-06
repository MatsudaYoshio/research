#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "GridForm.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 10; // �W�c�T�C�Y(�I�𓑑����ꂽ��͕K�����̐�)
	static constexpr double crossover_probability = 0.8; // �����m��(��������������m��)
	static constexpr int crossover_pair_number = 4; // ������K������y�A��
	static constexpr int mutation_probability = 0.01; // �ˑR�ψٗ�(�ˑR�ψق���������m��)

	/* ���� */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0��1
	static std::uniform_int_distribution<int> random_indivisual; // �W�c����̂�I�ԗ���
	static std::uniform_int_distribution<int> random_crossover_point; // �����_��I�ԗ���
	static std::uniform_int_distribution<int> random_bit;
	static std::uniform_real_distribution<double> random_0to1; // 0����1�̊Ԃ̎���

	vector<GridForm> population; // �W�c
	vector<double> fitness; // �K���x

	void initialize(); // ������
	void crossover(); // ����
	void mutation(); // �ˑR�ψ�
	void calculate_fitness(); // �K���x�̌v�Z
	void selection(); // �I�𓑑�
public:
	GeneticAlgorithm();
	void operator()(GridForm& best_individual);
};

#endif