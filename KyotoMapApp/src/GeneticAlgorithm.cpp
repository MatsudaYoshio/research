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

void GeneticAlgorithm::setup(HandCursor* hc) {
	this->hc = hc;

	/* ���W�ƃr�b�g�̕ϊ��\���쐬 */
	for (int x = 0, x_pos = 0, i = 0, bit_pos; x < FORM_W; ++x, x_pos += GRID_W) {
		for (int y = 0, y_pos = 0; y < FORM_H; ++y, y_pos += GRID_H) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, GRID_W, GRID_H);
			bit_pos = BITS_SIZE - 1 - i++;
			this->bit2grid_table[bit_pos].first = x;
			this->bit2grid_table[bit_pos].second = y;
			this->grid2bit_table[x][y] = bit_pos;
		}
	}

	/* �W�c���Ǘ�����ϐ��̃T�C�Y�ƃ������𒲐� */
	this->population.resize(this->population_size);
	this->population.reserve(this->population_size + 2 * this->crossover_pair_number); // �����ɂ���đ��������镪�����W�c�T�C�Y�̃��������m��

	/* �����W�c�̌̌����쐬���� */
	int split_index[16][4];
	split_index[0][0] = split_index[4][0] = split_index[7][0] = split_index[11][0] = 0;
	split_index[0][1] = split_index[4][1] = split_index[8][1] = split_index[12][1] = split_index[1][0] = split_index[5][0] = split_index[9][0] = split_index[13][0] = FORM_W / 4;
	split_index[1][1] = split_index[5][1] = split_index[9][1] = split_index[13][1] = split_index[2][0] = split_index[6][0] = split_index[10][0] = split_index[14][0] = FORM_W / 2;
	split_index[2][1] = split_index[6][1] = split_index[10][1] = split_index[14][1] = split_index[3][0] = split_index[7][0] = split_index[11][0] = split_index[15][0] = FORM_W * 3 / 4;
	split_index[3][1] = split_index[7][1] = split_index[11][1] = split_index[15][1] = FORM_W;
	split_index[0][2] = split_index[1][2] = split_index[2][2] = split_index[3][2] = 0;
	split_index[0][3] = split_index[1][3] = split_index[2][3] = split_index[3][3] = split_index[4][2] = split_index[5][2] = split_index[6][2] = split_index[7][2] = FORM_H / 4;
	split_index[4][3] = split_index[5][3] = split_index[6][3] = split_index[7][3] = split_index[8][2] = split_index[9][2] = split_index[10][2] = split_index[11][2] = FORM_H / 2;
	split_index[8][3] = split_index[9][3] = split_index[10][3] = split_index[11][3] = split_index[12][2] = split_index[13][2] = split_index[14][2] = split_index[15][2] = FORM_H * 3 / 4;
	split_index[12][3] = split_index[13][3] = split_index[14][3] = split_index[15][3] = FORM_H;
	this->initial_individuals.resize(65535);
	genome_type g;
	for (int i = 1; i < 65536; ++i) {
		g.reset();
		bitset<16> bs(i);
		for (int j = 0; j < 16; ++j) {
			if (bs.test(j)) {
				for (int k = split_index[j][0]; k < split_index[j][1]; ++k) {
					for (int l = split_index[j][2]; l < split_index[j][3]; ++l) {
						g.set(this->grid2bit_table[k][l]);
					}
				}
			}
		}
		this->initial_individuals[i - 1] = g;
	}
}

void GeneticAlgorithm::operator()(genome_type& best_individual) {
	this->initialize();
	for (int i = 0; i < 100; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}
	best_individual = this->population[0];
}

void GeneticAlgorithm::operator()(const genome_type& initial_individual, genome_type& best_individual) {
	this->initialize(initial_individual);
	for (int i = 0; i < 100; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}
	best_individual = this->population[0];
}

