#include "Genetic_Algorithm.h"
#include "Feature.h"
#include "NQueen.h"


int main() {
	int group_size = 500;			//colony size, �s��W��
	int mutation_rate = 10;			//���ܲv 1=1/1000
	int base_survival = 0;			//��¦�s���v
	int survival_size = 10;			//���w�C���Ѿܪ��s���W��(�٬O�����|�|���W�L�o�Ʀr,�]���n����]�|�����v�d�U��) input 0 = no set
	int foreigner = 6;				//�b���ͤl�N�ɡA�O�_�T�w�W�[���s����, 0 -> no set, > 0 -> �T�w�W�[N�ӷs����C   (�~��H����������)
	int eugenics = 0;				//�u�;�:�n���|�u����n���洫��], �n��������|�A��ӭƦs�ڸs�����H���洫��].  0 -> no set, >0 -> �s���v>=eugenics���|��@�n����].
	double power_survival = 2.0;	//x^n���s���v����, �i�H���n���d�U��, �çֳt�^�O���U��
	bool parents_safe = false;		//�O�_�d�s�����b��]�w��
	int epoch = 0;					//�V�m�N�ƤW��, �p�G�Ounsupervised���V�m, �����ӭn>=1. �Ϥ��Y�Osupervised, �h����=0.

	
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