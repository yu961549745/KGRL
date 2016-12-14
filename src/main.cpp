#include "stdafx.h"

int main(){
	srand((unsigned int)time(NULL));
	int eDim = 5;// ʵ��������ά��
	int rDim = 5;// ��ϵ������ά��
	double margin = 0.5;// Ŀ�꺯���е� margin
	int batchSize = 100;// SGD �� batchSize
	int trainPeriod = 2000;// ����Ŀ�꺯��ֵ������
	vector<double> stepSizes{ 0.1, 0.01, 0.001 };// �����仯����

	TransD model(eDim, rDim, margin, batchSize, trainPeriod, stepSizes);

	tic("Loading Knowledge Graph ...");
	KG& kg = model.kg;
	loadKG(kg, "../data/FB_A/train.txt");
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Init Embedding Space...");
	printf("eDim = %d\nrDim = %d\n", eDim, rDim);
	model.init();
	toc();

	tic("Trainning...");
	model.train();
	toc();

	tic("Loading valid data...");
	SubKG valid;
	loadSubKG(valid, kg, "../data/FB_A/valid.txt");
	// �����֤���ݼ��е���Ԫ�飬��֤��֤���ݼ��е���Ԫ�鱻��Ϊ����ȷ�ġ�
	loadKG(kg, "../data/FB_A/valid.txt");
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	// Ԥ�������ȽϺ�ʱ����ȡǰ�������ڲ���
	SubKG minValid(valid.begin(), valid.begin() + 100);

	tic("Predicting on valid data...");
	printf("Hist@10 = %g\n", model.predictHead(minValid, 10));
	toc();

	SubKG test;
	tic("Loading test data");
	loadSubKGWithoutHead(test, kg, "../data/FB_A/valid.txt");
	toc();

	// TODO ������ܴ�����δ֪ʵ����ϵ������
	tic("Predicting on test data...");
	char* outfile = "../FB_A_out.txt";
	FILE* fid = fopen(outfile, "w");
	for (int i = 0; i < test.size();i++){
		vector<char*> out;
		Triple t = test[i];
		model.predictHeads(out, t, 10);
		fprintf(fid, "%s %s", t.r, t.t);
		for (int j = 0; j < 10; j++){
			fprintf(fid, " %s", out[j]);
		}
		fprintf(fid, "\n");
		if ((i + 1) % 100 == 0){
			printf("%d / %d\r", i + 1, test.size());
		}
	}
	printf("%30s", " ");
	fclose(fid);
	toc();

	showMemory();
	system("pause");
}