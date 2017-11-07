#include "GeneticAlgorithm.h"

using namespace std;
using namespace param;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_indivisual(0, GeneticAlgorithm::population_size);
uniform_int_distribution<int> GeneticAlgorithm::random_crossover_point(1, BITS_SIZE - 1);
uniform_int_distribution<int> GeneticAlgorithm::random_bit(0, BITS_SIZE);
uniform_real_distribution<double> GeneticAlgorithm::random_0to1(0.0, 1.0);

GeneticAlgorithm::GeneticAlgorithm() :population(this->population_size) {
	for (int x = 0, x_pos = 0, i = 0, bit_pos; x < FORM_W; ++x, x_pos += GRID_W) {
		for (int y = 0, y_pos = 0; y < FORM_H; ++y, y_pos += GRID_H) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, GRID_W, GRID_H);
			bit_pos = BITS_SIZE - 1 - i++;
			this->bit2grid_table[bit_pos].first = x;
			this->bit2grid_table[bit_pos].second = y;
			this->grid2bit_table[x][y] = bit_pos;
		}
	}

	this->population.reserve(this->population_size + 2 * this->crossover_pair_number); // 交叉によって増加しうる分だけ集団サイズのメモリを確保
}

void GeneticAlgorithm::operator()(genome_type& best_individual) {
	this->initialize();
	ofImage img;
	for (int i = 0; i < 10; ++i) {
		cout << "before\n";
		cout << i << ": " << this->population[i].count() << endl;
		this->draw_rectangles(this->population[i]);
		img.grabScreen(0, 0, W, H);
		img.save("before" + to_string(i + 1) + ".png");

	}

	for (int i = 0; i < 1000; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}

	for (int i = 0; i < 10; ++i) {
		cout << "after\n";
		cout << i << ": " << this->population[i].count() << endl;
		this->draw_rectangles(this->population[i]);
		img.grabScreen(0, 0, W, H);
		img.save("after" + to_string(i + 1) + ".png");
	}
	best_individual = this->population[0];
}

void GeneticAlgorithm::initialize() {
	/* ランダムに初期集団を生成 */
	for (int i = 0; i < this->population_size; ++i) {
		for (int j = 0; j < BITS_SIZE; ++j) {
			this->population[i][j] = this->random_0or1(this->mt);

		}
	}

	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (x > 2 && x < 12 && y > 6 && y < 9) {
				this->population[0].set(this->grid2bit_table[x][y]);
			}
			else {
				this->population[0].reset(this->grid2bit_table[x][y]);
			}
		}
	}

	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (x > 2 && x < 4 && y > 2 && y < 8) {
				this->population[1].set(this->grid2bit_table[x][y]);
			}
			else {
				this->population[1].reset(this->grid2bit_table[x][y]);
			}
		}
	}

	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (x > 2 && x < 12 && y > 6 && y < 10) {
				this->population[2].set(this->grid2bit_table[x][y]);
			}
			else {
				this->population[2].reset(this->grid2bit_table[x][y]);
			}
		}
	}

	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (x > 10 && x < 14 && y > 2 && y < 7) {
				this->population[3].set(this->grid2bit_table[x][y]);
			}
			else {
				this->population[3].reset(this->grid2bit_table[x][y]);
			}
		}
	}

	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (x > 6 && x < 12 && y > 12 && y < 17) {
				this->population[4].set(this->grid2bit_table[x][y]);
			}
			else {
				this->population[4].reset(this->grid2bit_table[x][y]);
			}
		}
	}
}

