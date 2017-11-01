#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "GridForm.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 10; // �W�c�T�C�Y

	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> rn_bit;

	array<GridForm, population_size> population; // �W�c

	void initialize();
public:
	GeneticAlgorithm();
	void operator()(GridForm& best_individual);
};

#endif