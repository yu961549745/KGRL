#include "mexPrint.h"
#include "stdafx.h"

void printHelp(){
	puts("KGRL");
	puts("-eDim \n\t实体向量维数");
	puts("-rDim \n\t关系向量维数");
	puts("-margin \n\t目标函数margin");
	puts("-batchSize \n\tSGD采样量");
	puts("-errSize \n\t一个正确三元组采样几个错误三元组");
	puts("-validSize \n\t链接预测的验证数量");
	puts("-data \n\t数据集名称，\n\t可选 FB_A|FB_B|WN_A|WN_B");
	puts("-task \n\t任务名称，\n\t可选 LP_Train|LP_Test|TC_Train|TC_Test");
	puts("\tLP表示链接预测，TC表示三元组分类，Train表示训练并验证，Test表示测试并输出");
	puts("\t必须同样的参数训练之后，才能进行测试");
	exit(1);
}

void runKGRL(int eDim, int rDim, double margin, int batchSize, int errSize, char* dataName, char* taskName, int validSize){
	// 初始化数据集
	unordered_map<string, Dataset> dataMap;
	dataMap["FB_A"] = FB_A;
	dataMap["FB_B"] = FB_B;
	dataMap["WN_A"] = WN_A;
	dataMap["WN_B"] = WN_B;

	// 默认参数
	ModelParam param;
	param.eDim = eDim;// 实体向量的维数
	param.rDim = rDim;// 关系向量的维数
	param.margin = margin;// 目标函数中的 margin
	param.batchSize = batchSize;// SGD 的 batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// 步长变化向量
	param.errSize = errSize;// 一个正确三元组采样几个错误三元组

	// 输出参数信息
	puts("Params...");
	printf("eDim=%d\n", param.eDim);
	printf("rDim=%d\n", param.rDim);
	printf("margin=%g\n", param.margin);
	printf("batchSize=%d\n", param.batchSize);
	printf("errSize=%d\n", param.errSize);
	printf("validSize=%d\n", validSize);
	printf("data=%s\n", dataName);
	printf("task=%s\n", taskName);
	puts("...........................\n");

	TransD model(param);
	Dataset data = dataMap[dataName];
	str modelFile, outFile;
	sprintf(modelFile, "../model/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	sprintf(outFile, "../out/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);

	// 训练 FB_A / WN_A
	if (strcmp(taskName, "LP_Train")==0) 
		TrainAndValidLP(model, data, modelFile, validSize);
	// 利用保存的模型测试 FB_A / WN_A
	else if (strcmp(taskName, "LP_Test")==0)
		LoadAndTestLP(model, data, modelFile, outFile);
	// 训练 FB_B / WN_B
	else if (strcmp(taskName, "TC_Train")==0) 
		TrainAndValidTC(model, data, modelFile);
	// 利用保存的模型测试 FB_B / WN_B
	else if (strcmp(taskName, "TC_Test")==0) 
		LoadAndTestTC(model, data, modelFile, outFile);
	else 
		printHelp();

	puts("-------------END-------------");
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray* prhs[]){
	int eDim = mxGetScalar(prhs[0]);
	int rDim = mxGetScalar(prhs[1]);
	double margin = mxGetScalar(prhs[2]);
	int batchSize = mxGetScalar(prhs[3]);
	int errSize = mxGetScalar(prhs[4]);
	char* dataName = mxArrayToString(prhs[5]);
	char* taskName = mxArrayToString(prhs[6]);
	int validSize = mxGetScalar(prhs[7]);
	runKGRL(eDim, rDim, margin, batchSize, errSize, dataName, taskName, validSize);
}