void GeneticAlgorithm::crossover() {
	for (int i = 0; i < this->crossover_pair_number; ++i) {
		/* 交叉確率に基づいて交叉するかどうかを決める */
		if (this->random_0to1(this->mt) < this->crossover_probability) {
			/* 異なる二つの個体(親)を選ぶ */
			pair<int, int> parents_index(this->random_indivisual(this->mt), this->random_indivisual(this->mt));
			while (parents_index.first == parents_index.second) {
				parents_index.second = this->random_indivisual(this->mt);
			}

			/* 一点交叉 */
			int crossover_point = this->random_crossover_point(this->mt);
			pair<genome_type, genome_type> children;
			children.first = this->population[parents_index.first];
			children.second = this->population[parents_index.second];
			for (int j = 0; j < crossover_point; ++j) {
				children.first[j] = this->population[parents_index.second][j];
				children.second[j] = this->population[parents_index.first][j];
			}
			this->population.emplace_back(children.first);
			this->population.emplace_back(children.second);

			///* 多点交叉 */
			//pair<int, int> crossover_points(this->random_crossover_point(this->mt), this->random_crossover_point(this->mt));
			//while (crossover_points.first == crossover_points.second) {
			//	crossover_points.second = this->random_crossover_point(this->mt);
			//}
			//pair<GridForm, GridForm> children;
			//children.first.grid_bits = this->population[parents_index.first].grid_bits;
			//children.second.grid_bits = this->population[parents_index.second].grid_bits;
			//for (int j = crossover_points.first; j < crossover_points.second; ++j) {
			//	children.first.grid_bits[j] = this->population[parents_index.second].grid_bits[j];
			//	children.second.grid_bits[j] = this->population[parents_index.first].grid_bits[j];
			//}
			//this->population.emplace_back(children.first);
			//this->population.emplace_back(children.second);

		}
	}
}

void GeneticAlgorithm::mutation() {
	for (auto& i : this->population) {
		/* 突然変異率に基づいて突然変異するかどうかを決める */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			i.flip(this->random_bit(this->mt)); // ランダムな位置のビットを反転
		}
	}
}

void GeneticAlgorithm::calculate_fitness() {
	const int population_size_now = this->population.size(); // 交叉や突然変異によって増加した一時的な集団サイズ
	this->fitness.resize(population_size_now);
	fill(begin(this->fitness), end(this->fitness), 0.0);

	for (int i = 0; i < population_size_now; ++i) {
		double area = this->population[i].count(); // 面積
		if (area >= BITS_SIZE/4) {
			this->fitness[i] -= 10000 * (area - BITS_SIZE / 4);
		}
		else {
			this->fitness[i] += 2000*area;
		}


		ofPoint center_point;
		for (int j = 0; j < BITS_SIZE; ++j) {
			if (this->population[i].test(j)) {
				center_point += this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter();
			}
		}
		center_point /= area;

		for (int j = 0; j < BITS_SIZE; ++j) {
			if (this->population[i].test(j)) {
				this->fitness[i] -= euclid_distance(this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().x, this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().y, center_point.x, center_point.y);
			}
		}
	}
}

void GeneticAlgorithm::selection() {
	vector<genome_type> new_population(this->population_size);

	/* エリート主義 */
	const int elite_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	new_population[0] = this->population[elite_index];

	const double fitness_max = this->fitness[elite_index];
	const double fitness_min = *min_element(begin(this->fitness), end(this->fitness));

	// 適応度のスケーリング
	if (fitness_max != fitness_min) {
		/* 1～10000にスケーリング */
		const double scaled_fitness_max = 10000;
		const double scaled_fitness_min = 1;
		const double a = (scaled_fitness_max - scaled_fitness_min) / (fitness_max - fitness_min);
		const double b = scaled_fitness_min - a*fitness_min;
		for (auto& f : this->fitness) {
			f = a*f + b;
		}
	}
	else {
		if (fitness_min < 0) {
			for (auto & f : this->fitness) {
				f += fitness_min + 1;
			}
		}
	}

	const double fitness_sum = accumulate(begin(this->fitness), end(this->fitness), 0.0);
	discrete_distribution<int> roulette(begin(this->fitness), end(this->fitness));
	for (int i = 1; i < this->population_size; ++i) {
		new_population[i] = this->population[roulette(this->mt)];
	}

	this->population = new_population;

}

void GeneticAlgorithm::draw_rectangles(const genome_type& g) const {
	ofFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, W, H);
	ofSetColor(ofColor::blue);
	for (int i = 0; i < BITS_SIZE; ++i) {
		if (g.test(i)) {
			ofDrawRectangle(this->grid_rects[this->bit2grid_table[i].first][this->bit2grid_table[i].second]);
		}
	}
	ofNoFill();
}

double GeneticAlgorithm::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}