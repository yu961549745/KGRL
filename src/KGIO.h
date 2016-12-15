// Knowledge Graph ���������غ���
#ifndef _KGIO_H_
#define _KGIO_H_

#include "stdafx.h"

#define BUFF_LEN 1024
// ��ʱ�ַ���
typedef char str[BUFF_LEN];

// ���� (h,r,t) ��Ԫ��
void loadKG(KG& kg, char* path){
	str h, r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		kg.addTriple(h, r, t);
	}
	fclose(fid);
}

// ������ͼ��
void loadSubKG(SubKG& dst, KG& base, char* path){
	str h, r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		dst.push_back(Triple(base.E[h], base.R[r], base.E[t]));
	}
	fclose(fid);
}

// ������ͼ�ף�pos��ʾ��ȷ��Ԫ�飬neg��ʾ������Ԫ��
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

// ���� (*,r,t) ��ҪԤ�� h
void loadSubKGWithoutHead(SubKG& dst, KG& base, char* path){
	str r, t;
	FILE* fid = fopen(path, "r");
	while (fscanf(fid, "%s %s", r, t) != EOF){
		dst.push_back(Triple(0, base.R[r], base.E[t]));
	}
	fclose(fid);
}

// �������
void fprintMat(FILE* fid, mat& v){
	int m = v.n_rows;
	int n = v.n_cols;
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			fprintf(fid, "%lf ", v(i, j));
		}
		fprintf(fid, "\n");
	}
}

// �������
void fscanMat(FILE*fid, mat& v){
	int m = v.n_rows;
	int n = v.n_cols;
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			fscanf(fid, "%lf", &v(i, j));
		}
	}
}


void completeKG_LP(KG& kg, Dataset& data){
	str h, r, t;
	FILE* fid = fopen(data.getValid(), "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		kg.addEntity(h);
		kg.addRelation(r);
		kg.addEntity(t);
	}
	fclose(fid);
	fid = fopen(data.getTest(), "r");
	while (fscanf(fid, "%s %s", r, t) != EOF){
		kg.addRelation(r);
		kg.addEntity(t);
	}
	fclose(fid);
}

void completeKG_TC(KG& kg, Dataset& data){
	str h, r, t;
	FILE* fid = fopen(data.getValid(), "r");
	while (fscanf(fid, "%s %s %s %*d", h, r, t) != EOF){
		kg.addEntity(h);
		kg.addRelation(r);
		kg.addEntity(t);
	}
	fclose(fid);
	fid = fopen(data.getTest(), "r");
	while (fscanf(fid, "%s %s %s", h, r, t) != EOF){
		kg.addEntity(h);
		kg.addRelation(r);
		kg.addEntity(t);
	}
	fclose(fid);
}

// ������֤���ϲ��Լ��ϵ�ʵ��͹�ϵ
void completeKG(KG& kg, Dataset& data){
	if (data.type == LP){
		completeKG_LP(kg, data);
	}
	else{
		completeKG_TC(kg, data);
	}
}

#endif