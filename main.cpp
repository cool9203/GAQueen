#include "Genetic_Algorithm.h"
#include "Feature.h"
#include "NQueen.h"


int main() {
	int group_size = 500;			//colony size, 群體上限
	int mutation_rate = 10;			//突變率 1=1/1000
	int base_survival = 0;			//基礎存活率
	int survival_size = 10;			//限定每次天擇的存活上限(還是有機會會活超過這數字,因為好的基因會高機率留下來) input 0 = no set
	int foreigner = 6;				//在產生子代時，是否固定增加全新個體, 0 -> no set, > 0 -> 固定增加N個新個體。   (外國人移民的概念)
	int eugenics = 0;				//優生學:好的會優先跟好的交換基因, 好的換完後會再整個倖存族群全部隨機交換基因.  0 -> no set, >0 -> 存活率>=eugenics的會當作好的基因.
	double power_survival = 2.0;	//x^n的存活率機制, 可以讓好的留下來, 並快速淘汰中下的
	bool parents_safe = false;		//是否留存父母在基因庫裡
	int epoch = 0;					//訓練代數上限, 如果是unsupervised的訓練, 那應該要>=1. 反之若是supervised, 則應該=0.

	
	Genetic_Algorithm<FEATURE> ga(group_size, mutation_rate, base_survival, survival_size, foreigner, eugenics, parents_safe, power_survival, epoch);
	std::vector<FEATURE*> data = ga.start();

	for (int i = 0; i < data.size(); i++) {
		if (data[i]->goal()) {
			data[i]->show();
			std::cout << std::endl;
		}
	}

	system("PAUSE");
	return 0;
}