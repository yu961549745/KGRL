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
int main(int argc, char* argv[]){
	//srand((unsigned int)time(NULL));
	srand(0);

	// 初始化数据集
	unordered_map<string, Dataset> dataMap;
	dataMap["FB_A"] = FB_A;
	dataMap["FB_B"] = FB_B;
	dataMap["WN_A"] = WN_A;
	dataMap["WN_B"] = WN_B;

	// 默认参数
	ModelParam param;
	param.eDim = 50;// 实体向量的维数
	param.rDim = 50;// 关系向量的维数
	param.margin = 0.5;// 目标函数中的 margin
	param.batchSize = 100;// SGD 的 batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// 步长变化向量
	param.errSize = 1;// 一个正确三元组采样几个错误三元组

	int validSize = 200;// 在进行链接预测时，因为耗时较长，
	// 因此只取验证数据集的前几个数据进行验证
	str dataName = "FB_A";
	str taskName = "LP_Train";

	// 命令行参数处理
	for (int k = 1; k < argc; k++){
		char* s = argv[k];
		if (s[0] = '-'){
			if (strcmp(s, "-eDim") == 0) sscanf(argv[++k], "%d", &param.eDim);
			else if (strcmp(s, "-rDim") == 0) sscanf(argv[++k], "%d", &param.rDim);
			else if (strcmp(s, "-margin") == 0) sscanf(argv[++k], "%lf", &param.margin);
			else if (strcmp(s, "-batchSize") == 0) sscanf(argv[++k], "%d", &param.batchSize);
			else if (strcmp(s, "-errSize") == 0) sscanf(argv[++k], "%d", &param.errSize);
			else if (strcmp(s, "-validSize") == 0) sscanf(argv[++k], "%d", &validSize);
			else if (strcmp(s, "-data") == 0) sscanf(argv[++k], "%s", dataName);
			else if (strcmp(s, "-task") == 0) sscanf(argv[++k], "%s", taskName);
			else printHelp();
		}
	}

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

	if (argc < 2){
		system("pause");
	}
}