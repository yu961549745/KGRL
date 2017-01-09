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
int main(int argc, char* argv[]){
	//srand((unsigned int)time(NULL));
	srand(0);

	// ��ʼ�����ݼ�
	unordered_map<string, Dataset> dataMap;
	dataMap["FB_A"] = FB_A;
	dataMap["FB_B"] = FB_B;
	dataMap["WN_A"] = WN_A;
	dataMap["WN_B"] = WN_B;

	// Ĭ�ϲ���
	ModelParam param;
	param.eDim = 50;// ʵ��������ά��
	param.rDim = 50;// ��ϵ������ά��
	param.margin = 0.5;// Ŀ�꺯���е� margin
	param.batchSize = 100;// SGD �� batchSize
	param.stepSizes = vector<double>{ 10, 1, 0.1 };// �����仯����
	param.errSize = 1;// һ����ȷ��Ԫ���������������Ԫ��

	int validSize = 200;// �ڽ�������Ԥ��ʱ����Ϊ��ʱ�ϳ���
	// ���ֻȡ��֤���ݼ���ǰ�������ݽ�����֤
	str dataName = "FB_A";
	str taskName = "LP_Train";

	// �����в�������
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

	if (argc < 2){
		system("pause");
	}
}