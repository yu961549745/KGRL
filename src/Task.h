// 任务：训练、验证、链接预测、三元组分类
#ifndef _TASK_H_
#define _TASK_H_

#include "stdafx.h"

// 加载知识图谱并补全
void loadData(Model& model, Dataset& data){
	KG& kg = model.kg;
	tic("Loading Knowledge Graph ...");
	loadKG(kg, data.getTrain());
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	tic("Complete Knowledge Graph ...");
	completeKG(kg, data);
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();
}

// 模型训练并保存
void Train(Model& model, Dataset& data, char* outfile){
	loadData(model, data);

	tic("Init Embedding Space...");
	printf("eDim = %d\nrDim = %d\n", model.eDim, model.rDim);
	model.init();
	toc();

	tic("Trainning...");
	model.train();
	toc();

	tic("Saving Model...");
	model.save(outfile);
	toc();
}

// 加载模型
void Load(Model& model, Dataset& data, char* infile){
	loadData(model, data);

	tic("Loading Model ...");
	model.load(infile);
	toc();
}

// 根据链接预测验证模型效果
void Valid_LP(Model& model, Dataset& data, int validSize){
	KG& kg = model.kg;

	tic("Loading valid data...");
	SubKG valid;
	loadSubKG(valid, kg, data.getValid());
	// 添加验证数据集中的三元组，保证验证数据集中的三元组被认为是正确的。
	loadKG(kg, "../data/FB_A/valid.txt");
	printf("T %d\nE %d\nR %d\n", kg.T.size(), kg.E.size(), kg.R.size());
	toc();

	// 预测的运算比较耗时，先取前几个用于测试
	SubKG minValid(valid.begin(), valid.begin() + validSize);

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

// 三元组分类验证，同时确定最优阈值
void Valid_TC(Model& model, Dataset& data, char* outPath = NULL){
	SubKG pos, neg;

	tic("Loading valid data...");
	loadSubKGWithType(pos, neg, model.kg, data.getValid());
	toc();

	tic("Calculating fscores ...");
	vector<double> vpos, vneg;
	for (auto i = pos.begin(); i != pos.end(); i++){
		double f = model.fscore(*i);
		vpos.push_back(f);
	}
	for (auto i = neg.begin(); i != neg.end(); i++){
		double f = model.fscore(*i);
		vneg.push_back(f);
	}
	toc();

	// 输出到Matlab目录用于作图
	if (outPath != NULL){
		str path;
		sprintf(path, "%s/p.txt", outPath);
		FILE* fid = fopen(path, "w");
		for (auto i = vpos.begin(); i != vpos.end(); i++){
			fprintf(fid, "%lf\n", *i);
		}
		fclose(fid);
		sprintf(path, "%s/n.txt", outPath);
		fid = fopen(path, "w");
		for (auto i = vneg.begin(); i != vneg.end(); i++){
			fprintf(fid, "%lf\n", *i);
		}
		fclose(fid);
	}

	tic("Calculationg best threash...");
	sort(vpos.begin(), vpos.end());
	sort(vneg.begin(), vneg.end());
	int NP = (int)vpos.size(), NN = (int)vneg.size();
	double thresh = 0, bestThresh = 0;
	int count = NN, maxCount = NN;
	int pp = 0, pn = 0;
	while (true){
		if (pp < NP && pn < NN){
			if (vpos[pp] <= vneg[pn]){
				thresh = vpos[pp++];
			}
			else {
				thresh = vneg[pn++];
			}
		}
		else if (pp >= NP && pn < NN){
			thresh = vneg[pn++];
		}
		else if (pp < NP && pn >= NN){
			thresh = vpos[pp++];
		}
		else{
			break;
		}
		count = pp + NN - pn;
		if (count > maxCount){
			maxCount = count;
			bestThresh = thresh;
		}
	}
	double accuracy = 1.0*maxCount / (NP + NN);
	printf("%lf -> %lf\n", bestThresh, accuracy);
	toc();

	model.thresh = bestThresh;
}

void Test_TC(Model& model, Dataset& data, char* outfile){
	SubKG test;
	KG& kg = model.kg;

	tic("Loading test data ...");
	loadSubKG(test, kg, data.getTest());
	toc();

	tic("Classify ...");
	FILE* fid = fopen(outfile, "w");
	for (auto i = test.begin(); i != test.end(); i++){
		Triple& t = *i;
		int type = model.fscore(t) <= model.thresh ? 1 : -1;
		fprintf(fid, "%s %s %s %d\n", 
			kg.getEntity(t.h), kg.getRelation(t.r), kg.getEntity(t.t), type);
	}
	fclose(fid);
	toc();
}

void TrainAndValidLP(Model& model, Dataset& data, char* modelFile, int validSize){
	Train(model, data, modelFile);
	Valid_LP(model, data, validSize);
}
void LoadAndTestLP(Model& model, Dataset& data, char* modelFile, char* outFile){
	Load(model, data, modelFile);
	Test_LP(model, data, outFile);
}
void TrainAndValidTC(Model& model, Dataset& data, char* modelFile){
	Train(model, data, modelFile);
	Valid_TC(model, data);
}
void LoadAndTestTC(Model& model, Dataset& data, char* modelFile, char* outFile){
	Load(model, data, modelFile);
	Valid_TC(model, data);
	Test_TC(model, data, outFile);
}

#endif