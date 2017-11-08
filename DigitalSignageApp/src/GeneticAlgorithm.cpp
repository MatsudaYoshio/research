#include <unordered_set>
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

void GeneticAlgorithm::setup(HandCursor* hc) {
	this->hc = hc;
}

void GeneticAlgorithm::operator()(genome_type& best_individual) {
	this->initialize();
	ofImage img;
	for (int i = 0; i < 10; ++i) {
		//cout << "before\n";
		//cout << i << ": " << this->population[i].count() << endl;
		//this->draw_rectangles(this->population[i]);
		//img.grabScreen(0, 0, W, H);
		//img.save("before" + to_string(i + 1) + ".png");

	}

	for (int i = 0; i < 500; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}

	for (int i = 0; i < 10; ++i) {
		//cout << "after\n";
		//cout << i << ": " << this->population[i].count() << endl;
		//this->draw_rectangles(this->population[i]);
		//img.grabScreen(0, 0, W, H);
		//img.save("after" + to_string(i + 1) + ".png");
	}
	best_individual = this->population[0];
}

void GeneticAlgorithm::operator()(const genome_type& initial_individual, genome_type& best_individual) {
	this->initialize();
	this->population[this->population_size - 1] = initial_individual;
	for (int i = 0; i < 500; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
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

	this->population[5].reset();
	for (int x = FORM_W / 2; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			this->population[5].set(this->grid2bit_table[x][y]);
		}
	}

	this->population[6].reset();
	for (int x = FORM_W / 4; x < FORM_W * 3 / 4; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			this->population[6].set(this->grid2bit_table[x][y]);
		}
	}

	this->population[7].reset();
	for (int x = 0; x < FORM_W; ++x) {
		if ((x >= FORM_W / 4 && x < FORM_W / 2) || x >= FORM_W * 3 / 4) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[7].set(this->grid2bit_table[x][y]);
			}
		}
	}

	this->population[8].reset();
	for (int x = 0; x < FORM_W; ++x) {
		if ((x >= FORM_W / 8 && x <= FORM_W / 4) || (x >= FORM_W * 5 / 8 && x <= FORM_W * 3 / 4)) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[8].set(this->grid2bit_table[x][y]);
			}
		}
	}

	this->population[9].reset();
	for (int x = 0; x < FORM_W; ++x) {
		if ((x >= FORM_W / 8 && x <= FORM_W / 4) || (x >= FORM_W / 4 && x < FORM_W * 5 / 8) || x >= FORM_W * 7 / 8) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[9].set(this->grid2bit_table[x][y]);
			}
		}
	}

	this->population[10].reset();
	for (int x = 0; x < FORM_W; ++x) {
		if ((x >= FORM_W / 8 && x < FORM_W / 4) || (x >= FORM_W * 3 / 8 && x <= FORM_W / 2) || (x >= FORM_W * 3 / 4 && x < FORM_W * 7 / 8)) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[10].set(this->grid2bit_table[x][y]);
			}
		}
	}

	this->population[11].reset();
	for (int x = 0; x < FORM_W; ++x) {
		if ((x >= FORM_W / 8 && x < FORM_W / 4) || (x >= FORM_W *3 / 8 && x < FORM_W / 2) || (x >= FORM_W * 5 / 8 && x < FORM_W * 3 / 4) || x >= FORM_W * 7 / 8) {
			for (int y = 0; y < FORM_H; ++y) {
				this->population[11].set(this->grid2bit_table[x][y]);
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

			uniform_int_distribution<int> random_crossover_method(1, 6);

			switch (random_crossover_method(this->mt)) {
			case 1:
			{
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
			}
			break;
			case 2:
			{
				/* 二点交叉 */
				pair<int, int> crossover_points(this->random_crossover_point(this->mt), this->random_crossover_point(this->mt));
				while (crossover_points.first == crossover_points.second) {
					crossover_points.second = this->random_crossover_point(this->mt);
				}
				pair<genome_type, genome_type> children;
				children.first = this->population[parents_index.first];
				children.second = this->population[parents_index.second];
				for (int j = crossover_points.first; j < crossover_points.second; ++j) {
					children.first[j] = this->population[parents_index.second][j];
					children.second[j] = this->population[parents_index.first][j];
				}
				this->population.emplace_back(children.first);
				this->population.emplace_back(children.second);
			}
			break;
			case 3:
			{
				/* 一様交叉 */
				genome_type mask;
				for (int j = 0; j < BITS_SIZE; ++j) {
					mask[j] = this->random_0or1(this->mt);
				}
				this->population.emplace_back((mask & this->population[parents_index.first]) | (~mask & this->population[parents_index.second]));
				this->population.emplace_back((mask & this->population[parents_index.second]) | (~mask & this->population[parents_index.first]));
			}
			break;
			case 4:
				/* 算術交叉(AND) */
				this->population.emplace_back(this->population[parents_index.first] & this->population[parents_index.second]);
				break;
			case 5:
				/* 算術交叉(OR) */
				this->population.emplace_back(this->population[parents_index.first] | this->population[parents_index.second]);
				break;
			case 6:
				/* 算術交叉(XOR) */
				this->population.emplace_back(this->population[parents_index.first] ^ this->population[parents_index.second]);
				break;
			}
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
		if (area >= BITS_SIZE / 4) {
			this->fitness[i] -= 100000 * (area - BITS_SIZE / 4);
		}
		else {
			this->fitness[i] += 2000 * area;
		}


		ofPoint center_point; // 重心
		for (int j = 0; j < BITS_SIZE; ++j) {
			if (this->population[i].test(j)) {
				center_point += this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter();
			}
		}
		center_point /= area;

		for (int j = 0; j < BITS_SIZE; ++j) {
			if (this->population[i].test(j)) {
				this->fitness[i] -= 5 * euclid_distance(this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().x, this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().y, center_point.x, center_point.y);
			}
		}

		/* 連結数 */
		int connectivity_number = 0;
		/*int dx[] = { 1,1,0,-1,-1,-1,0,1 };
		int dy[] = { 0,-1,-1,-1,0,1,1,1 };*/
		int dx[] = { 1, 0, -1, 0 };
		int dy[] = { 0, -1, 0, 1 };
		int x, y, nx, ny;
		bool flag;
		stack<int> s;
		genome_type genome_tmp = this->population[i];
		unordered_set<int> us;
		for (int j = 0; j < BITS_SIZE; ++j) {
			if (genome_tmp.test(j)) {
				us.emplace(j);
			}
		}
		if (!us.empty()) {
			++connectivity_number;
			s.push(*begin(us));
			while (!s.empty()) {
				x = this->bit2grid_table[s.top()].first;
				y = this->bit2grid_table[s.top()].second;
				genome_tmp.reset(s.top());
				us.erase(s.top());
				s.pop();
				flag = false;
				for (int k = 0; k < 4; ++k) {
					nx = x + dx[k];
					ny = y + dy[k];
					if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
						continue;
					}
					if (genome_tmp.test(this->grid2bit_table[nx][ny])) {
						s.push(this->grid2bit_table[nx][ny]);
						flag = true;
					}
				}
				if (!flag && s.empty()) {
					if (us.empty()) {
						break;
					}
					else {
						s.push(*begin(us));
						++connectivity_number;
					}
				}
			}
		}

		if (connectivity_number == 1) {
			this->fitness[i] += 10000;
		}

		/* 周囲長 */
		int arc_length = 0;
		for (int j = 0; j < BITS_SIZE; ++j) {
			if (population[i].test(j)) {
				for (int k = 0; k < 4; ++k) {
					nx = this->bit2grid_table[j].first + dx[k];
					ny = this->bit2grid_table[j].second + dy[k];
					if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
						continue;
					}
					if (!population[i].test(this->grid2bit_table[nx][ny])) {
						++arc_length;
					}
				}
			}
		}
		this->fitness[i] -= 100 * arc_length;

		/* オイラー数 */
		int e = 0, f = 0;
		for (int j = 0; j < BITS_SIZE; ++j) {
			if (population[i].test(j)) {
				for (int k = 0; k < 2; ++k) {
					nx = this->bit2grid_table[j].first + dx[k];
					ny = this->bit2grid_table[j].second + dy[k];
					if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
						continue;
					}
					if (population[i].test(this->grid2bit_table[nx][ny])) {
						++e;
					}
				}
				if (this->bit2grid_table[j].first == FORM_W - 1 || this->bit2grid_table[j].second == FORM_H - 1) {
					continue;
				}
				if (population[i].test(this->grid2bit_table[this->bit2grid_table[j].first + 1][this->bit2grid_table[j].second]) && population[i].test(this->grid2bit_table[this->bit2grid_table[j].first][this->bit2grid_table[j].second + 1]) && population[i].test(this->grid2bit_table[this->bit2grid_table[j].first + 1][this->bit2grid_table[j].second + 1])) {
					++f;
				}
			}
		}
		int euler_number = area - e + f;

		this->fitness[i] -= 1000 * (connectivity_number - euler_number);

		/* カーソルからの距離 */
		for (const auto& td : this->hc->track_data) {
			for (int j = 0; j < BITS_SIZE; ++j) {
				if (population[i].test(j)) {
					this->fitness[i] += 100 * this->euclid_distance(this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().x, this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().y, W - td.second.current_pointer.x, td.second.current_pointer.y);
				}
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