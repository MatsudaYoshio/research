#include <unordered_set>
#include "GeneticAlgorithm.h"

using namespace std;
using namespace param;
using namespace concurrency;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_indivisual(0, GeneticAlgorithm::population_size - 1);
uniform_int_distribution<int> GeneticAlgorithm::random_crossover_method(1, 5);
uniform_int_distribution<int> GeneticAlgorithm::random_mutation_method(1, 3);
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

	/* 集団を管理する変数のサイズとメモリを調整 */
	this->population.resize(this->population_size);
	this->population_new.resize(this->population_size);
	this->population.reserve(this->population_size + 2 * this->crossover_pair_number); // 交叉によって増加しうる分だけ集団サイズのメモリを確保

	this->ofs.open("raw_fitness.csv");
	//this->ofs2.open("block_size.csv");
	this->ofs3.open("scaled_fitness.csv");
}

void GeneticAlgorithm::operator()(set<long long int>& selected_users_id, set<long long int>& all_users_id) {
	if (selected_users_id.empty()) {
		this->selected_users_num = 0;
		return;
	}
	//this->tb.Start();
	this->initialize(selected_users_id, all_users_id);
	//if (this->selected_users_num > 1) {
	this->tb.Start();
	for (int i = 0; i < this->max_iteration; ++i) {
		//this->crossover();
		this->mutation();
		this->calculate_fitness();

		//for (int j = 0; j < this->population_size_tmp; ++j) {
		//	if (this->selected_users_num > 1) {
		//		if (j != 0) {
		//			this->ofs << ",";
		//		}
		//		this->ofs << this->fitness[j];
		//	}
		//}
		//if (this->selected_users_num > 1) {
		//	this->ofs << endl;
		//}

		this->scaling();

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
	}
	cout << "GA time : " << this->tb.GetMs() << endl;
	//}
}

void GeneticAlgorithm::initialize(set<long long int>& selected_users_id, set<long long int>& all_users_id) {
	this->all_users_id = all_users_id;
	this->selected_users_id = selected_users_id;
	this->selected_users_num = this->selected_users_id.size();
	this->elite_fitness = DBL_MIN;

	/* ユーザIDに対してインデックスを割り当てる(0から順に) */
	int i = 0;
	this->user_id2user_index.clear();
	for (const auto& user_id : this->selected_users_id) {
		this->user_id2user_index.emplace(make_pair(user_id, i++));
	}

	/* 各ブロックのビット数を決める */
	this->block_bits_size = log2(this->selected_users_num) + 1; // ユーザ数と同じ分だけユーザでない状態を表せるようなビット数にする

	this->genetic_length = this->block_size*this->block_bits_size; // 遺伝子長を決める

	/* ユーザIDに割り当てられたインデックスのビット配列を作成 */
	this->user_bits = vector<vector<bool>>(this->selected_users_num, vector<bool>(this->block_bits_size));
	for (int j = 0; j < i; ++j) {
		for (int k = 0; k < this->block_bits_size; ++k) {
			this->user_bits[j][k] = (j >> k) & 1;
		}
	}

	/* 各ユーザの顔の位置を中心に適当な領域を設ける */
	uniform_int_distribution<int> random_size(1, this->form_w / 4);
	parallel_for(0, this->population_size, [&](int i) {
		this->population[i].resize(this->genetic_length);
		fill(begin(this->population[i]), end(this->population[i]), true);

		int start_block;
		double dist_tmp, min_dist;

		for (const auto& user : this->selected_users_id) {
			min_dist = DBL_MAX;
			try {
				for (int b = 0; b < this->block_size; ++b) {
					dist_tmp = ofDist(this->hc->user_data.at(user).transformed_face_point.x(), this->hc->user_data.at(user).transformed_face_point.y(), this->grid_rects[this->block2grid_table[b].first][this->block2grid_table[b].second].getCenter().x, this->grid_rects[this->block2grid_table[b].first][this->block2grid_table[b].second].getCenter().y);
					if (dist_tmp < min_dist) {
						min_dist = dist_tmp;
						start_block = b;
					}
				}
			}
			catch (std::out_of_range&) {
				continue;
			}

			int rect_size{ random_size(this->mt) };
			for (int x = max(0, this->block2grid_table[start_block].first - rect_size); x < min(this->form_w, this->block2grid_table[start_block].first + rect_size); ++x) {
				for (int y = max(0, this->block2grid_table[start_block].second - rect_size); y < min(this->form_h, this->block2grid_table[start_block].second + rect_size); ++y) {
					for (int j = 0; j < this->block_bits_size; ++j) {
						this->population[i][j + this->grid2block_table[x][y] * this->block_bits_size] = this->user_bits[this->user_id2user_index[user]][j];
					}
				}
			}
		}
	});
}

