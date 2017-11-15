#include <unordered_set>
#include "GeneticAlgorithm.h"

using namespace std;
using namespace param;

random_device GeneticAlgorithm::rd;
mt19937 GeneticAlgorithm::mt(GeneticAlgorithm::rd());
uniform_int_distribution<int> GeneticAlgorithm::random_0or1(0, 1);
uniform_int_distribution<int> GeneticAlgorithm::random_indivisual(0, GeneticAlgorithm::population_size-1);
//uniform_int_distribution<int> GeneticAlgorithm::random_crossover_point(1, BLOCK_SIZE - 1);
uniform_int_distribution<int> GeneticAlgorithm::random_bit(0, BLOCK_SIZE);
uniform_real_distribution<double> GeneticAlgorithm::random_0to1(0.0, 1.0);

void GeneticAlgorithm::setup(HandCursor* hc) {
	this->hc = hc; // 手カーソル情報

	/* ブロックと座標の変換表を作成 */
	for (int y = 0, y_pos = 0, i = 0; y < FORM_H; ++y, y_pos += GRID_H) {
		for (int x = 0, x_pos = 0; x < FORM_W; ++x, x_pos += GRID_W) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, GRID_W, GRID_H);
			this->block2grid_table[i].first = x;
			this->block2grid_table[i].second = y;
			this->grid2block_table[x][y] = i++;
		}
	}

	/* 集団を管理する変数のサイズとメモリを調整 */
	this->population.resize(this->population_size);
	this->population.reserve(this->population_size + 2 * this->crossover_pair_number); // 交叉によって増加しうる分だけ集団サイズのメモリを確保
}

void GeneticAlgorithm::operator()(genome_type& best_individual, const set<int>& users_id) {
	this->initialize(users_id);
	for (int i = 0; i < 10000; ++i) {
		this->crossover();
		this->mutation();
		this->calculate_fitness();
		this->selection();
	}
	best_individual = this->best_individual;
}

void GeneticAlgorithm::operator()(const genome_type& initial_individual, genome_type& best_individual, const set<int>& users_id) {
	//this->initialize(initial_individual);
	//for (int i = 0; i < 100; ++i) {
	//	this->crossover();
	//	this->mutation();
	//	this->calculate_fitness();
	//	this->selection();
	//}
	//best_individual = this->best_individual;
}

void GeneticAlgorithm::initialize(const set<int>& users_id) {
	this->users_id = users_id;
	this->users_num = this->users_id.size();

	/* ユーザIDに対してインデックスを割り当てる(0から順に) */
	int i = 0;
	this->user_id_index.clear();
	for (const auto& user_id : this->users_id) {
		this->user_id_index.emplace(make_pair(user_id, i++));
	}
	
	/* 各ブロックのビット数を決める */
	this->block_bits_size = 1;
	while (pow(2, this->block_bits_size++) > 2 * this->users_num) {} // ユーザ数と同じ分だけユーザでない状態を表せるようなビット数にする

	this->genetic_length = BLOCK_SIZE*this->block_bits_size; // 遺伝子長を決める

	/* 全個体のビットをランダムに初期化する */
	for (i = 0; i < this->population_size; ++i) {
		this->population[i].resize(this->genetic_length);
		for (int j = 0; j < this->genetic_length; ++j) {
			this->population[i][j] = this->random_0or1(this->mt);
		}
	}
}

void GeneticAlgorithm::initialize(const param::genome_type& initial_individual) {
	//this->initialize();
	//swap(this->population[0], this->population[this->population_size - 1]);
	//this->population[0] = initial_individual; // 先頭に引数の個体を入れる
}