void GeneticAlgorithm::initialize() {
	/* ���炩���ߗp�ӂ��Ă��������W�c�̌�,�����_���ȋ�`�̑g�ݍ��킹�̌�,���S�����_���̌̂�6:3:1�ŏ����W�c���`�� */

	int break_index1 = this->population_size * 3 / 5, break_index2 = this->population_size * 9 / 10;

	/* ���炩���ߗp�ӂ��Ă��������W�c�̌̂ŏ����W�c��60%���`�� */
	uniform_int_distribution<int> random_initial(0, 65534);
	for (int i = 0; i < break_index1; ++i) {
		this->population[i] = this->initial_individuals[random_initial(this->mt)];
	}

	/* �����_���ȋ�`�̑g�ݍ��킹�̌̂ŏ����W�c��30%���`�� */
	uniform_int_distribution<int> random_n(1, 5);
	uniform_int_distribution<int> random_w(2, FORM_W - 2);
	uniform_int_distribution<int> random_h(2, FORM_H - 2);
	int n, w, h, x, y;
	genome_type g;
	for (int i = break_index1 + 1; i < break_index2; ++i) {
		g.reset();
		n = random_n(this->mt);
		for (int j = 0; j < n; ++j) {
			w = random_w(this->mt);
			h = random_h(this->mt);
			uniform_int_distribution<int> random_x(0, FORM_W - w);
			uniform_int_distribution<int> random_y(0, FORM_H - h);
			x = random_x(this->mt);
			y = random_y(this->mt);
			for (int k = x; k < x + w; ++k) {
				for (int l = y; l < y + h; ++l) {
					g.set(this->grid2bit_table[k][l]);
				}
			}
		}
		this->population[i] = g;
	}

	/* ���S�����_���̌̂ŏ����W�c��10%���`�� */
	for (int i = break_index2; i < this->population_size; ++i) {
		for (int j = 0; j < BITS_SIZE; ++j) {
			this->population[i][j] = this->random_0or1(this->mt);
		}
	}
}

void GeneticAlgorithm::initialize(const param::genome_type& initial_individual) {
	this->initialize();
	swap(this->population[0], this->population[this->population_size - 1]);
	this->population[0] = initial_individual; // �擪�Ɉ����̌̂�����
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

			uniform_int_distribution<int> random_crossover_method(1, 6);

			switch (random_crossover_method(this->mt)) {
			case 1:
			{
				/* ��_���� */
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
				/* ��_���� */
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
				/* ��l���� */
				genome_type mask;
				for (int j = 0; j < BITS_SIZE; ++j) {
					mask[j] = this->random_0or1(this->mt);
				}
				this->population.emplace_back((mask & this->population[parents_index.first]) | (~mask & this->population[parents_index.second]));
				this->population.emplace_back((mask & this->population[parents_index.second]) | (~mask & this->population[parents_index.first]));
			}
			break;
			case 4:
				/* �Z�p����(AND) */
				this->population.emplace_back(this->population[parents_index.first] & this->population[parents_index.second]);
				break;
			case 5:
				/* �Z�p����(OR) */
				this->population.emplace_back(this->population[parents_index.first] | this->population[parents_index.second]);
				break;
			case 6:
				/* �Z�p����(XOR) */
				this->population.emplace_back(this->population[parents_index.first] ^ this->population[parents_index.second]);
				break;
			}
		}
	}
}

void GeneticAlgorithm::mutation() {
	uniform_int_distribution<int> random_mutation_method(1, 3);
	for (int i = 0; i < this->population_size; ++i) {
		/* �ˑR�ψٗ��Ɋ�Â��ēˑR�ψق��邩�ǂ��������߂� */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			switch (random_mutation_method(this->mt)) {
			case 1:
				/* ���ׂẴr�b�g�𔽓] */
				this->population[i].flip();
				break;
			case 2:
			{
				/* �t��(����͈̔͂̃r�b�g�̏������t�ɂ���) */
				int s = this->random_bit(this->mt), t = this->random_bit(this->mt);
				if (s > t) {
					swap(s, t);
				}
				reverse(&this->population[s], &this->population[t]);
			}
			break;
			case 3:
			{
				/* �X�N�����u��(����͈̔͂̃r�b�g�̏����������_���ɕ��בւ���) */
				int s = this->random_bit(this->mt), t = this->random_bit(this->mt);
				if (s > t) {
					swap(s, t);
				}
				shuffle(&this->population[s], &this->population[t], this->mt);
			}
			break;
			}

		}
	}
}

void GeneticAlgorithm::calculate_fitness() {
	const int population_size_now = this->population.size(); // ������ˑR�ψقɂ���đ��������ꎞ�I�ȏW�c�T�C�Y
	this->fitness.resize(population_size_now);
	fill(begin(this->fitness), end(this->fitness), 0.0);

	for (int i = 0; i < population_size_now; ++i) {
		double area = this->population[i].count(); // �ʐ�
		if (area >= BITS_SIZE / 4) {
			this->fitness[i] -= 100000 * (area - BITS_SIZE / 4);
		}
		else {
			this->fitness[i] += 2000 * area;
		}


		ofPoint center_point; // �d�S
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

		/* �A���� */
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

		/* ���͒� */
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

		/* �I�C���[�� */
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

		/* �J�[�\������̋��� */
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

	/* �G���[�g��` */
	const int elite_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	new_population[0] = this->population[elite_index];

	const double fitness_max = this->fitness[elite_index];
	const double fitness_min = *min_element(begin(this->fitness), end(this->fitness));

	// �K���x�̃X�P�[�����O
	if (fitness_max != fitness_min) {
		/* 1�`10000�ɃX�P�[�����O */
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
	/*ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, W, H);*/
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