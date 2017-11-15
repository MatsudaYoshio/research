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
	/* �ߖT�T���p�̕����x�N�g��(8�ߖT) */
	static constexpr int dx[] = { 1, 0, -1, 0, 1, -1, -1, 1 };
	static constexpr int dy[] = { 0, -1, 0, 1, -1, -1, 1, 1 };

	/* ���� */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0��1��Ԃ�����
	static std::uniform_int_distribution<int> random_indivisual; // �W�c����̂�I�ԗ���
	//static std::uniform_int_distribution<int> random_crossover_point; // �����_��I�ԗ���
	static std::uniform_int_distribution<int> random_bit;
	static std::uniform_real_distribution<double> random_0to1; // 0����1�̊Ԃ̎�����Ԃ�����

	HandCursor* hc; // ��J�[�\��

	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects; // �i�q��`
	vector<param::genome_type> initial_individuals; // �����W�c�̌̌��
	param::genome_type base_individual;

	vector<param::genome_type> population; // �W�c
	vector<double> fitness; // �K���x
	param::genome_type best_individual; // �œK��

	/* ������ */
	void initialize(const set<int>& users_id);
	void initialize(const param::genome_type& initial_individual);
	
	void crossover(); // ����
	void mutation(); // �ˑR�ψ�
	void calculate_fitness(); // �K���x�̌v�Z
	void selection(); // �I�𓑑�

	int block_bits_size; // �e�u���b�N�̃r�b�g��
	int genetic_length; // ��`�q��(���r�b�g��)
	array<array<int, param::FORM_H>, param::FORM_W> grid2block_table; // ���W����u���b�N�ւ̕ϊ��\
	array<pair<int, int>, param::BLOCK_SIZE> block2grid_table; // �u���b�N������W�ւ̕ϊ��\ 
	vector<array<int, param::BLOCK_SIZE>> block_assignment; // �e�u���b�N�ɑ΂��郆�[�U�̊��蓖��
	set<int> users_id; // ���[�UID���X�g
	int users_num; // ���[�U��
	unordered_map<int, int> user_id_index; // ���[�UID�ɑ΂���C���f�b�N�X


	int elite_index;
	array<int, param::BLOCK_SIZE> elite_block_assignment;

	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const; // ���[�N���b�h����
public:
	void setup(HandCursor* hc);
	void draw_rectangles() const;
	void operator()(param::genome_type& best_individual, const set<int>& users_id);
	void operator()(const param::genome_type& initial_individual, param::genome_type& best_individual, const set<int>& users_id);
};

#endif