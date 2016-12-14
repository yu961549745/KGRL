// Knowledge Graph 输入输出相关函数
#ifndef _KGIO_H_
#define _KGIO_H_

#include "stdafx.h"

#define BUFF_LEN 1024
// 临时字符串
typedef char str[BUFF_LEN];

// 加载 (h,r,t) 三元组
void loadKG(KG& kg, char* path){
	str h, r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		kg.addTriple(h, r, t);
	}
	fclose(fid);
}

// 加载子图谱
void loadSubKG(SubKG& dst, KG& base, char* path){
	str h, r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		dst.push_back(Triple(base.E[h], base.R[r], base.E[t]));
	}
	fclose(fid);
}

// 加载子图谱，pos表示正确三元组，neg表示错误三元组
void loadSubKGWithType(SubKG& pos,SubKG& neg, KG& base, char* path){
	str h, r, t;
	int type;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s %s %d", h, r, t, &type) != EOF){
		if (type == 1){
			pos.push_back(Triple(base.E[h], base.R[r], base.E[t]));
		}
		else{
			neg.push_back(Triple(base.E[h], base.R[r], base.E[t]));
		}
	}
	fclose(fid);
}

// 加载 (*,r,t) 需要预测 h
void loadSubKGWithoutHead(SubKG& dst, KG& base, char* path){
	str r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s", r, t) != EOF){
		dst.push_back(Triple(0, base.R[r], base.E[t]));
	}
	fclose(fid);
}

#endif