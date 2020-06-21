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
		typename T���ӭn�O��class or struct
		�ݭn��@�G
		1. double get_survival_rate();		//�n�^��0~1������double���A�s���v
		2. void mutation();					//���ܤ�k
		3. std::tuple<T*, T*> transfer(T*)	//�洫��]��k
		4. bool goal();						//�ˬd�O�_�F�����ؼ�, �p�G�Ounsupervised���ܳo��nreturn false, �Ϥ��Osupervised���ܫh�g�X�A���ˬd��k
		5. T(int seed);						//�غc�禡, �|��U���w�@��seed, �Ӳ��Ͷü�
		6. T(T&, int seed);					//�غc�禡, �|��U���w�@��seed, �Ӳ��Ͷü�
	   @@@@@@@@*/

protected:
	std::vector<T*> colony, survivor;
	std::vector<int> transfer_order;
	int colony_max_size;			//colony size, �s��W��
	int mutation_rate;				//���ܲv 1=1/1000
	int base_survival;				//��¦�s���v
	int survival_size;				//���w�C���Ѿܪ��s���W��(�٬O�����|�|���W�L�o�Ʀr,�]���n����]�|�����v�d�U��) input 0 = no set
	int foreigner;					//�b���ͤl�N�ɡA�O�_�T�w�W�[���s����, 0 -> no set, >0 -> �T�w�W�[N�ӷs����C   (�~��H����������)
	int eugenics;					//�u�;�:�n���|�u����n���洫��], �n��������|�A��ӭƦs�ڸs�����H���洫��].  0 -> no set, >0 -> �s���v>=eugenics���|��@�n����].
	double power_survival;			//x^n���s���v����, �i�H���n���d�U��, �çֳt�^�O���U��
	bool parents_safe;				//�O�_�d�s�����b��]�w��
	int epoch;						//�V�m�N�ƤW��, �p�G�Ounsupervised���V�m, �����ӭn>=1. �Ϥ��Y�Osupervised, �h����=0.

	std::default_random_engine generator; //��l�ơA�çQ��rd()���ȧ�X�۹������ؤl�X??
	std::uniform_int_distribution<int> distribution; //�]�w�d��P�M�w�n���ͤ����������ü� ex:int float..


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

		std::random_device rd; //�Q�εw��t�ε����ȨӨ���
		this->generator.seed(rd()); //�]�w���üƲ��;���seed
	}


	int get_rand_number(int low, int high) {
		//�]�w�üƤ����d��
		std::uniform_int_distribution<int>::param_type param(low, high);
		distribution.param(param);

		return this->distribution(this->generator);
	}


	//�Ѿ�
	void selection() {
		clear(this->survivor);

		int max_survial = 0;

		//�M����ӱڸs
		for (int i = 0; i < this->colony.size(); i++) {
			int survial = this->base_survival + static_cast<int>(this->colony[i]->get_survival_rate() * 100);	//���o�s���v

			if (survial <= 0)	//�p�G�s���v<=0 ���N�U�@��
				continue;

			if (max_survial < survial)
				max_survial = survial;

			if (this->power_survival && this->power_survival != 0);
				survial = std::pow((static_cast<double>(survial) / 100.0), this->power_survival) * 100;

			//�ݦ��S���s���U�ӡA���s�����ܴN�s��survivor
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


	//�洫��](���ͤU�@�N)
	void reproduction() {
		clear(this->colony);	//���M�űڸs

		if (this->survivor.size() <= 2) {
			create_colony();
			return;
		}

		//�]���Q�l�D�����H���A�ҥH�o��ά~�P�t��k�Ө��o�洫���ǡA���o�˷|�ɭP�i��n�]�ܦh���~���ڸs�񺡡A�ҳo��h�[�@��while�A�ӫO�ҥL�|��
		bool full_flag = false;
		while (!full_flag)
		{
			std::vector<int> order = operating::shuffle<int>(get_rand_number(0, 99999), 0, this->survivor.size() - 1);	//���o�~�P�᪺�ǦC
			operating::shuffle<int>(get_rand_number(0, 99999), this->transfer_order);	//���o�~�P�᪺�ǦC

			//�ݤ����N�O�_�n�O�d�b�ڸs��
			int max_size;
			if (this->parents_safe)
				max_size = this->colony_max_size - this->foreigner - this->survivor.size();
			else
				max_size = this->colony_max_size - this->foreigner;

			//�O�_���]�meugenics, �����ܭn����
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

			//�]���W�������洫�L�@���A�ҥH�o��n���T�{�ڸs���S����
			if (full_flag)
				break;

			//�D���ӨӰ�]�洫
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

		//��o��N��A�ڸs�Ȯɺ��F�A�ҥH�n��^�Ӭ�parents_safe�O���Otrue�A�O���ܴN�n�[�i�ڸs��
		if (this->parents_safe) {
			for (int j = 0; j < this->survivor.size(); j++) {
				T* temp = new T(*(this->survivor[j]), get_rand_number(0, 99999));
				this->colony.push_back(temp);
			}
		}

		//���۷s�W���s�����ڸs��
		for (int i = 0; i < this->foreigner; i++) {
			T* individual = new T(get_rand_number(0, 99999));
			add_init_colony(individual);
		}

		this->transfer_order.clear();
	}


	//����
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


	//���w2��index, ���ͫ�N�å[��colony��
	void product(int index1, int index2) {
		std::tuple<T*, T*> child = this->survivor[index1]->transfer(this->survivor[index2]);
		T* child1 = std::get<0>(child);
		T* child2 = std::get<1>(child);
		this->colony.push_back(child1);
		this->colony.push_back(child2);
	}


	//���o�ڸs
	std::vector<T*> get_colony() {
		return this->colony;
	}


	//�]�w��l���ڸs
	void set_init_colony(std::vector<T*> init_data) {
		this->colony.assign(init_data.begin(), init_data.end());
	}


	void add_init_colony(T *data) {
		this->colony.push_back(data);
	}

	
	//�ˬdcolony�̬O�_���F��]�w���ؼФF
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


	//�Q��public function��@�X�i�H���檺�t��k
	std::vector<T*> start() {
		//���гy�@�}�l���ڸs
		create_colony();

		int epoch_count = 1;
		while (true) {
			std::cout << "��" << epoch_count << "�N" << std::endl;

			if (epoch_count == this->epoch || goal())	//�p�G�Ounsupervised, ��goal����@�h�û���^flase, �B�P�ɳ]�w�N�ƤW���C �Ϥ��p�G�Osupervised, ��goal�h�n�]�w����, ����epoch�h���ӭn�]��0�C
				break;
			
			epoch_count++;

			//�Ѿ�(�A�t���)�L�{
			selection();

			//���ͤl�N
			reproduction();

			//���ܹL�{
			mutation();
		}

		return get_colony();
	}
};