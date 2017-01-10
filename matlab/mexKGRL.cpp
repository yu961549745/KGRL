#include "mexPrint.h"
#include "stdafx.h"

void printHelp(){
	puts("KGRL");
	puts("-eDim \n\tʵ������ά��");
	puts("-rDim \n\t��ϵ����ά��");
	puts("-margin \n\tĿ�꺯��margin");
	puts("-batchSize \n\tSGD������");
	puts("-errSize \n\tһ����ȷ��Ԫ���������������Ԫ��");
	puts("-validSize \n\t����Ԥ�����֤����");
	puts("-data \n\t���ݼ����ƣ�\n\t��ѡ FB_A|FB_B|WN_A|WN_B");
	puts("-task \n\t�������ƣ�\n\t��ѡ LP_Train|LP_Test|TC_Train|TC_Test");
	puts("\tLP��ʾ����Ԥ�⣬TC��ʾ��Ԫ����࣬Train��ʾѵ������֤��Test��ʾ���Բ����");
	puts("\t����ͬ���Ĳ���ѵ��֮�󣬲��ܽ��в���");
	exit(1);
}

void runKGRL(int eDim, int rDim, double margin, int batchSize, int errSize, char* dataName, char* taskName, int validSize){
	// ��ʼ�����ݼ�
	unordered_map<string, Dataset> dataMap;
	dataMap["FB_A"] = FB_A;
	dataMap["FB_B"] = FB_B;
	dataMap["WN_A"] = WN_A;
	dataMap["WN_B"] = WN_B;

	// Ĭ�ϲ���
	ModelParam param;
	param.eDim = eDim;// ʵ��������ά��
	param.rDim = rDim;// ��ϵ������ά��
	param.margin = margin;// Ŀ�꺯���е� margin
	param.batchSize = batchSize;// SGD �� batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// �����仯����
	param.errSize = errSize;// һ����ȷ��Ԫ���������������Ԫ��

	// ���������Ϣ
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

	// ѵ�� FB_A / WN_A
	if (strcmp(taskName, "LP_Train")==0) 
		TrainAndValidLP(model, data, modelFile, validSize);
	// ���ñ����ģ�Ͳ��� FB_A / WN_A
	else if (strcmp(taskName, "LP_Test")==0)
		LoadAndTestLP(model, data, modelFile, outFile);
	// ѵ�� FB_B / WN_B
	else if (strcmp(taskName, "TC_Train")==0) 
		TrainAndValidTC(model, data, modelFile);
	// ���ñ����ģ�Ͳ��� FB_B / WN_B
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