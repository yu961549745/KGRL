#include "stdafx.h"
void TransD_Train_LP(ModelParam& param, Dataset& data, char* outfile){
	TransD model(param);
	KG& kg = model.kg;

	tic("Loading Knowledge Graph ...");
	loadKG(kg, data.getTrain());
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Init Embedding Space...");
	printf("eDim = %d\nrDim = %d\n", param.eDim, param.rDim);
	model.init();
	toc();

	tic("Trainning...");
	model.train();
	toc();

	tic("Loading valid data...");
	SubKG valid;
	loadSubKG(valid, kg, data.getValid());
	// 添加验证数据集中的三元组，保证验证数据集中的三元组被认为是正确的。
	loadKG(kg, "../data/FB_A/valid.txt");
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	// 预测的运算比较耗时，先取前几个用于测试
	SubKG minValid(valid.begin(), valid.begin() + 100);

	tic("Predicting on valid data...");
	printf("Hist@10 = %g\n", model.predictHead(minValid, 10));
	toc();

	tic("Saving ES...");
	model.saveES(outfile);
	toc();

	showMemory();
}
void TransD_Test_LP(ModelParam& param, Dataset& data, char* infile, char* outfile){
	TransD model(param);
	KG& kg = model.kg;

	tic("Loading Knowledge Graph ...");
	loadKG(kg, data.getTrain());
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Loading Model ...");
	model.loadES(infile);
	toc();

	SubKG test;
	tic("Loading test data");
	loadSubKGWithoutHead(test, kg, data.getTest());
	toc();

	tic("Predicting on test data...");
	FILE* fid = fopen(outfile, "w");
	for (int i = 0; i < test.size(); i++){
		Triple t = test[i];
		vector<idtype> out;
		model.predictHeads(out, t, 10);
		fprintf(fid, "%s %s", kg.getRelation(t.r), kg.getEntity(t.t));
		for (int j = 0; j < 10; j++){
			fprintf(fid, " %s", kg.getEntity(out[j]));
		}
		fprintf(fid, "\n");
		if ((i + 1) % 100 == 0){
			printf("%d / %d\r", i + 1, test.size());
		}
	}
	printf("%30s", " ");
	fclose(fid);
	toc();
}
int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 20;// 实体向量的维数
	param.rDim = 20;// 关系向量的维数
	param.margin = 0.5;// 目标函数中的 margin
	param.batchSize = 100;// SGD 的 batchSize
	param.trainPeriod = 2000;// 计算目标函数值的周期
	param.stepSizes = vector<double>{ 0.1, 0.01, 0.001 };// 步长变化向量

	char* modelFile = "../model/FB_A.txt";
	char* outFile = "../out/FB_A.txt";

	//TransD_Train_LP(param, FB_A, modelFile);

	TransD_Test_LP(param, FB_A, modelFile, outFile);

	system("pause");
}