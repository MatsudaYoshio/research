#include <unordered_set>
#include "GeneticAlgorithm.h"

using namespace std;
using namespace param;
using namespace concurrency;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_mutation_method(1, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_block(0, GeneticAlgorithm::block_size);
uniform_real_distribution<double> GeneticAlgorithm::random_0to1(0.0, 1.0);

void GeneticAlgorithm::setup(HandCursor* hc) {
	this->hc = hc; // 手カーソル情報

	/* ブロックと座標の変換表を作成 */
	for (int y = 0, y_pos = 0, i = 0; y < this->form_h; ++y, y_pos += this->grid_h) {
		for (int x = 0, x_pos = 0; x < this->form_w; ++x, x_pos += this->grid_w) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, this->grid_w, this->grid_h);
			this->block2grid_table[i].first = x;
			this->block2grid_table[i].second = y;
			this->grid2block_table[x][y] = i++;
		}
	}

	///* 集団を管理する変数のサイズとメモリを調整 */
	//this->population.resize(this->population_size);
	//this->population_new.resize(this->population_size);

	//this->ofs.open("raw_fitness.csv");
	//this->ofs2.open("block_size.csv");
	//this->ofs3.open("scaled_fitness.csv");
}

void GeneticAlgorithm::operator()(set<long long int> selected_users_id, set<long long int> all_users_id) {
	this->selected_users_id = move(selected_users_id);
	this->all_users_id = move(all_users_id);

	//if (this->selected_users_id.empty()) {
	//	this->selected_users_num = 0;
	//	return;
	//}
	//this->tb.Start();
	//puts("!");
	this->initialize();
	//puts("!!");
	//if (this->selected_users_num > 1) {
	//this->tb.Start();
	for (int i = 0; i < this->max_iteration; ++i) {
		//this->crossover();
		//puts("!!!");
		this->mutation();
		//puts("start calculating");
		this->calculate_fitness();
		//puts("!!!!!");
		/*for (int j = 0; j < this->population_size; ++j) {
			if (this->selected_users_id.size() > 1) {
				if (j != 0) {
					this->ofs << ",";
				}
				this->ofs << this->fitness[j];
			}
		}
		if (this->selected_users_id.size() > 1) {
			this->ofs << endl;
		}*/

		this->scaling();
		//puts("!!!!!!!");
		//for (int j = 0; j < this->population_size_tmp; ++j) {
		//	if (this->selected_users_num > 1) {
		//		if (j != 0) {
		//			this->ofs3 << ",";
		//		}
		//		this->ofs3 << this->fitness[j];
		//	}
		//}
		//if (this->selected_users_num > 1) {
		//	this->ofs3 << endl;
		//}

		this->selection();
		//puts("!!!!!!!!!");
	}
	//cout << "GA time : " << this->tb.GetMs() << endl;
	//}
}

void GeneticAlgorithm::initialize() {
	//this->selected_users_num = this->selected_users_id.size();
	this->users_num = this->all_users_id.size();
	this->elite_fitness = DBL_MIN;
	fill(begin(this->elite_block_assignment), end(this->elite_block_assignment), this->NOT_USER);

	/* ユーザIDに対してインデックスを割り当てる(0から順に) */
	int i = 0;
	this->user_id2user_index.clear();
	//for (const auto& user_id : this->selected_users_id) {
	for (const auto& user_id : this->all_users_id) {
		this->user_id2user_index.emplace(make_pair(user_id, i++));
	}

	/* 各ブロックのビット数を決める */
	//this->block_bits_size = log2(this->selected_users_num) + 1; // ユーザ数と同じ分だけユーザでない状態を表せるようなビット数にする
	this->block_bits_size = log2(this->users_num) + 1;

	this->genetic_length = this->block_size*this->block_bits_size; // 遺伝子長を決める

	/* ユーザIDに割り当てられたインデックスのビット配列を作成 */
	//this->user_bits = vector<vector<bool>>(this->selected_users_num, vector<bool>(this->block_bits_size));
	this->user_bits = vector<vector<bool>>(this->users_num, vector<bool>(this->block_bits_size));
	for (int j = 0; j < i; ++j) {
		for (int k = 0; k < this->block_bits_size; ++k) {
			this->user_bits[j][k] = (j >> k) & 1;
		}
	}

	/* 各ユーザのカーソルの位置を中心に適当な領域を設ける */
	uniform_int_distribution<int> random_size(1, this->form_w / 4);
	parallel_for(0, this->population_size, [&](int i) {
		this->population[i].resize(this->genetic_length);
		fill(begin(this->population[i]), end(this->population[i]), true);

		int left, right, middle, start_block_x, start_block_y;
		//for (const auto& user : this->selected_users_id) {
		for (const auto& user : this->all_users_id) {
			/* 二分探索でユーザのカーソルに最近傍なブロックを探索し、それを初期化のためのスタートブロックとする */
			try {
				left = 0;
				right = this->form_w;
				while (right - left != 1) {
					middle = (left + right) / 2;
					if (this->hc->user_data.at(user).transformed_cursor_point.x() > this->grid_rects[middle - 1][0].getRight()) {
						left = middle;
					}
					else {
						right = middle;
					}
				}
				start_block_x = left;

				left = 0;
				right = this->form_h;
				while (right - left != 1) {
					middle = (left + right) / 2;
					if (this->hc->user_data.at(user).transformed_cursor_point.y() > this->grid_rects[0][middle - 1].getBottom()) {
						left = middle;
					}
					else {
						right = middle;
					}
				}
				start_block_y = left;
			}
			catch (std::out_of_range&) {
				continue;
			}

			int rect_size{ random_size(this->mt) };
			for (int x = max(0, start_block_x - rect_size); x < min(this->form_w, start_block_x + rect_size); ++x) {
				for (int y = max(0, start_block_y - rect_size); y < min(this->form_h, start_block_y + rect_size); ++y) {
					for (int j = 0; j < this->block_bits_size; ++j) {
						this->population[i][j + this->grid2block_table[x][y] * this->block_bits_size] = this->user_bits[this->user_id2user_index[user]][j];
					}
				}
			}
		}
	});
}