void GeneticAlgorithm::crossover() {
	for (int i = 0; i < this->crossover_pair_number; ++i) {
		if (this->random_0to1(this->mt) < this->crossover_probability) { // 交叉確率に基づいて交叉するかどうかを決める
			/* 異なる二つの個体(親)を選ぶ */
			pair<int, int> parents_index(this->random_indivisual(this->mt), this->random_indivisual(this->mt));
			while (parents_index.first == parents_index.second) {
				parents_index.second = this->random_indivisual(this->mt);
			}

			/* 一点交叉 */
			uniform_int_distribution<int> random_crossover_point(1, this->genetic_length - 1);
			int crossover_point = random_crossover_point(this->mt);
			
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
	}
	

	//for (int i = 0; i < this->crossover_pair_number; ++i) {
	//	/* 交叉確率に基づいて交叉するかどうかを決める */
	//	if (this->random_0to1(this->mt) < this->crossover_probability) {
	//		/* 異なる二つの個体(親)を選ぶ */
	//		pair<int, int> parents_index(this->random_indivisual(this->mt), this->random_indivisual(this->mt));
	//		while (parents_index.first == parents_index.second) {
	//			parents_index.second = this->random_indivisual(this->mt);
	//		}

	//		uniform_int_distribution<int> random_crossover_method(1, 6);

	//		switch (random_crossover_method(this->mt)) {
	//		case 1:
	//		{
	//			/* 一点交叉 */
	//			int crossover_point = this->random_crossover_point(this->mt);
	//			pair<genome_type, genome_type> children;
	//			children.first = this->population[parents_index.first];
	//			children.second = this->population[parents_index.second];
	//			for (int j = 0; j < crossover_point; ++j) {
	//				children.first[j] = this->population[parents_index.second][j];
	//				children.second[j] = this->population[parents_index.first][j];
	//			}
	//			this->population.emplace_back(children.first);
	//			this->population.emplace_back(children.second);
	//		}
	//		break;
	//		case 2:
	//		{
	//			/* 二点交叉 */
	//			pair<int, int> crossover_points(this->random_crossover_point(this->mt), this->random_crossover_point(this->mt));
	//			while (crossover_points.first == crossover_points.second) {
	//				crossover_points.second = this->random_crossover_point(this->mt);
	//			}
	//			pair<genome_type, genome_type> children;
	//			children.first = this->population[parents_index.first];
	//			children.second = this->population[parents_index.second];
	//			for (int j = crossover_points.first; j < crossover_points.second; ++j) {
	//				children.first[j] = this->population[parents_index.second][j];
	//				children.second[j] = this->population[parents_index.first][j];
	//			}
	//			this->population.emplace_back(children.first);
	//			this->population.emplace_back(children.second);
	//		}
	//		break;
	//		case 3:
	//		{
	//			/* 一様交叉 */
	//			genome_type mask;
	//			for (int j = 0; j < BITS_SIZE; ++j) {
	//				mask[j] = this->random_0or1(this->mt);
	//			}
	//			this->population.emplace_back((mask & this->population[parents_index.first]) | (~mask & this->population[parents_index.second]));
	//			this->population.emplace_back((mask & this->population[parents_index.second]) | (~mask & this->population[parents_index.first]));
	//		}
	//		break;
	//		case 4:
	//			/* 算術交叉(AND) */
	//			this->population.emplace_back(this->population[parents_index.first] & this->population[parents_index.second]);
	//			break;
	//		case 5:
	//			/* 算術交叉(OR) */
	//			this->population.emplace_back(this->population[parents_index.first] | this->population[parents_index.second]);
	//			break;
	//		case 6:
	//			/* 算術交叉(XOR) */
	//			this->population.emplace_back(this->population[parents_index.first] ^ this->population[parents_index.second]);
	//			break;
	//		}
	//	}
	//}
}

void GeneticAlgorithm::mutation() {
	uniform_int_distribution<int> random_mutation_method(2, 3);
	for (int i = 0; i < this->population_size; ++i) {
		/* 突然変異率に基づいて突然変異するかどうかを決める */
		if (this->random_0to1(this->mt) < this->mutation_probability) {
			switch (random_mutation_method(this->mt)) {
			case 1:
				/* すべてのビットを反転 */
				this->population[i].flip();
				break;
			case 2:
			{
				/* 逆位(特定の範囲のビットの順序を逆にする) */
				int s = this->random_bit(this->mt), t = this->random_bit(this->mt);
				if (s > t) {
					swap(s, t);
				}
				reverse(&this->population[s], &this->population[t]);
			}
			break;
			case 3:
			{
				/* スクランブル(特定の範囲のビットの順序をランダムに並べ替える) */
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
	const int population_size_now = this->population.size(); // 交叉や突然変異によって増加した一時的な集団サイズ

	this->fitness.resize(population_size_now);
	fill(begin(this->fitness), end(this->fitness), 0.0);
	
	this->block_assignment.resize(population_size_now);

	for (int i = 0; i < population_size_now; ++i) {
		/* 各ブロックに割り当てられたユーザidを求める */
		for (int j = 0; j < BLOCK_SIZE; ++j) {
			this->block_assignment[i][j] = -1;
			for (const auto& user_id : this->users_id) {
				bool flag = true;
				for (int k = 0; k < this->block_bits_size; ++k) {
					if (((this->user_id_index[user_id] >> k) & 1) != this->population[i][j*this->block_bits_size + k]) {
						flag = false;
						break;
					}
				}
				if (flag) {
					this->block_assignment[i][j] = user_id;
					break;
				}
			}
		}

		/* 面積 */
		double area_sum = 0;
		for (int j = 0; j < BLOCK_SIZE; ++j) {
			if (this->block_assignment[i][j] != -1) {
				++area_sum;
			}
		}

		this->fitness[i] += 1000*area_sum;
	}

	//for (int i = 0; i < population_size_now; ++i) {
	//	int users_num = this->user_bit_assignments[i].size(); // ユーザ数

	//	/* 面積 */
	//	unordered_map<int, double> area(users_num); // 各ユーザの領域面積
	//	double area_sum = 0; // 全ユーザの領域面積の合計
	//	for (const auto& user : this->user_bit_assignments[i]) {
	//		area[user.first] = user.second.size();
	//		area_sum += area[user.first];
	//	}

	//	double area_mean = area_sum / users_num;
	//	double area_variance = 0.0; // ユーザ同士の領域面積の分散
	//	for (const auto& a : area) {
	//		area_variance += a.second*a.second;
	//	}
	//	area_variance /= users_num;
	//	area_variance -= area_mean*area_mean;

	//	if (area_mean <= FORM_W*FORM_H / 4) {
	//		this->fitness[i] += 10000*exp(-pow(area_mean - FORM_W*FORM_H / 4, 2)/100);
	//	}
	//	else {
	//		this->fitness[i] -= 1000*exp(FORM_W*FORM_H / 4 - area_mean);
	//	}
	//	this->fitness[i] -= exp(area_variance)/100;

	//	/* 重心 */
	//	unordered_map<int, ofPoint> center_points(users_num); // 各ユーザの重心
	//	for (const auto& user : this->user_bit_assignments[i]) {
	//		center_points.emplace(make_pair(user.first, ofPoint(0, 0)));
	//		for (int j = 0; j < user.second.size(); ++j) {
	//			center_points[user.first].x += this->grid_rects[this->bit2grid_table[user.second[j]].first][this->bit2grid_table[user.second[j]].second].getCenter().x;
	//			center_points[user.first].y += this->grid_rects[this->bit2grid_table[user.second[j]].first][this->bit2grid_table[user.second[j]].second].getCenter().y;
	//		}
	//		center_points[user.first].x /= area[user.first];
	//		center_points[user.first].y /= area[user.first];
	//	}

	//	/* 重心からの距離の合計を求める */
	//	unordered_map<int, double> center_points_distance(users_num);
	//	for (const auto& user : this->user_bit_assignments[i]) {
	//		center_points_distance.emplace(make_pair(user.first, 0.0));
	//		for (int j = 0; j < user.second.size(); ++j) {
	//			center_points_distance[user.first] += this->euclid_distance(center_points[user.first].x, center_points[user.first].y, this->grid_rects[this->bit2grid_table[user.second[j]].first][this->bit2grid_table[user.second[j]].second].getCenter().x, this->grid_rects[this->bit2grid_table[user.second[j]].first][this->bit2grid_table[user.second[j]].second].getCenter().y);
	//		}
	//	}

	//	/* 他のユーザのカーソルからの距離を求める */
	//	unordered_map<int, double> cursor_distance(users_num);
	//	for (const auto& main_user : this->user_bit_assignments[i]) {
	//		cursor_distance.emplace(make_pair(main_user.first, 0.0));
	//		for (const auto& other_user : this->hc->track_data) {
	//			if (main_user.first == other_user.first) {
	//				continue;
	//			}
	//			for (int j = 0; j < main_user.second.size(); ++j) {
	//				cursor_distance[main_user.first] += this->euclid_distance(this->grid_rects[this->bit2grid_table[main_user.second[j]].first][this->bit2grid_table[main_user.second[j]].second].getCenter().x, this->grid_rects[this->bit2grid_table[main_user.second[j]].first][this->bit2grid_table[main_user.second[j]].second].getCenter().y, W-other_user.second.current_pointer.x, other_user.second.current_pointer.y);
	//			}
	//		}
	//	}

	//	for (const auto& user : this->user_bit_assignments[i]) {
	//		//this->fitness[i] -= center_points_distance[user.first]/100;
	//		this->fitness[i] -= 1000/cursor_distance[user.first];
	//	}

	//	/* 重複 */
	//	int intersects = 0;
	//	set<int> s;
	//	for (const auto& user : this->user_bit_assignments[i]) {
	//		for (int j = 0; j < user.second.size(); ++j) {
	//			int n = s.size();
	//			s.emplace(this->user_bit_assignments[i][user.first][j]);
	//			if (n != s.size()) {
	//				++intersects;
	//			}
	//		}
	//	}

	//	this->fitness[i] += 100000 * intersects;

	//}

	//for (int i = 0; i < population_size_now; ++i) {
	//	double area = this->population[i].count(); // 面積
	//	if (area >= BITS_SIZE / 4) {
	//		this->fitness[i] -= 100000 * (area - BITS_SIZE / 4);
	//	}
	//	else {
	//		this->fitness[i] += 2000 * area;
	//	}


	//	ofPoint center_point; // 重心
	//	for (int j = 0; j < BITS_SIZE; ++j) {
	//		if (this->population[i].test(j)) {
	//			center_point += this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter();
	//		}
	//	}
	//	center_point /= area;

	//	for (int j = 0; j < BITS_SIZE; ++j) {
	//		if (this->population[i].test(j)) {
	//			this->fitness[i] -= 5 * euclid_distance(this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().x, this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().y, center_point.x, center_point.y);
	//		}
	//	}

	//	/* 連結数 */
	//	int connectivity_number = 0;
	//	/*int dx[] = { 1,1,0,-1,-1,-1,0,1 };
	//	int dy[] = { 0,-1,-1,-1,0,1,1,1 };*/
	//	int dx[] = { 1, 0, -1, 0 };
	//	int dy[] = { 0, -1, 0, 1 };
	//	int x, y, nx, ny;
	//	bool flag;
	//	stack<int> s;
	//	genome_type genome_tmp = this->population[i];
	//	unordered_set<int> us;
	//	for (int j = 0; j < BITS_SIZE; ++j) {
	//		if (genome_tmp.test(j)) {
	//			us.emplace(j);
	//		}
	//	}
	//	if (!us.empty()) {
	//		++connectivity_number;
	//		s.push(*begin(us));
	//		while (!s.empty()) {
	//			x = this->bit2grid_table[s.top()].first;
	//			y = this->bit2grid_table[s.top()].second;
	//			genome_tmp.reset(s.top());
	//			us.erase(s.top());
	//			s.pop();
	//			flag = false;
	//			for (int k = 0; k < 4; ++k) {
	//				nx = x + dx[k];
	//				ny = y + dy[k];
	//				if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
	//					continue;
	//				}
	//				if (genome_tmp.test(this->grid2bit_table[nx][ny])) {
	//					s.push(this->grid2bit_table[nx][ny]);
	//					flag = true;
	//				}
	//			}
	//			if (!flag && s.empty()) {
	//				if (us.empty()) {
	//					break;
	//				}
	//				else {
	//					s.push(*begin(us));
	//					++connectivity_number;
	//				}
	//			}
	//		}
	//	}

	//	if (connectivity_number == *this->selected_user_num) {
	//		this->fitness[i] += 10000;
	//	}

	//	/* 周囲長 */
	//	int arc_length = 0;
	//	for (int j = 0; j < BITS_SIZE; ++j) {
	//		if (population[i].test(j)) {
	//			for (int k = 0; k < 4; ++k) {
	//				nx = this->bit2grid_table[j].first + dx[k];
	//				ny = this->bit2grid_table[j].second + dy[k];
	//				if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
	//					continue;
	//				}
	//				if (!population[i].test(this->grid2bit_table[nx][ny])) {
	//					++arc_length;
	//				}
	//			}
	//		}
	//	}
	//	this->fitness[i] -= 100 * arc_length;

	//	/* オイラー数 */
	//	int e = 0, f = 0;
	//	for (int j = 0; j < BITS_SIZE; ++j) {
	//		if (population[i].test(j)) {
	//			for (int k = 0; k < 2; ++k) {
	//				nx = this->bit2grid_table[j].first + dx[k];
	//				ny = this->bit2grid_table[j].second + dy[k];
	//				if (nx < 0 || nx >= FORM_W || ny < 0 || ny >= FORM_H) {
	//					continue;
	//				}
	//				if (population[i].test(this->grid2bit_table[nx][ny])) {
	//					++e;
	//				}
	//			}
	//			if (this->bit2grid_table[j].first == FORM_W - 1 || this->bit2grid_table[j].second == FORM_H - 1) {
	//				continue;
	//			}
	//			if (population[i].test(this->grid2bit_table[this->bit2grid_table[j].first + 1][this->bit2grid_table[j].second]) && population[i].test(this->grid2bit_table[this->bit2grid_table[j].first][this->bit2grid_table[j].second + 1]) && population[i].test(this->grid2bit_table[this->bit2grid_table[j].first + 1][this->bit2grid_table[j].second + 1])) {
	//				++f;
	//			}
	//		}
	//	}
	//	int euler_number = area - e + f;

	//	this->fitness[i] -= 1000 * (connectivity_number - euler_number);

	//	/* カーソルからの距離 */
	//	for (const auto& td : this->hc->track_data) {
	//		for (int j = 0; j < BITS_SIZE; ++j) {
	//			if (population[i].test(j)) {
	//				this->fitness[i] += 100 * this->euclid_distance(this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().x, this->grid_rects[this->bit2grid_table[j].first][this->bit2grid_table[j].second].getCenter().y, W - td.second.current_pointer.x, td.second.current_pointer.y);
	//			}
	//		}
	//	}

	//}
}

void GeneticAlgorithm::selection() {

	vector<genome_type> new_population(this->population_size);

	/* エリート主義(最適個体を確保) */
	this->elite_index = max_element(begin(this->fitness), end(this->fitness)) - begin(this->fitness);
	this->best_individual = new_population[0] = this->population[this->elite_index];
	this->elite_block_assignment = this->block_assignment[this->elite_index];

	const double fitness_max = this->fitness[this->elite_index]; // 適応度の最大値
	const double fitness_min = *min_element(begin(this->fitness), end(this->fitness)); // 適応度の最小値


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
				f += fitness_min + 1;
			}
		}
	}

	double fitness_sum = accumulate(begin(this->fitness), end(this->fitness), 0.0);
	uniform_real_distribution<double> random_fitness(0, fitness_sum);
	vector<int> v(this->population_size);
	iota(begin(v), end(v), 0);
	double r, s;
	for (int i = 1; i < this->population_size; ++i) {
		shuffle(begin(v), end(v), this->mt);
		s = 0.0;
		r = random_fitness(this->mt);
		for (int j = 0; j < this->population_size; ++j) {
			s += this->fitness[j];
			if (s > r) {
				new_population[i] = this->population[j];
				break;
			}
		}
	}

	this->population = new_population;

}

void GeneticAlgorithm::draw_rectangles() const {
	ofFill();
	for (int i = 0; i < BLOCK_SIZE; ++i) {
		if (this->elite_block_assignment[i] != -1) {
			try {
				ofSetColor(this->hc->track_data.at(this->elite_block_assignment[i]).cursor_color);
			}
			catch (std::out_of_range&) {
				continue;
			}
			ofDrawRectangle(this->grid_rects[this->block2grid_table[i].first][this->block2grid_table[i].second]);
		}
	}
	ofNoFill();
}

double GeneticAlgorithm::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}