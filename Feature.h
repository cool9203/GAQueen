#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
#include "operating.h"

/*@@@@@@@@@@
	隨便寫的一維特徵
  @@@@@@@@@@*/

#define MAX_SIZE 12

class FEATURE {
protected:
	friend FEATURE;
	std::vector<int> feature;
	std::default_random_engine generator; //初始化，並利用rd()的值找出相對應的種子碼??
	std::uniform_int_distribution<int> distribution; //設定範圍與決定要產生什麼類型的亂數 ex:int float..
	int size;

public:
	//一定要
	FEATURE(int seed) {
		this->generator.seed(seed); //設定偽亂數產生器的seed
		this->size = MAX_SIZE;
		random_set();
	}


	//非必要
	FEATURE(std::vector<int> data, int seed) {
		this->generator.seed(seed); //設定偽亂數產生器的seed
		this->size = data.size();
		set(data);
	}


	//一定要
	FEATURE(FEATURE& data, int seed) {
		this->generator.seed(seed); //設定偽亂數產生器的seed
		this->size = data.size;
		set(data.feature);
	}


	//一定要
	double get_survival_rate() {
		double score = 0.0;
		for (int i = 0; i < this->feature.size(); i++) {
			if (this->feature[i] == 2)
				score++;
		}
		return score / this->feature.size();
	}


	//一定要
	void mutation() {
		random_set();
	}


	//一定要
	std::tuple<FEATURE*, FEATURE*> transfer(FEATURE* feature) {
		if (this->size != feature->size)
			throw std::exception("tranfer function: size not equal.");

		std::vector<int> f1, f2;
		bool change = false;

		for (int i = 0; i < this->size; i++) {
			if (change) {
				f1.push_back(feature->feature[i]);
				f2.push_back(this->feature[i]);
			}
			else {
				f1.push_back(this->feature[i]);
				f2.push_back(feature->feature[i]);
			}

			if (i == (this->size / 2) - 1)
				change = true;
		}
		FEATURE* child1 = new FEATURE(f1, get_rand_number(0, 99999));
		FEATURE* child2 = new FEATURE(f2, get_rand_number(0, 99999));
		return { child1, child2 };
	}


	//一定要
	bool goal() {
		if (get_survival_rate() == 1.0)
			return true;
		return false;
	}


	//非必要
	//feature隨機設置
	void random_set() {
		this->feature.clear();
		for (int i = 0; i < this->size; i++)
			this->feature.push_back(get_rand_number(0, 5));
	}


	//非必要
	int get_rand_number(int low, int high) {
		//設定亂數分布範圍
		std::uniform_int_distribution<int>::param_type param(low, high);
		distribution.param(param);

		return this->distribution(this->generator);
	}


	//非必要
	//feature指定設置
	void set(std::vector<int> data) {
		this->feature.clear();
		this->feature.assign(data.begin(), data.end());
	}


	//非必要
	void show() {
		for (int i = 0; i < this->feature.size(); i++)
			std::cout << this->feature[i] << ", ";
	}
};