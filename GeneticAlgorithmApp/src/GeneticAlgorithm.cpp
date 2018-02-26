#include <unordered_set>
#include "GeneticAlgorithm.h"

using namespace std;
using namespace param;

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
	this->population.reserve(this->population_size + 2 * this->crossover_pair_number); // 交叉によって増加しうる分だけ集団サイズのメモリを確保

	this->elite_fitness = DBL_MIN;

	this->selected_users_num = 0;

	this->ofs.open("raw_fitness.csv");
	//this->ofs2.open("block_size.csv");
	this->ofs3.open("scaled_fitness.csv");
}

void GeneticAlgorithm::operator()(set<long long int>& selected_users_id, set<long long int>& all_users_id) {
	if (selected_users_id.empty()) {
		this->selected_users_num = 0;
		return;
	}
	this->tb.Start();
	this->initialize(selected_users_id, all_users_id);
	//if (this->selected_users_num > 1) {
	this->tb.Start();
	for (int i = 0; i < 300; ++i) {
		//this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}
	cout << "GA time : " << this->tb.GetMs() << endl;
	//}
}

void GeneticAlgorithm::initialize(set<long long int>& selected_users_id, set<long long int>& all_users_id) {
	this->all_users_id = all_users_id;
	this->selected_users_id = selected_users_id;
	bool flag = true;
	if (this->selected_users_num != this->selected_users_id.size()) {
		flag = false;
		this->selected_users_num = this->selected_users_id.size();
		this->elite_fitness = DBL_MIN;
	}

	/* ユーザIDに対してインデックスを割り当てる(0から順に) */
	int i = 0;
	this->user_id_index.clear();
	for (const auto& user_id : this->selected_users_id) {
		this->user_id_index.emplace(make_pair(user_id, i++));
	}

	/* 各ブロックのビット数を決める */
	this->block_bits_size = log2(this->selected_users_num) + 1; // ユーザ数と同じ分だけユーザでない状態を表せるようなビット数にする
	// this->block_bits_size = 1;
	// while (pow(2, this->block_bits_size++) > 2 * this->selected_users_num) {}

	this->genetic_length = this->block_size*this->block_bits_size; // 遺伝子長を決める

	/* ユーザIDに割り当てられたインデックスのビット配列を作成 */
	vector<vector<int>> user_bits = vector<vector<int>>(this->selected_users_num, vector<int>(this->block_bits_size));
	for (int j = 0; j < i; ++j) {
		for (int k = 0; k < this->block_bits_size; ++k) {
			user_bits[j][k] = (j >> k) & 1;
		}
	}

	/* 各ユーザの顔の位置を中心に適当な領域を設ける */
	uniform_int_distribution<int> random_size(1, this->form_w / 4);
	int start_block, rect_size;
	double dist_tmp, min_dist;
	for (i = 0; i < this->population_size; ++i) {
		this->population[i].resize(this->genetic_length);
		fill(begin(this->population[i]), end(this->population[i]), true);
		for (const auto& user : this->selected_users_id) {
			min_dist = DBL_MAX;
			try {
				for (int b = 0; b < this->block_size; ++b) {
					dist_tmp = this->euclid_distance(this->hc->user_data.at(user).transformed_face_point.x(), this->hc->user_data.at(user).transformed_face_point.y(), this->grid_rects[this->block2grid_table[b].first][this->block2grid_table[b].second].getCenter().x, this->grid_rects[this->block2grid_table[b].first][this->block2grid_table[b].second].getCenter().y);
					if (dist_tmp < min_dist) {
						min_dist = dist_tmp;
						start_block = b;
					}
				}
			}
			catch (std::out_of_range&) {
				continue;
			}

			rect_size = random_size(this->mt);
			for (int x = max(0, this->block2grid_table[start_block].first - rect_size); x < min(this->form_w, this->block2grid_table[start_block].first + rect_size); ++x) {
				for (int y = max(0, this->block2grid_table[start_block].second - rect_size); y < min(this->form_h, this->block2grid_table[start_block].second + rect_size); ++y) {
					for (int j = 0; j < this->block_bits_size; ++j) {
						this->population[i][j + this->grid2block_table[x][y] * this->block_bits_size] = user_bits[this->user_id_index[user]][j];
					}
				}
			}
		}
	}

	if (flag && !this->elite_individual.empty()) {
		this->population[0] = this->elite_individual;
	}
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

	//for (int i = 0; i < this->population_size_tmp; ++i) {
	//	/* 各ブロックに割り当てられたユーザidを求める */
	//	for (int j = 0; j < this->block_size; ++j) {
	//		this->block_assignments[i][j] = this->NOT_USER;
	//		for (const auto& user_id : this->selected_users_id) {
	//			bool flag = true;
	//			for (int k = 0; k < this->block_bits_size; ++k) {
	//				if (((this->user_id_index[user_id] >> k) & 1) != this->population[i][j*this->block_bits_size + k]) {
	//					flag = false;
	//					break;
	//				}
	//			}
	//			if (flag) {
	//				this->block_assignments[i][j] = user_id;
	//				this->user_block[i][user_id].emplace(j); // 各ユーザがもつブロックを求める
	//				break;
	//			}
	//		}
	//	}

	//	/*** 面積 ***/

	//	/* 各ユーザの領域面積で最小なものを最大化する */
	//	int min_area = INT_MAX;
	//	for (const auto& user : this->user_block[i]) {
	//		min_area = min(min_area, static_cast<int>(user.second.size()));
	//	}

	//	this->fitness[i] += min_area*this->grid_h*this->grid_w;

	//	//ofs << min_area*GRID_H*GRID_W << endl;

	//	//if (min_area <= BLOCK_SIZE / 4) {
	//	//	this->fitness[i] += 1000000 * exp(-pow(min_area - BLOCK_SIZE / 4, 2));
	//	//}
	//	//else {
	//	//	this->fitness[i] += 1000 * (BLOCK_SIZE / 4 - min_area);
	//	//}

	//	/* 領域の重心 */
	//	unordered_map<int, ofPoint> center_points(this->selected_users_num); // 各ユーザの重心
	//	for (const auto& user : this->user_block[i]) {
	//		center_points.emplace(make_pair(user.first, ofPoint(0, 0)));
	//		for (const auto& block : user.second) {
	//			center_points[user.first].x += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().x;
	//			center_points[user.first].y += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().y;
	//		}
	//		center_points[user.first].x /= user.second.size();
	//		center_points[user.first].y /= user.second.size();
	//	}

	//	/* 領域の重心から顔との距離を求める */
	//	for (const auto& user : this->user_block[i]) {
	//		try {
	//			this->fitness[i] -= 1000 * this->euclid_distance(center_points[user.first].x, center_points[user.first].y, this->hc->user_data.at(user.first).transformed_face_point.x(), this->hc->user_data.at(user.first).transformed_face_point.y());
	//		}
	//		catch (std::out_of_range&) {
	//			continue;
	//		}
	//	}

	//	if (this->selected_users_num > 1) {
	//		if (i != 0) {
	//			this->ofs << ",";
	//			//this->ofs2 << ",";
	//		}
	//		this->ofs << this->fitness[i];
	//		//this->ofs2 << min_area;
	//	}

	//	///* 他のユーザのカーソルからの距離を求める */
	//	//for (const auto& main_user : this->user_block) {
	//	//	for (const auto& other_user : this->all_users_id) {
	//	//		if (main_user.first == other_user) {
	//	//			continue;
	//	//		}
	//	//		try {
	//	//			for (const auto& block : main_user.second) {
	//	//				this->fitness[i] += 1000 * this->euclid_distance(center_points[main_user.first].x, center_points[main_user.first].y, CAMERA_W - hc->track_data.at(other_user).current_pointer.x, hc->track_data.at(other_user).current_pointer.y);
	//	//			}
	//	//		}
	//	//		catch (std::out_of_range&) {}
	//	//	}
	//	//}
	//}
	//if (this->selected_users_num > 1) {
	//	this->ofs << endl;
	//	//this->ofs2 << endl;
	//}

	concurrency::parallel_for(0, this->population_size_tmp, [&](int i) {
		/* 各ブロックに割り当てられたユーザidを求める */
		for (int j = 0; j < this->block_size; ++j) {
			this->block_assignments[i][j] = this->NOT_USER;
			for (const auto& user_id : this->selected_users_id) {
				bool flag = true;
				for (int k = 0; k < this->block_bits_size; ++k) {
					if (((this->user_id_index[user_id] >> k) & 1) != this->population[i][j*this->block_bits_size + k]) {
						flag = false;
						break;
					}
				}
				if (flag) {
					this->block_assignments[i][j] = user_id;
					this->user_block[i][user_id].emplace(j); // 各ユーザがもつブロックを求める
					break;
				}
			}
		}

		/*** 面積 ***/

		/* 各ユーザの領域面積で最小なものを最大化する */
		int min_area = INT_MAX;
		for (const auto& user : this->user_block[i]) {
			min_area = min(min_area, static_cast<int>(user.second.size()));
		}

		this->fitness[i] += min_area*this->grid_h*this->grid_w;

		/* 領域の重心 */
		unordered_map<int, ofPoint> center_points(this->selected_users_num); // 各ユーザの重心
		for (const auto& user : this->user_block[i]) {
			center_points.emplace(make_pair(user.first, ofPoint(0, 0)));
			for (const auto& block : user.second) {
				center_points[user.first].x += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().x;
				center_points[user.first].y += this->grid_rects[this->block2grid_table[block].first][this->block2grid_table[block].second].getCenter().y;
			}
			center_points[user.first].x /= user.second.size();
			center_points[user.first].y /= user.second.size();
		}

		/* 領域の重心から顔との距離を求める */
		for (const auto& user : this->user_block[i]) {
			try {
				this->fitness[i] -= 1000 * this->euclid_distance(center_points[user.first].x, center_points[user.first].y, this->hc->user_data.at(user.first).transformed_face_point.x(), this->hc->user_data.at(user.first).transformed_face_point.y());
			}
			catch (std::out_of_range&) {
				continue;
			}
		}
	});
}

void GeneticAlgorithm::selection() {
	//double fitness_sum = accumulate(begin(this->fitness), end(this->fitness), 0.0);
	//ofs << fitness_sum / this->fitness.size() << endl;

	vector<genome_type> new_population(this->population_size);

	/* エリート主義(最適個体を確保) */
	double best_fitness_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	//cout << this->fitness[best_fitness_index] << " " << this->elite_fitness << endl;

	if (this->fitness[best_fitness_index] > this->elite_fitness) {
		this->elite_fitness = this->fitness[best_fitness_index];
		this->elite_individual = this->population[best_fitness_index];
		this->elite_block_assignment = this->block_assignments[best_fitness_index];
	}

	//this->elite_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	//this->elite_individual = new_population[0] = this->population[this->elite_index];
	//this->elite_block_assignment = this->block_assignments[this->elite_index];

	const double fitness_max = *max_element(begin(this->fitness), end(this->fitness)); // 適応度の最大値
	const double fitness_min = *min_element(begin(this->fitness), end(this->fitness)); // 適応度の最小値

	//puts("start");
	//for (int i = 0; i < this->fitness.size(); ++i) {
	//	cout << this->fitness[i] << " ";
	//}
	//cout << endl;

	/** ルーレット方式で選択 **/

	/* 適応度のスケーリング(適応度の数値に負の値が含まれるため) */
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
			for (auto& f : this->fitness) {
				f += -fitness_min + 10;
			}
		}
	}

	double fitness_sum = accumulate(begin(this->fitness), end(this->fitness), 0.0);
	uniform_real_distribution<double> random_fitness(0.0, fitness_sum);
	vector<int> v(this->population_size_tmp);
	std::iota(begin(v), end(v), 0);
	double r, s;
	for (int i = 0; i < this->population_size; ++i) {
		shuffle(begin(v), end(v), this->mt);
		s = 0.0;
		r = random_fitness(this->mt);
		for (int j = 0; j < this->population_size_tmp; ++j) {
			s += this->fitness[v[j]];
			if (s >= r) {
				new_population[i] = this->population[v[j]];
				break;
			}
		}
	}

	if (this->selected_users_num > 1) {
		this->ofs3 << this->fitness[0];
		for (int i = 1; i < this->population_size; ++i) {
			this->ofs3 << "," << this->fitness[i];
		}
		this->ofs3 << endl;
	}

	//for (int i = 0; i < 5; ++i) {
	//	cout << this->fitness[i] << " ";
	//}
	//cout << endl;

	this->population = new_population;
}

void GeneticAlgorithm::draw(const array<int, block_size>& block_assignment) const {
	ofFill();
	for (int i = 0; i < this->block_size; ++i) {
		if (block_assignment[i] != this->NOT_USER) {
			try {
				ofSetColor(this->hc->user_data.at(block_assignment[i]).cursor_color, 100);
			}
			catch (std::out_of_range&) {
				continue;
			}
			ofDrawRectangle(this->grid_rects[this->block2grid_table[i].first][this->block2grid_table[i].second]);
		}
	}
	ofNoFill();
}

void GeneticAlgorithm::draw() const {
	this->draw(this->elite_block_assignment);
}

double GeneticAlgorithm::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}