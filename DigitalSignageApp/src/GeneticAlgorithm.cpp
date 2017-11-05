#include "GeneticAlgorithm.h"
 
using namespace std;
using namespace param;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_indivisual(0, GeneticAlgorithm::population_size);
uniform_real_distribution<double> GeneticAlgorithm::random_0to1(0.0, 1.0);

GeneticAlgorithm::GeneticAlgorithm():population(this->population_size) {
	this->population.reserve(this->population_size + 2*this->crossover_pair_number); // �����ɂ���đ��������镪�����W�c�T�C�Y�̃��������m��
}

void GeneticAlgorithm::operator()(GridForm& best_individual) {
	this->initialize();
	this->population[0].draw();
}

void GeneticAlgorithm::initialize() {
	/* �����_���ɏ����W�c�𐶐� */
	for (int i = 0; i < this->population_size; ++i) {
		for (int x = 0; x < FORM_W; ++x) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[i].set_bit(x, y, this->random_0or1(this->mt));
			}
		}
	}
}

void GeneticAlgorithm::crossover() {
	for (int p = 0; p < this->crossover_pair_number; ++p) {
		/* �قȂ��̌�(�y�A)��I�� */
		this->crossover_pair_index.first = this->random_indivisual(this->mt);
		do {
			this->crossover_pair_index.second = this->random_indivisual(this->mt);
		} while (this->crossover_pair_index.first == this->crossover_pair_index.second);

		/* �����m���Ɋ�Â��Č������邩�ǂ��������߂� */
		if (this->random_0to1(this->mt) < this->crossover_probablity) {

		}
	}
}