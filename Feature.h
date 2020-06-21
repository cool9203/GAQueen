#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
#include <tuple>
#include "operating.h"

/*@@@@@@@@@@
	�H�K�g���@���S�x
  @@@@@@@@@@*/

#define MAX_SIZE 12

class FEATURE {
protected:
	friend FEATURE;
	std::vector<int> feature;
	std::default_random_engine generator; //��l�ơA�çQ��rd()���ȧ�X�۹������ؤl�X??
	std::uniform_int_distribution<int> distribution; //�]�w�d��P�M�w�n���ͤ����������ü� ex:int float..
	int size;

public:
	//�@�w�n
	FEATURE(int seed) {
		this->generator.seed(seed); //�]�w���üƲ��;���seed
		this->size = MAX_SIZE;
		random_set();
	}


	//�D���n
	FEATURE(std::vector<int> data, int seed) {
		this->generator.seed(seed); //�]�w���üƲ��;���seed
		this->size = data.size();
		set(data);
	}


	//�@�w�n
	FEATURE(FEATURE& data, int seed) {
		this->generator.seed(seed); //�]�w���üƲ��;���seed
		this->size = data.size;
		set(data.feature);
	}


	//�@�w�n
	double get_survival_rate() {
		double score = 0.0;
		for (int i = 0; i < this->feature.size(); i++) {
			if (this->feature[i] == 2)
				score++;
		}
		return score / this->feature.size();
	}


	//�@�w�n
	void mutation() {
		random_set();
	}


	//�@�w�n
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


	//�@�w�n
	bool goal() {
		if (get_survival_rate() == 1.0)
			return true;
		return false;
	}


	//�D���n
	//feature�H���]�m
	void random_set() {
		this->feature.clear();
		for (int i = 0; i < this->size; i++)
			this->feature.push_back(get_rand_number(0, 5));
	}


	//�D���n
	int get_rand_number(int low, int high) {
		//�]�w�üƤ����d��
		std::uniform_int_distribution<int>::param_type param(low, high);
		distribution.param(param);

		return this->distribution(this->generator);
	}


	//�D���n
	//feature���w�]�m
	void set(std::vector<int> data) {
		this->feature.clear();
		this->feature.assign(data.begin(), data.end());
	}


	//�D���n
	void show() {
		for (int i = 0; i < this->feature.size(); i++)
			std::cout << this->feature[i] << ", ";
	}
};