#include "GeneticAlgorithm.h"
 
using namespace std;
using namespace param;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_indivisual(0, GeneticAlgorithm::population_size);
uniform_int_distribution<int> GeneticAlgorithm::random_crossover_point(1, BITS_SIZE-1);
uniform_int_distribution<int> GeneticAlgorithm::random_bit(0, BITS_SIZE);
uniform_real_distribution<double> GeneticAlgorithm::random_0to1(0.0, 1.0);

GeneticAlgorithm::GeneticAlgorithm():population(this->population_size) {
	this->population.reserve(this->population_size + 2*this->crossover_pair_number); // �����ɂ���đ��������镪�����W�c�T�C�Y�̃��������m��
}

void GeneticAlgorithm::operator()(GridForm& best_individual) {
	this->initialize();
	this->population[0].draw();
	for (int i = 0; i < 10; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}
	best_individual = this->population[0];
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
	for (int i = 0; i < this->crossover_pair_number; ++i) {
		/* �����m���Ɋ�Â��Č������邩�ǂ��������߂� */
		if (this->random_0to1(this->mt) < this->crossover_probability) {
			/* �قȂ��̌�(�e)��I�� */
			pair<int, int> parents_index(this->random_indivisual(this->mt), this->random_indivisual(this->mt));
			while (parents_index.first == parents_index.second) {
				parents_index.second = this->random_indivisual(this->mt);
			}

			/* ��_���� */
			int crossover_point = this->random_crossover_point(this->mt);
			pair<GridForm, GridForm> children;
			children.first.grid_bits = this->population[parents_index.first].grid_bits;
			children.second.grid_bits = this->population[parents_index.second].grid_bits;
			for (int j = 0; j < crossover_point; ++j) {
				children.first.grid_bits[j] = this->population[parents_index.second].grid_bits[j];
				children.second.grid_bits[j] = this->population[parents_index.first].grid_bits[j];
			}
			this->population.emplace_back(children.first);
			this->population.emplace_back(children.second);
		}
	}
}

void GeneticAlgorithm::mutation() {
	for (auto& i : this->population) {
		/* �ˑR�ψٗ��Ɋ�Â��ēˑR�ψق��邩�ǂ��������߂� */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			i.grid_bits.flip(this->random_bit(this->mt)); // �����_���Ȉʒu�̃r�b�g�𔽓]
		}
	}
}

void GeneticAlgorithm::calculate_fitness() {
	const int population_size_now = this->population.size(); // ������ˑR�ψقɂ���đ��������ꎞ�I�ȏW�c�T�C�Y
	this->fitness.resize(population_size_now);
	fill(begin(this->fitness), end(this->fitness), 0.0);

	for (int i = 0; i < population_size_now; ++i) {
		this->fitness[i] += this->population[i].grid_bits.count(); // �ʐ�
	}
}

void GeneticAlgorithm::selection() {
	vector<GridForm> new_population(this->population_size);

	/* �G���[�g��` */
	const int elite_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	new_population[0] = this->population[elite_index];

	/* �K���x��1�`100�ɃX�P�[�����O */
	const double scaled_fitness_max = 100;
	const double scaled_fitness_min = 1;
	const double fitness_max = this->fitness[elite_index];
	const double fitness_min = *min_element(begin(this->fitness), end(this->fitness));
	const double a = (scaled_fitness_max - scaled_fitness_min) / (fitness_max - fitness_min);
	const double b = scaled_fitness_min - a*fitness_min;
	for (auto& f : this->fitness) {
		f = a*f + b;
	}

	const double fitness_sum = accumulate(begin(this->fitness), end(this->fitness), 0.0);
	discrete_distribution<size_t> roulette(begin(this->fitness), end(this->fitness));
	int selection_index;
	for (int i = 1; i < this->population_size; ++i) {
		do {
			selection_index = roulette(this->mt);
		} while (selection_index == elite_index);
	}

	this->population = new_population;

}