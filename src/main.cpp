#include "stdafx.h"

int main(){
	srand((unsigned int)time(NULL));
	int eDim = 5;// 实体向量的维数
	int rDim = 5;// 关系向量的维数
	double margin = 0.5;// 目标函数中的 margin
	int batchSize = 100;// SGD 的 batchSize
	int trainPeriod = 2000;// 计算目标函数值的周期
	vector<double> stepSizes{ 0.1, 0.01, 0.001 };// 步长变化向量

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
	// 添加验证数据集中的三元组，保证验证数据集中的三元组被认为是正确的。
	loadKG(kg, "../data/FB_A/valid.txt");
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	// 预测的运算比较耗时，先取前几个用于测试
	SubKG minValid(valid.begin(), valid.begin() + 100);

	tic("Predicting on valid data...");
	printf("Hist@10 = %g\n", model.predictHead(minValid, 10));
	toc();

	SubKG test;
	tic("Loading test data");
	loadSubKGWithoutHead(test, kg, "../data/FB_A/valid.txt");
	toc();

	// TODO 这里可能存在有未知实体或关系的问题
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