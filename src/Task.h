// 任务：训练、验证、链接预测、三元组分类
#ifndef _TASK_H_
#define _TASK_H_

#include "stdafx.h"

// 模型训练并保存
void Train(Model& model, Dataset& data, char* outfile){
	KG& kg = model.kg;

	tic("Loading Knowledge Graph ...");
	loadKG(kg, data.getTrain());
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Init Embedding Space...");
	printf("eDim = %d\nrDim = %d\n", model.eDim, model.rDim);
	model.init();
	toc();

	tic("Trainning...");
	model.train();
	toc();

	tic("Saving ES...");
	model.saveES(outfile);
	toc();
}

// 加载模型
void Load(Model& model, Dataset& data, char* infile){
	KG& kg = model.kg;

	tic("Loading Knowledge Graph ...");
	loadKG(kg, data.getTrain());
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Loading Model ...");
	model.loadES(infile);
	toc();
}

// 根据链接预测验证模型效果
void Valid_LP(Model& model, Dataset& data){
	KG& kg = model.kg;

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
}

// 进行链接预测的测试
void Test_LP(Model& model, Dataset& data, char* outfile){
	KG& kg = model.kg;

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
	printf("%30s\r", " ");
	fclose(fid);
	toc();
}

#endif