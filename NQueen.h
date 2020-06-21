#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
#include "operating.h"

/*@@@@@@@@@@
	把八皇后問題套用在GA上
  @@@@@@@@@@*/

#define QUEEN_SIZE 8

class NQueen {
protected:
	friend NQueen;
	std::vector<std::vector<int>> map;
	std::vector<int> X, Y;
	int _size;
	std::default_random_engine generator; //偽亂數產生器
	std::uniform_int_distribution<int> distribution;

public:
	NQueen(int seed) {
		_size = QUEEN_SIZE;
		generator.seed(seed); //設定偽亂數產生器的seed
		rand_set();
	}


	NQueen(NQueen& queen, int seed) : NQueen(queen.X, queen.Y, seed, queen._size) { }


	NQueen(std::vector<int> x, std::vector<int> y, int seed) :NQueen(x, y, seed, QUEEN_SIZE) { }


	NQueen(std::vector<int> x, std::vector<int> y, int seed, int size) {
		_size = QUEEN_SIZE;
		generator.seed(seed); //設定偽亂數產生器的seed
		this->X.clear();
		this->Y.clear();
		this->X.assign(x.begin(), x.end());
		this->Y.assign(y.begin(), y.end());
	}


	double get_survival_rate() {
		//依皇后問題來看，我認為這邊應該要用2次或3次曲線會是比較好描述存活率的方法
		double max_conflict = static_cast<double>(this->_size * (this->_size - 1) * this->_size / 2) / 8;
		double conflict = static_cast<double>(price(this->X, this->Y));
		double score = conflict / max_conflict;
		return 1.0 - score;
	}


	void mutation() {
		rand_set();
	}


	std::tuple<NQueen*, NQueen*> transfer(NQueen* feature) {
		if (this->_size != feature->_size)
			throw std::exception("tranfer function: size not equal.");

		std::vector<int> x1, x2, y1, y2;
		bool change = false;

		for (int i = 0; i < this->_size; i++) {
			if (change) {
				x1.push_back(feature->X[i]);
				x2.push_back(this->X[i]);
				y1.push_back(feature->Y[i]);
				y2.push_back(this->Y[i]);
			}
			else {
				x1.push_back(this->X[i]);
				x2.push_back(feature->X[i]);
				y1.push_back(this->Y[i]);
				y2.push_back(feature->Y[i]);
			}

			if (i == (this->_size / 2) - 1)
				change = true;
		}
		NQueen* child1 = new NQueen(x1, y1, get_rand_number(0, 99999));
		NQueen* child2 = new NQueen(x1, y2, get_rand_number(0, 99999));
		return { child1, child2 };
	}


	bool goal() {
		return price(this->X, this->Y) == 0;
	}


	void rand_set() {
		this->X.clear();
		this->Y.clear();
		for (int i = 0; i < _size; i++) {
			int randnumber = get_rand_number(0, 7);
			X.push_back(i);
			Y.push_back(randnumber);
		}
	}


	void set_map() {
		map.clear();
		//初始化棋盤
		for (int i = 0; i < _size; i++) {
			std::vector<int> tempv;
			for (int j = 0; j < _size; j++) {
				tempv.push_back(0);
			}
			map.push_back(tempv);
		}

		for (int i = 0; i < static_cast<int>(X.size()); i++) {
			map[X.at(i)][Y.at(i)] = 1;
		}
	}


	void show() {
		set_map();

		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				if (map[i][j] == 1)
					std::cout << "╳ ";
				else
					std::cout << "＿";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}


	int price(std::vector<int> x, std::vector<int> y) {
		int count = 0;
		for (int i = 0; i < _size; i++) {
			for (int j = i + 1; j < _size; j++) {
				double _x = x[i] - x[j];
				double _y = y[i] - y[j];
				double m = _y / _x;
				if (_x == 0.0 || _y == 0.0 || m == 1.0 || m == -1.0) {
					count++;
					continue;
				}
			}
		}

		return count;
	}


	int get_rand_number(int low, int high) {
		//設定亂數分布範圍
		std::uniform_int_distribution<int>::param_type param(low, high);
		distribution.param(param);

		return this->distribution(this->generator);
	}
};