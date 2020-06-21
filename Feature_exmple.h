#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
#include "operating.h"


class FEATURE_exmple {
protected:
	friend FEATURE_exmple;
	std::vector<int> feature;
	std::default_random_engine generator; //偽亂數產生器
	std::uniform_int_distribution<int> distribution; //設定範圍與決定要產生什麼類型的亂數 ex:int float..

public:
	FEATURE_exmple(int seed) {
		this->generator.seed(seed); //設定偽亂數產生器的seed
	}


	FEATURE_exmple(FEATURE_exmple& data, int seed) {
		this->generator.seed(seed); //設定偽亂數產生器的seed
	}


	//一定要
	double get_survival_rate() {
		
	}


	//一定要
	void mutation() {
		
	}


	//一定要
	std::tuple<FEATURE_exmple*, FEATURE_exmple*> transfer(FEATURE_exmple* feature) {
		
	}


	//一定要
	bool goal() {
		if (get_survival_rate() == 1.0)
			return true;
		return false;
	}
};