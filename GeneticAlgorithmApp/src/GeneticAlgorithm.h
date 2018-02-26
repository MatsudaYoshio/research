#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include <dlib/threads.h>
#include <windows.h>
#include <ppl.h>
#include "AppParameters.h"
#include "HandCursor.h"
#include "TimerBase.h"

class GeneticAlgorithm {
private:
	using genome_type = vector<bool>;

	static constexpr int population_size{ 100 }; // �W�c�T�C�Y(�I�𓑑����ꂽ��͕K�����̐�)
	static constexpr double crossover_probability{ 0 }; // �����m��(��������������m��)
	static constexpr int crossover_pair_number{ 8 }; // ������K������y�A��
	static constexpr double mutation_probability{ 1.0 }; // �ˑR�ψٗ�(�ˑR�ψق���������m��)

	/* �ߖT�T���p�̕����x�N�g��(�C���f�b�N�X��0�`3�܂Ŏg����4�ߖT�A�S���g����8�ߖT) */
	static constexpr int dx[] = { 1, 0, -1, 0, 1, -1, -1, 1 };
	static constexpr int dy[] = { 0, -1, 0, 1, -1, -1, 1, 1 };

	static constexpr int grid_w{ 480 };
	static constexpr int grid_h{ 216 };
	static constexpr int form_w{ param::DISPLAY_W / grid_w };
	static constexpr int form_h{ param::DISPLAY_H / grid_h };
	static constexpr int block_size{ form_w*form_h };

	/* ���� */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0��1��Ԃ�����
	static std::uniform_int_distribution<int> random_indivisual; // �W�c����̂�I�ԗ���
	static std::uniform_int_distribution<int> random_crossover_method; // ������@��I�ԗ���
	static std::uniform_int_distribution<int> random_mutation_method; // �ˑR�ψَ�@��I�ԗ���
	static std::uniform_int_distribution<int> random_block;
	static std::uniform_real_distribution<double> random_0to1; // 0����1�̊Ԃ̎�����Ԃ�����

	HandCursor* hc; // ��J�[�\��

	array<array<ofRectangle, form_h>, form_w> grid_rects; // �i�q��`
	vector<genome_type> initial_individuals; // �����W�c�̌̌��
	genome_type base_individual;

	vector<genome_type> population; // �W�c
	vector<double> fitness; // �K���x
	genome_type elite_individual; // �G���[�g��
	double elite_fitness;

	/* ������ */
	void initialize(set<long long int>& selected_users_id, set<long long int>& all_users_id);

	void crossover(); // ����
	void mutation(); // �ˑR�ψ�
	void calculate_fitness(); // �K���x�̌v�Z
	void selection(); // �I�𓑑�

	int block_bits_size; // �e�u���b�N�̃r�b�g��
	int genetic_length; // ��`�q��(���r�b�g��)
	array<array<int, form_h>, form_w> grid2block_table; // ���W����u���b�N�ւ̕ϊ��\
	array<pair<int, int>, block_size> block2grid_table; // �u���b�N������W�ւ̕ϊ��\ 
	vector<array<int, block_size>> block_assignments; // �e�u���b�N�ɑ΂��郆�[�U�̊��蓖��
	set<long long int> selected_users_id, all_users_id; // ���[�UID���X�g
	int selected_users_num; // ���[�U��
	unordered_map<int, int> user_id_index; // ���[�UID�ɑ΂���C���f�b�N�X
	int population_size_tmp; // ������ˑR�ψقɂ���đ��������ꎞ�I�ȏW�c�T�C�Y
	vector<unordered_map<long long int, set<int>>> user_block; // �e���[�U�����u���b�N

	array<int, block_size> elite_block_assignment;

	ofstream ofs, ofs2, ofs3;
	TimerBase tb;

	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const; // ���[�N���b�h����
public:
	static constexpr int NOT_USER{ INT_MIN };

	void setup(HandCursor* hc);
	void draw() const;
	void draw(const array<int, block_size>& block_assignment) const;
	void operator()(set<long long int>& selected_users_id, set<long long int>& all_users_id);
};

#endif