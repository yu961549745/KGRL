#include "stdafx.h"
int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 10;// 实体向量的维数
	param.rDim = 10;// 关系向量的维数
	param.margin = 0.5;// 目标函数中的 margin
	param.batchSize = 100;// SGD 的 batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// 步长变化向量
	param.errSize = 1;// 一个正确三元组采样几个错误三元组

	// 在进行链接预测时，因为耗时较长，因此只取验证数据集的前几个数据进行验证
	int validSize = 200;

	TransD model(param);

	Dataset& data = FB_A;

	str modelFile, outFile;
	sprintf(modelFile, "../model/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	sprintf(outFile, "../out/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);

	TrainAndValidLP(model, data, modelFile, validSize);     // 训练 FB_A / WN_A
	//LoadAndTestLP(model, data, modelFile, outFile);       // 利用保存的模型测试 FB_A / WN_A
	//TrainAndValidTC(model, data, modelFile);              // 训练 FB_B / WN_B
	//LoadAndTestTC(model, data, modelFile, outFile);	    // 利用保存的模型测试 FB_B / WN_B

	system("pause");
}