void GeneticAlgorithm::mutation() {
	uniform_int_distribution<int> random_bit(0, this->genetic_length - 1);
	for (int i = 0; i < this->population_size; ++i) {
		/* 突然変異率に基づいて突然変異するかどうかを決める */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			this->population[i][random_bit(this->mt)].flip(); // あるビットを反転
		}
	}
}

void GeneticAlgorithm::calculate_fitness() {
	fill(begin(this->fitness), end(this->fitness), 0.0);

	parallel_for(0, this->population_size, [&](int i) {
		for (const auto& user_id : this->selected_users_id) {
			this->user_block[i][user_id] = set<int>{};
		}

		/* 各ブロックに割り当てられたユーザidを求める */
		for (int j = 0; j < this->block_size; ++j) {
			this->block_assignments[i][j] = this->NOT_USER;
			//for (const auto& user_id : this->selected_users_id) {
			for (const auto& user_id : this->all_users_id) {
				for (int k = 0; k < this->block_bits_size; ++k) {
					if (this->user_bits[this->user_id2user_index[user_id]][k] != this->population[i][j*this->block_bits_size + k]) {
						goto NOT_MATCH;
					}
				}
				if (this->selected_users_id.find(user_id) != end(this->selected_users_id)) {
					this->block_assignments[i][j] = user_id;
				}
				//this->block_assignments[i][j] = user_id;
				this->user_block[i][user_id].emplace(j); // 各ユーザがもつブロックを求める
			NOT_MATCH:;
			}
		}

		/* 各ユーザの領域面積で最小なものの面積を求める */
		auto min_area{ INT_MAX };
		for (const auto& user : this->user_block[i]) {
			min_area = min(min_area, static_cast<int>(user.second.size()));
		}
		min_area *= this->grid_h*this->grid_w;

		auto perimeter_mean{ 0.0 };
		auto distance_mean{ 0.0 };
		auto block_sum{ 0 };
		auto connectivity_mean{ 0.0 };
		int x, y, nx, ny;
		for (const auto& user : this->user_block[i]) {
			try {
				/* 周囲長の平均を求める */
				for (const auto& block : user.second) {
					for (int j = 0; j < 2; ++j) {
						nx = this->block2grid_table[block].first + this->dx[j];
						ny = this->block2grid_table[block].first + this->dy[j];
						if (nx < 0 || nx >= this->form_w || ny < 0 || ny >= this->form_h) {
							continue;
						}
						if (user.first != this->block_assignments[i][this->grid2block_table[nx][ny]]) {
							perimeter_mean += this->grid_h;
						}
					}
					for (int j = 2; j < 4; ++j) {
						nx = this->block2grid_table[block].first + this->dx[j];
						ny = this->block2grid_table[block].first + this->dy[j];
						if (nx < 0 || nx >= this->form_w || ny < 0 || ny >= this->form_h) {
							continue;
						}
						if (user.first != this->block_assignments[i][this->grid2block_table[nx][ny]]) {
							perimeter_mean += this->grid_w;
						}
					}
					//}

					/* 各ブロックとカーソルとの距離の総和の平均を求める */
					distance_mean += ofDistSquared(this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().x, this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().y, this->hc->user_data.at(user.first).transformed_cursor_point.x(), this->hc->user_data.at(user.first).transformed_cursor_point.y());
					++block_sum;
				}
			}
			catch (std::out_of_range) {}

			/* 連結数の平均を求める */
			set<int> user_block_tmp{ user.second };
			while (!user_block_tmp.empty()) {
				stack<int> s;
				s.emplace(*begin(user_block_tmp));
				while (!s.empty()) {
					x = this->block2grid_table[s.top()].first;
					y = this->block2grid_table[s.top()].second;
					user_block_tmp.erase(s.top());
					s.pop();
					for (int j = 0; j < 4; ++j) {
						nx = x + this->dx[j];
						ny = y + this->dy[j];
						if (nx < 0 || nx >= this->form_w || ny < 0 || ny >= this->form_h) {
							continue;
						}
						if (user_block_tmp.find(this->grid2block_table[nx][ny]) != end(user_block_tmp)) {
							s.emplace(this->grid2block_table[nx][ny]);
						}
					}
				}
				++connectivity_mean;
			}
		}

		//perimeter_mean /= this->selected_users_num;
		perimeter_mean /= this->users_num;
		distance_mean /= block_sum;
		//connectivity_mean /= this->selected_users_num;
		connectivity_mean /= this->users_num;

		this->fitness[i] += min_area - 2 * distance_mean - 1.5*perimeter_mean - 40000 * connectivity_mean;
	});
}

