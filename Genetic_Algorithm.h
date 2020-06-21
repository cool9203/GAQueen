#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
#include "operating.h"

template <typename T>
class Genetic_Algorithm {
	/* @@@@@@@@
		typename T應該要是個class or struct
		需要實作：
		1. double get_survival_rate();		//要回傳0~1之間的double型態存活率
		2. void mutation();					//突變方法
		3. std::tuple<T*, T*> transfer(T*)	//交換基因方法
		4. bool goal();						//檢查是否達到全域目標, 如果是unsupervised的話這邊要return false, 反之是supervised的話則寫出你的檢查方法
		5. T(int seed);						//建構函式, 會協助給定一個seed, 來產生亂數
		6. T(T&, int seed);					//建構函式, 會協助給定一個seed, 來產生亂數
	   @@@@@@@@*/

protected:
	std::vector<T*> colony, survivor;
	std::vector<int> transfer_order;
	int colony_max_size;			//colony size, 群體上限
	int mutation_rate;				//突變率 1=1/1000
	int base_survival;				//基礎存活率
	int survival_size;				//限定每次天擇的存活上限(還是有機會會活超過這數字,因為好的基因會高機率留下來) input 0 = no set
	int foreigner;					//在產生子代時，是否固定增加全新個體, 0 -> no set, >0 -> 固定增加N個新個體。   (外國人移民的概念)
	int eugenics;					//優生學:好的會優先跟好的交換基因, 好的換完後會再整個倖存族群全部隨機交換基因.  0 -> no set, >0 -> 存活率>=eugenics的會當作好的基因.
	double power_survival;			//x^n的存活率機制, 可以讓好的留下來, 並快速淘汰中下的
	bool parents_safe;				//是否留存父母在基因庫裡
	int epoch;						//訓練代數上限, 如果是unsupervised的訓練, 那應該要>=1. 反之若是supervised, 則應該=0.

	std::default_random_engine generator; //初始化，並利用rd()的值找出相對應的種子碼??
	std::uniform_int_distribution<int> distribution; //設定範圍與決定要產生什麼類型的亂數 ex:int float..


public:
	Genetic_Algorithm(int colony_max_size, int mutation_rate, int base_survival, int survival_size, int foreigner, int eugenics, bool parents_safe, bool power_survival, int epoch) {
		this->colony_max_size = colony_max_size;
		this->mutation_rate = mutation_rate;
		this->base_survival = base_survival;
		this->survival_size = survival_size;
		this->foreigner = foreigner;
		this->eugenics = eugenics;
		this->parents_safe = parents_safe;
		this->power_survival = power_survival;
		this->epoch = epoch;

		std::random_device rd; //利用硬體系統給的值來取值
		this->generator.seed(rd()); //設定偽亂數產生器的seed
	}


	int get_rand_number(int low, int high) {
		//設定亂數分布範圍
		std::uniform_int_distribution<int>::param_type param(low, high);
		distribution.param(param);

		return this->distribution(this->generator);
	}


	//天擇
	void selection() {
		clear(this->survivor);

		int max_survial = 0;

		//遍歷整個族群
		for (int i = 0; i < this->colony.size(); i++) {
			int survial = this->base_survival + static_cast<int>(this->colony[i]->get_survival_rate() * 100);	//取得存活率

			if (survial <= 0)	//如果存活率<=0 那就下一位
				continue;

			if (max_survial < survial)
				max_survial = survial;

			if (this->power_survival && this->power_survival != 0);
				survial = std::pow((static_cast<double>(survial) / 100.0), this->power_survival) * 100;

			//看有沒有存活下來，有存活的話就存到survivor
			int temp_number = get_rand_number(0, 100);
			if (temp_number <= survial) {
				if (eugenics != 0 && eugenics >= (survial - this->base_survival))
					this->transfer_order.push_back(this->survivor.size());
				T* temp = new T(*(this->colony[i]), get_rand_number(0, 99999));
				this->survivor.push_back(temp);
			}
		}
		std::cout << max_survial << std::endl;
	}


