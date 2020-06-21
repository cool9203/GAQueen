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
	std::default_random_engine generator; //���üƲ��;�
	std::uniform_int_distribution<int> distribution; //�]�w�d��P�M�w�n���ͤ����������ü� ex:int float..

public:
	FEATURE_exmple(int seed) {
		this->generator.seed(seed); //�]�w���üƲ��;���seed
	}


	FEATURE_exmple(FEATURE_exmple& data, int seed) {
		this->generator.seed(seed); //�]�w���üƲ��;���seed
	}


	//�@�w�n
	double get_survival_rate() {
		
	}


	//�@�w�n
	void mutation() {
		
	}


	//�@�w�n
	std::tuple<FEATURE_exmple*, FEATURE_exmple*> transfer(FEATURE_exmple* feature) {
		
	}


	//�@�w�n
	bool goal() {
		if (get_survival_rate() == 1.0)
			return true;
		return false;
	}
};