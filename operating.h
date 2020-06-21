#pragma once
#include <random>
#include <regex>

namespace operating {
	//���o�p�g�r��
	static std::string lower(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
		return str;
	}

	//��regex��@string.split()
	static std::vector<std::string> split(std::string s, std::string rgx_str) {
		std::vector<std::string> elems;
		std::regex rgx(rgx_str);
		std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
		std::sregex_token_iterator end;

		while (iter != end) {
			elems.push_back(*iter);
			++iter;
		}
		return elems;
	}

	//���w�@��seed�A�M�ᥴ�ð}�C����
	template <typename T>
	static void shuffle(int seed, std::vector<T>& data) {
		if (data.size() < 2)
			return;

		std::default_random_engine generator(seed);								//��l��
		std::uniform_int_distribution<int> distribution(0, data.size() - 1);	//�]�w�d��P�M�w�n���ͤ����������ü� ex:int

		int max_count = distribution(generator) + data.size();
		for (int i = 0; i < max_count; i++) {
			int index1 = distribution(generator);
			int index2 = distribution(generator);
			while (index1 == index2) {
				index2 = distribution(generator);
			}

			std::swap(data[index1], data[index2]);
		}
	}

	//shuffle overlodding : ���w�Ʀr�U���M�W���Areturn vector
	//how to use: operating::shuffle<int>(seed, n, n+size());
	template <typename T>
	static std::vector<T> shuffle(int seed, int start_num, int end_num) {
		std::vector<T> data;

		for (int i = start_num; i <= end_num; i++)
			data.push_back(i);

		shuffle(seed, data);
		return data;
	}
}