	//交換基因(產生下一代)
	void reproduction() {
		clear(this->colony);	//先清空族群

		if (this->survivor.size() <= 2) {
			create_colony();
			return;
		}

		//因為想追求公平隨機，所以這邊用洗牌演算法來取得交換順序，但這樣會導致可能要跑很多次才能把族群填滿，所這邊多加一個while，來保證他會滿
		bool full_flag = false;
		while (!full_flag)
		{
			std::vector<int> order = operating::shuffle<int>(get_rand_number(0, 99999), 0, this->survivor.size() - 1);	//取得洗牌後的序列
			operating::shuffle<int>(get_rand_number(0, 99999), this->transfer_order);	//取得洗牌後的序列

			//看父母代是否要保留在族群內
			int max_size;
			if (this->parents_safe)
				max_size = this->colony_max_size - this->foreigner - this->survivor.size();
			else
				max_size = this->colony_max_size - this->foreigner;

			//是否有設置eugenics, 有的話要先做
			if (this->eugenics > 0 && this->transfer_order.size() >= 2) {
				for (int i = 0; i < this->transfer_order.size() - this->transfer_order.size() % 2; i += 2) {
					if (this->colony.size() < max_size) {
						product(this->transfer_order[i], this->transfer_order[i + 1]);
					}
					else {
						full_flag = true;
						break;
					}
				}
			}

			//因為上面有先交換過一輪，所以這邊要先確認族群有沒有滿
			if (full_flag)
				break;

			//挑選兩個來基因交換
			for (int i = 0; i < order.size() - order.size() % 2; i += 2) {
				if (this->colony.size() < max_size) {
					product(order[i], order[i + 1]);
				}
				else {
					full_flag = true;
					break;
				}
			}
		}	//while end

		//到這邊代表，族群暫時滿了，所以要返回來看parents_safe是不是true，是的話就要加進族群裡
		if (this->parents_safe) {
			for (int j = 0; j < this->survivor.size(); j++) {
				T* temp = new T(*(this->survivor[j]), get_rand_number(0, 99999));
				this->colony.push_back(temp);
			}
		}

		//接著新增全新個體到族群內
		for (int i = 0; i < this->foreigner; i++) {
			T* individual = new T(get_rand_number(0, 99999));
			add_init_colony(individual);
		}

		this->transfer_order.clear();
	}


	//突變
	void mutation() {
		if (this->mutation_rate == 0)
			return;

		std::vector<int> order = operating::shuffle<int>(get_rand_number(0, 99999), 0, this->colony.size() - 1);

		for (int i = 0; i < order.size(); i++) {
			int temp_number = get_rand_number(1, 1000);
			if (temp_number <= this->mutation_rate) {
				int index = order[i];
				this->colony[index]->mutation();
			}
		}
		
	}


	//給定2個index, 產生後代並加到colony裡
	void product(int index1, int index2) {
		std::tuple<T*, T*> child = this->survivor[index1]->transfer(this->survivor[index2]);
		T* child1 = std::get<0>(child);
		T* child2 = std::get<1>(child);
		this->colony.push_back(child1);
		this->colony.push_back(child2);
	}


	//取得族群
	std::vector<T*> get_colony() {
		return this->colony;
	}


	//設定初始的族群
	void set_init_colony(std::vector<T*> init_data) {
		this->colony.assign(init_data.begin(), init_data.end());
	}


	void add_init_colony(T *data) {
		this->colony.push_back(data);
	}

	
	//檢查colony裡是否有達到設定的目標了
	bool goal() {
		for (int i = 0; i < this->colony.size(); i++) {
			if (colony[i]->goal())
				return true;
		}
		return false;
	}


	void create_colony() {
		for (int i = colony.size(); i < colony_max_size; i++) {
			T* individual = new T(get_rand_number(0, 99999));
			add_init_colony(individual);
		}
	}


	void clear(std::vector<T*> &data) {
		for (int i = 0; i < data.size(); i++) {
			delete data[i];
		}
		data.clear();
	}


	//利用public function實作出可以執行的演算法
	std::vector<T*> start() {
		//先創造一開始的族群
		create_colony();

		int epoch_count = 1;
		while (true) {
			std::cout << "第" << epoch_count << "代" << std::endl;

			if (epoch_count == this->epoch || goal())	//如果是unsupervised, 那goal的實作則永遠返回flase, 且同時設定代數上限。 反之如果是supervised, 那goal則要設定條件, 此時epoch則應該要設為0。
				break;
			
			epoch_count++;

			//天擇(適配函數)過程
			selection();

			//產生子代
			reproduction();

			//突變過程
			mutation();
		}

		return get_colony();
	}
};