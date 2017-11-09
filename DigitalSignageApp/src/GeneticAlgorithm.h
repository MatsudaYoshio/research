#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "AppParameters.h"
#include "HandCursor.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 20; // �W�c�T�C�Y(�I�𓑑����ꂽ��͕K�����̐�)
	static constexpr double crossover_probability = 0.8; // �����m��(��������������m��)
	static constexpr int crossover_pair_number = 8; // ������K������y�A��
	static constexpr int mutation_probability = 0.01; // �ˑR�ψٗ�(�ˑR�ψق���������m��)

	/* ���� */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0��1
	static std::uniform_int_distribution<int> random_indivisual; // �W�c����̂�I�ԗ���
	static std::uniform_int_distribution<int> random_crossover_point; // �����_��I�ԗ���
	static std::uniform_int_distribution<int> random_bit;
	static std::uniform_real_distribution<double> random_0to1; // 0����1�̊Ԃ̎���

	HandCursor* hc; // ��J�[�\��
	int* selected_user_num; // �I�𑀍�����Ă��郆�[�U��(�T�u�E�B���h�E���o���Ă��郆�[�U��)
	array<unordered_set<int>, param::BITS_SIZE> user_assignment; // �i�q��`(�r�b�g)�ɑ΂��郆�[�U�̊��蓖��

	array<array<int, param::FORM_H>, param::FORM_W> grid2bit_table; // ���W����r�b�g�ւ̕ϊ��\
	array<pair<int, int>, param::BITS_SIZE> bit2grid_table; // �r�b�g������W�ւ̕ϊ��\
	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects; // �i�q��`
	vector<param::genome_type> initial_individuals; // �����W�c�̌̌��

	vector<param::genome_type> population; // �W�c
	vector<double> fitness; // �K���x

	/* ������ */
	void initialize();
	void initialize(const param::genome_type& initial_individual);
	
	void crossover(); // ����
	void mutation(); // �ˑR�ψ�
	void calculate_fitness(); // �K���x�̌v�Z
	void selection(); // �I�𓑑�

	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const; // ���[�N���b�h����
public:
	void setup(HandCursor* hc, int* selected_user_num);
	void draw_rectangles(const param::genome_type& g) const;
	void operator()(param::genome_type& best_individual, const array<unordered_set<int>, param::BITS_SIZE>& user_assignment);
	void operator()(const param::genome_type& initial_individual, param::genome_type& best_individual, const array<unordered_set<int>, param::BITS_SIZE>& user_assignment);
};

#endif