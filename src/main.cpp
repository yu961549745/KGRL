#include "stdafx.h"

int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 20;// 实体向量的维数
	param.rDim = 20;// 关系向量的维数
	param.margin = 0.5;// 目标函数中的 margin
	param.batchSize = 100;// SGD 的 batchSize
	param.stepSizes = vector<double>{ 0.1, 0.01, 0.001 };// 步长变化向量

	Dataset& data = FB_A;

	str modelFile, outFile;
	sprintf(modelFile, "../model/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	sprintf(outFile, "../out/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	TransD model(param);

	Train(model, data, modelFile);
	//Load(model, data, modelFile);
	Valid_LP(model, data);

	//Load(model, data, modelFile);
	//Test_LP(model, data, outFile);

	system("pause");
}