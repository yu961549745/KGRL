#include "stdafx.h"

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
	char* outFile = "../out/FB_A_tmp.txt";

	TransD model(param);

	//Train(model, FB_A, modelFile);
	Load(model, FB_A, modelFile);
	Valid_LP(model, FB_A);

	//Load(model, FB_A, modelFile);
	//Test_LP(model, FB_A, outFile);

	system("pause");
}