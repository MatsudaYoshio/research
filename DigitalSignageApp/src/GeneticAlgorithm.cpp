#include "GeneticAlgorithm.h"
 
using namespace std;
using namespace param;

GeneticAlgorithm::GeneticAlgorithm() :mt(rd()), rn_bit(uniform_int_distribution<int>(0, 1)) {}

void GeneticAlgorithm::operator()(GridForm& best_individual) {
	this->initialize();
	this->population[0].draw();
}

void GeneticAlgorithm::initialize() {
	for (int i = 0; i < this->population_size; ++i) {
		for (int x = 0; x < FORM_W; ++x) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[i].grid_bits[x][y] = this->rn_bit(this->mt);
			}
		}
	}
}