void GeneticAlgorithm::scaling() {
	/* Goldberg(Genetic Algorithms in Search, Optimization, and Machine Learning)
	 線形スケーリング */

	 /* もし適応度の最小値が負ならば、全体にその分を足して正の値に調整 */
	const auto fitness_min{ *min_element(cbegin(this->fitness), cend(this->fitness)) };
	if (fitness_min < 0) {
		parallel_for_each(begin(this->fitness), end(this->fitness), [&](auto& f) {
			f -= fitness_min;
		});
	}

	/* スケーリングのためのパラメータの計算 */
	this->fitness_sum = accumulate(cbegin(this->fitness), cend(this->fitness), 0.0);
	const auto fitness_mean{ this->fitness_sum / this->population_size }; // 平均適応度
	const auto fitness_minmax{ minmax_element(cbegin(this->fitness), cend(this->fitness)) }; // 最大適応度と最小適応度のペア

	double a, b;
	if (*fitness_minmax.first > (this->fitness_multiple*fitness_mean - *fitness_minmax.second) / (this->fitness_multiple - 1.0)) {
		a = ((this->fitness_multiple - 1.0)*fitness_mean) / (*fitness_minmax.second - fitness_mean);
		b = (fitness_mean*(*fitness_minmax.second - this->fitness_multiple*fitness_mean)) / (*fitness_minmax.second - fitness_mean);
	}
	else {
		a = fitness_mean / (fitness_mean - *fitness_minmax.first);
		b = (-*fitness_minmax.first*fitness_mean) / (fitness_mean - *fitness_minmax.first);
	}

	parallel_for_each(begin(this->fitness), end(this->fitness), [&](auto& f) {
		f = a*f + b;
	});
}

void GeneticAlgorithm::selection() {
	/* エリート主義(最適個体を確保) */
	this->best_fitness_index = distance(cbegin(this->fitness), max_element(cbegin(this->fitness), cend(this->fitness)));
	this->population_new[0] = this->population[this->best_fitness_index];

	if (this->fitness[this->best_fitness_index] > this->elite_fitness) { // エリート個体よりも最適であれば
		/* エリート個体を更新 */
		this->elite_fitness = this->fitness[this->best_fitness_index];
		this->elite_individual = this->population[this->best_fitness_index];
		this->elite_block_assignment = this->block_assignments[this->best_fitness_index];
	}

	/* ルーレット方式で選択 */
	this->fitness[this->best_fitness_index] = 0;
	discrete_distribution<int> random_fitness(begin(this->fitness), end(this->fitness));
	for (int i = 0; i < this->population_size; ++i) {
		this->population_new[i] = this->population[random_fitness(this->mt)];
	}

	this->population = move(this->population_new);
}

void GeneticAlgorithm::draw() const {
	ofFill();
	for (int i = 0; i < this->block_size; ++i) {
		if (this->elite_block_assignment[i] != this->NOT_USER) {
			try {
				ofSetColor(this->hc->user_data.at(this->elite_block_assignment[i]).cursor_color, 140);
				ofDrawRectangle(this->grid_rects[this->block2grid_table[i].first][this->block2grid_table[i].second]);
			}
			catch (std::out_of_range&) {
				continue;
			}
		}
	}
	ofNoFill();
}