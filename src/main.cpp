#include "stdafx.h"

int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 20;// ʵ��������ά��
	param.rDim = 20;// ��ϵ������ά��
	param.margin = 1;// Ŀ�꺯���е� margin
	param.batchSize = 100;// SGD �� batchSize
	param.stepSizes = vector<double>{ 0.1, 0.01, 0.001 };// �����仯����

	Dataset& data = FB_B;

	str modelFile, outFile;
	sprintf(modelFile, "../model/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	sprintf(outFile, "../out/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	TransD model(param);


	Train(model, data, modelFile);
	//Load(model, data, modelFile);

	Valid_TC(model, data);
	//Valid_LP(model, data, 200);

	//Load(model, data, modelFile);
	//Test_LP(model, data, outFile);

	system("pause");
}