void GeneticAlgorithm::crossover() {
	for (int i = 0; i < this->crossover_pair_number; ++i) {
		if (this->random_0to1(this->mt) < this->crossover_probability) { // 交叉確率に基づいて交叉するかどうかを決める
			/* 異なる二つの個体(親)を選ぶ */
			pair<int, int> parents_index(this->random_indivisual(this->mt), this->random_indivisual(this->mt));
			while (parents_index.first == parents_index.second) {
				parents_index.second = this->random_indivisual(this->mt);
			}

			/* 交叉手法をランダムに選ぶ */
			switch (this->random_crossover_method(this->mt)) {
			case 1:
			{
				/* 一点交叉 */
				uniform_int_distribution<int> random_crossover_block(1, this->block_size - 2);
				const int crossover_block = random_crossover_block(this->mt);
				pair<genome_type, genome_type> children(this->population[parents_index.first], this->population[parents_index.second]);
				for (int j = 0; j < crossover_block*this->block_bits_size; ++j) {
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
				uniform_int_distribution<int> random_crossover_block(1, this->block_size - 2);
				pair<int, int> crossover_blocks(random_crossover_block(this->mt), random_crossover_block(this->mt));
				while (crossover_blocks.first == crossover_blocks.second) {
					crossover_blocks.second = random_crossover_block(this->mt);
				}
				pair<genome_type, genome_type> children(this->population[parents_index.first], this->population[parents_index.second]);
				for (int j = crossover_blocks.first*this->block_bits_size; j < crossover_blocks.second*this->block_bits_size; ++j) {
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
				genome_type mask(this->block_size);
				for (int j = 0; j < this->block_size; ++j) {
					mask[j] = this->random_0or1(this->mt);
				}
				pair<genome_type, genome_type> children(this->population[parents_index.first], this->population[parents_index.second]);
				for (int j = 0; j < this->block_size; ++j) {
					if (mask[j]) {
						for (int k = 0; k < this->block_bits_size; ++k) {
							children.first[j*this->block_bits_size + k] = this->population[parents_index.second][j*this->block_bits_size + k];
							children.second[j*this->block_bits_size + k] = this->population[parents_index.first][j*this->block_bits_size + k];
						}
					}
				}
				this->population.emplace_back(children.first);
				this->population.emplace_back(children.second);
			}
			break;
			case 4:
			{
				/* 直線交叉(水平) */
				uniform_int_distribution<int> random_line(1, this->form_h - 1);
				int line = random_line(this->mt);
				pair<genome_type, genome_type> children(this->population[parents_index.first], this->population[parents_index.second]);
				for (int j = 0; j < line*this->form_w*this->block_bits_size; ++j) {
					children.first[j] = this->population[parents_index.second][j];
					children.second[j] = this->population[parents_index.first][j];
				}
				this->population.emplace_back(children.first);
				this->population.emplace_back(children.second);
			}
			break;
			case 5:
			{
				/* 直線交叉(垂直) */
				uniform_int_distribution<int> random_line(1, this->form_w - 1);
				int line = random_line(this->mt);
				pair<genome_type, genome_type> children(this->population[parents_index.first], this->population[parents_index.second]);
				for (int y = 0; y < this->form_h; ++y) {
					for (int j = this->grid2block_table[0][y] * this->block_bits_size; j < (this->grid2block_table[0][y] + line) * this->block_bits_size; ++j) {
						children.first[j] = this->population[parents_index.second][j];
						children.second[j] = this->population[parents_index.first][j];
					}
				}
				this->population.emplace_back(children.first);
				this->population.emplace_back(children.second);
			}
			}
		}
	}
}

void GeneticAlgorithm::mutation() {
	//uniform_int_distribution<int> random_mutation_method(1, 3);
	//set<int> active_block;
	//set<int> active_bit;
	//vector<int> v;
	//if (this->user_block.empty()) {
	//	return;
	//}
	//for (const auto& user : this->user_block) {
	//	for (const auto& block : user.second) {
	//		active_block.emplace(block);
	//	}
	//}

	//set<int> active_block_tmp = active_block;
	//int nx, ny;
	//for (const auto& b : active_block_tmp) {
	//	for (int i = 0; i < 8; ++i) {
	//		nx = this->block2grid_table[b].first + this->dx[i];
	//		ny = this->block2grid_table[b].second + this->dy[i];
	//		if (nx < 0 || nx >= this->form_w || ny < 0 || ny >= this->form_h) {
	//			continue;
	//		}
	//		for (int j = 0; j < this->block_bits_size; ++j) {
	//			active_bit.emplace(this->grid2block_table[nx][ny] + j);
	//		}
	//		active_block.emplace(this->grid2block_table[nx][ny]);
	//	}
	//}

	//int x = 0;
	//v.resize(active_bit.size());
	//for (const auto& b : active_bit) {
	//	v[x++] = b;
	//}

	uniform_int_distribution<int> random_bit(0, this->genetic_length - 1);
	//uniform_int_distribution<int> random_active_bit(0, v.size() - 1);
	for (int i = 0; i < this->population_size; ++i) {
		/* 突然変異率に基づいて突然変異するかどうかを決める */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			//for (int j = 0; j < 10; ++j) {
			int b = random_bit(this->mt);
			//int b = random_active_bit(this->mt);
			//if (active_block.find(b / this->block_bits_size) != end(active_block)) {
			this->population[i][b].flip();
			//}
		}
		///* 突然変異手法をランダムに選ぶ */
		//switch (random_mutation_method(this->mt)) {
		//case 1:
		//	/* すべてのビットを反転 */
		//	this->population[i].flip();
		//	break;
		//case 2:
		//{
		//	/* 逆位(特定の範囲のビットの順序を逆にする) */
		//	int s = this->random_bit(this->mt), t = this->random_bit(this->mt);
		//	if (s > t) {
		//		swap(s, t);
		//	}
		//	reverse(&this->population[s], &this->population[t]);
		//}
		//break;
		//case 3:
		//{
		//	/* スクランブル(特定の範囲のビットの順序をランダムに並べ替える) */
		//	int s = this->random_bit(this->mt), t = this->random_bit(this->mt);
		//	if (s > t) {
		//		swap(s, t);
		//	}
		//	shuffle(&this->population[s], &this->population[t], this->mt);
		//}
		//break;
		//}

	}
}

void GeneticAlgorithm::calculate_fitness() {
	this->population_size_tmp = this->population.size();

	this->fitness.resize(this->population_size_tmp);
	fill(begin(this->fitness), end(this->fitness), 0.0);

	this->block_assignments.resize(this->population_size_tmp);

	this->user_block.clear();
	this->user_block.resize(this->population_size_tmp);

	//#pragma omp parallel for
	//	for (int i = 0; i < this->population_size_tmp; ++i) {
	//		/* 各ブロックに割り当てられたユーザidを求める */
	//		for (int j = 0; j < this->block_size; ++j) {
	//			this->block_assignments[i][j] = this->NOT_USER;
	//			for (const auto& user_id : this->selected_users_id) {
	//				bool flag = true;
	//				for (int k = 0; k < this->block_bits_size; ++k) {
	//					if (((this->user_id_index[user_id] >> k) & 1) != this->population[i][j*this->block_bits_size + k]) {
	//						flag = false;
	//						break;
	//					}
	//				}
	//				if (flag) {
	//					this->block_assignments[i][j] = user_id;
	//					this->user_block[i][user_id].emplace(j); // 各ユーザがもつブロックを求める
	//					break;
	//				}
	//			}
	//		}
	//
	//		/*** 面積 ***/
	//		/* 各ユーザの領域面積で最小なものを最大化する */
	//		int min_area = INT_MAX;
	//		for (const auto& user : this->user_block[i]) {
	//			min_area = min(min_area, static_cast<int>(user.second.size()));
	//		}
	//		min_area *= this->grid_h*this->grid_w;
	//
	//		/* 領域の重心 */
	//		unordered_map<int, ofPoint> center_points(this->selected_users_num); // 各ユーザの重心
	//		for (const auto& user : this->user_block[i]) {
	//			center_points.emplace(make_pair(user.first, ofPoint(0, 0)));
	//			for (const auto& block : user.second) {
	//				center_points[user.first].x += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().x;
	//				center_points[user.first].y += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().y;
	//			}
	//			center_points[user.first].x /= user.second.size();
	//			center_points[user.first].y /= user.second.size();
	//		}
	//
	//		double distance{ 0 };
	//
	//		/* 領域の重心から顔との距離を求める */
	//		for (const auto& user : this->user_block[i]) {
	//			try {
	//				distance -= this->euclid_distance(center_points[user.first].x, center_points[user.first].y, this->hc->user_data.at(user.first).transformed_face_point.x(), this->hc->user_data.at(user.first).transformed_face_point.y());
	//			}
	//			catch (std::out_of_range&) {
	//				continue;
	//			}
	//		}
	//
	//		//if (this->selected_users_num > 1) {
	//		//	if (i != 0) {
	//		//		this->ofs << ",";
	//		//		//this->ofs2 << ",";
	//		//	}
	//		//	this->ofs << this->fitness[i];
	//		//	//this->ofs2 << min_area;
	//		//}
	//
	//		///* 他のユーザのカーソルからの距離を求める */
	//		//for (const auto& main_user : this->user_block) {
	//		//	for (const auto& other_user : this->all_users_id) {
	//		//		if (main_user.first == other_user) {
	//		//			continue;
	//		//		}
	//		//		try {
	//		//			for (const auto& block : main_user.second) {
	//		//				this->fitness[i] += 1000 * this->euclid_distance(center_points[main_user.first].x, center_points[main_user.first].y, CAMERA_W - hc->track_data.at(other_user).current_pointer.x, hc->track_data.at(other_user).current_pointer.y);
	//		//			}
	//		//		}
	//		//		catch (std::out_of_range&) {}
	//		//	}
	//		//}
	//
	//		this->fitness[i] += 1 * min_area + 1000 * distance;
	//	}
		//if (this->selected_users_num > 1) {
		//	this->ofs << endl;
		//	//this->ofs2 << endl;
		//}

	parallel_for(0, this->population_size_tmp, [&](int i) {
		/* 各ブロックに割り当てられたユーザidを求める */
		for (int j = 0; j < this->block_size; ++j) {
			this->block_assignments[i][j] = this->NOT_USER;
			for (const auto& user_id : this->selected_users_id) {
				for (int k = 0; k < this->block_bits_size; ++k) {
					if (this->user_bits[this->user_id2user_index[user_id]][k] != this->population[i][j*this->block_bits_size + k]) {
						goto NOT_MATCH;
					}
				}
				this->block_assignments[i][j] = user_id;
				this->user_block[i][user_id].emplace(j); // 各ユーザがもつブロックを求める
			NOT_MATCH:;
			}
		}

		/* 各ユーザの領域面積で最小なものを最大化する */
		int min_area{ INT_MAX };
		for (const auto& user : this->user_block[i]) {
			min_area = min(min_area, static_cast<int>(user.second.size()));
		}
		min_area *= this->grid_h*this->grid_w;

		/* 領域の重心から顔との距離を求める */
		unordered_map<int, ofPoint> center_points(this->selected_users_num); // 各ユーザの重心
		double distance{ 0 };
		for (const auto& user : this->user_block[i]) {
			center_points.emplace(make_pair(user.first, ofPoint(0, 0)));
			for (const auto& block : user.second) {
				center_points[user.first].x += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().x;
				center_points[user.first].y += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().y;
			}
			center_points[user.first].x /= user.second.size();
			center_points[user.first].y /= user.second.size();

			try {
				distance -= ofDist(center_points[user.first].x, center_points[user.first].y, this->hc->user_data.at(user.first).transformed_face_point.x(), this->hc->user_data.at(user.first).transformed_face_point.y());
			}
			catch (std::out_of_range&) {
				continue;
			}
		}

		this->fitness[i] += 1 * min_area + 1000 * distance;
	});
}

void GeneticAlgorithm::scaling() {
	/* Goldberg(Genetic Algorithms in Search, Optimization, and Machine Learning)
	 線形スケーリング */

	 /* スケーリングのためのパラメータの計算 */
	this->fitness_sum = accumulate(cbegin(this->fitness), cend(this->fitness), 0.0);
	const auto fitness_mean{ this->fitness_sum / this->population_size_tmp }; // 平均適応度
	const auto fitness_minmax{ minmax_element(cbegin(this->fitness), cend(this->fitness)) }; // 最大適応度と最小適応度のペア
	this->best_fitness_index = distance(cbegin(this->fitness), fitness_minmax.second);
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

	/* もし適応度の最小値が負ならば、全体にその分を足して正の値に調整 */
	const auto fitness_min{ *min_element(cbegin(this->fitness), cend(this->fitness)) };
	if (fitness_min < 0) {
		parallel_for_each(begin(this->fitness), end(this->fitness), [&](auto& f) {
			f -= fitness_min;
		});
	}
}

void GeneticAlgorithm::selection() {
	/* エリート主義(最適個体を確保) */
	this->population_new[0] = this->population[this->best_fitness_index];

	if (this->fitness[this->best_fitness_index] > this->elite_fitness) { // エリート個体よりも最適であれば
		/* エリート個体を更新 */
		this->elite_fitness = this->fitness[this->best_fitness_index];
		this->elite_individual = this->population[this->best_fitness_index];
		this->elite_block_assignment = this->block_assignments[this->best_fitness_index];
	}

	/** ルーレット方式で選択 **/
	uniform_real_distribution<double> random_fitness(0.0, this->fitness_sum);
	parallel_for(1, this->population_size, [&](int i) {
		vector<int> v(this->population_size_tmp);
		std::iota(begin(v), end(v), 0);
		shuffle(begin(v), end(v), this->mt);
		double s{ 0.0 };
		double r{ random_fitness(this->mt) };
		for (int j = 0; j < this->population_size_tmp; ++j) {
			s += this->fitness[v[j]];
			if (s >= r) {
				this->population_new[i] = this->population[v[j]];
				break;
			}
		}
	});

	this->population = this->population_new;
}

void GeneticAlgorithm::draw() const {
	ofFill();
	for (int i = 0; i < this->block_size; ++i) {
		if (this->elite_block_assignment[i] != this->NOT_USER) {
			try {
				ofSetColor(this->hc->user_data.at(this->elite_block_assignment[i]).cursor_color, 150);
			}
			catch (std::out_of_range&) {
				continue;
			}
			ofDrawRectangle(this->grid_rects[this->block2grid_table[i].first][this->block2grid_table[i].second]);
		}
	}
	ofNoFill();
}