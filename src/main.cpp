#include "stdafx.h"

int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 20;// ʵ��������ά��
	param.rDim = 20;// ��ϵ������ά��
	param.margin = 0.5;// Ŀ�꺯���е� margin
	param.batchSize = 100;// SGD �� batchSize
	param.trainPeriod = 2000;// ����Ŀ�꺯��ֵ������
	param.stepSizes = vector<double>{ 0.1, 0.01, 0.001 };// �����仯����

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