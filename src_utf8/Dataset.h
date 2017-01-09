// 定义数据集
#ifndef _DATASET_H_
#define _DATASET_H_

#include "stdafx.h"

enum DataType{ LP, TC };

// 数据集
class Dataset{
private:
	string name, train, valid, test;
public:
	DataType type;
	Dataset(){}
	Dataset(DataType _type, char* _name, char* _path, char* _train, char* _valid, char* _test){
		type = _type;
		name = _name;
		train = _path; train += _train;
		valid = _path; valid += _valid;
		test = _path; test += _test;
	}
	char* getTrain(){
		return (char*)train.c_str();
	}
	char* getValid(){
		return (char*)valid.c_str();
	}
	char* getTest(){
		return (char*)test.c_str();
	}
	char* getName(){
		return (char*)name.c_str();
	}
};

Dataset FB_A(LP, "FB_A", "../data/FB_A/", "train.txt", "valid.txt", "test.txt");
Dataset FB_B(TC, "FB_B", "../data/FB_B/", "train.txt", "valid.txt", "test.txt");
Dataset WN_A(LP, "WN_A", "../data/WN_A/", "train.txt", "valid.txt", "test.txt");
Dataset WN_B(TC, "WN_B", "../data/WN_B/", "train.txt", "valid.txt", "test.txt");

#endif
