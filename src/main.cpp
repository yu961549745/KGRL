#include "stdafx.h"
int main(){
	srand((unsigned int)time(NULL));

	ModelParam param;
	param.eDim = 10;// ʵ��������ά��
	param.rDim = 10;// ��ϵ������ά��
	param.margin = 0.5;// Ŀ�꺯���е� margin
	param.batchSize = 100;// SGD �� batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// �����仯����
	param.errSize = 1;// һ����ȷ��Ԫ���������������Ԫ��

	// �ڽ�������Ԥ��ʱ����Ϊ��ʱ�ϳ������ֻȡ��֤���ݼ���ǰ�������ݽ�����֤
	int validSize = 200;

	TransD model(param);

	Dataset& data = FB_A;

	str modelFile, outFile;
	sprintf(modelFile, "../model/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);
	sprintf(outFile, "../out/%s_%d_%d.txt", data.getName(), param.eDim, param.rDim);

	TrainAndValidLP(model, data, modelFile, validSize);     // ѵ�� FB_A / WN_A
	//LoadAndTestLP(model, data, modelFile, outFile);       // ���ñ����ģ�Ͳ��� FB_A / WN_A
	//TrainAndValidTC(model, data, modelFile);              // ѵ�� FB_B / WN_B
	//LoadAndTestTC(model, data, modelFile, outFile);	    // ���ñ����ģ�Ͳ��� FB_B / WN_B

	system("pause");
}