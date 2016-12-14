/*
	Knowledge Graph 相关数据结构
*/
#ifndef _KG_H_
#define _KG_H_

#include "stdafx.h"
/*
三元组数据结构
保存的是KG中string对象的c_str()指针
*/
class Triple{
public:
	char *h, *r, *t;
	Triple(char* _h, char* _r, char* _t){
		h = _h; r = _r; t = _t;
	}
	friend ostream & operator<<(ostream &os, const Triple &t){
		os << "( " << t.h << " , " << t.r << " , " << t.t << " )" << endl;
		return os;
	}
};
struct TripleHash{
	size_t operator()(const Triple& k) const{
		hash<char*> h;
		return h(k.h) ^ h(k.r) ^ h(k.t);
	}
};
struct TripleEqual{
	bool operator()(const Triple& a, const Triple& b) const{
		return a.h == b.h && a.r == b.r && a.t == b.t;
	}
};

/*
知识图谱数据结构：
T 三元组map，键为三元组，值为正确与否
E 实体map，键为实体，值为对应字符串指针
R 关系map，键为实体，值为对应字符串指针
*/
class KG{
public:
	unordered_map<Triple, bool, TripleHash, TripleEqual> T;
	unordered_map<string, char*> E, R;
	// 添加三元组，自动去重
	void addTriple(string h, string r, string t, bool type = true){
		char *ph = addEntity(h);
		char *pr = addRelation(r);
		char *pt = addEntity(t);
		Triple tt(ph, pr, pt);
		if (T.count(tt) == 0){
			T[tt] = type;
			TV.push_back(tt);
		}
	}
	// 随机三元组
	Triple rndTriple(){
		return TV[rand() % TV.size()];
	}
	// 随机实体
	char* rndEntity(){
		return EV[rand() % EV.size()];
	}
	// 随机关系
	char* rndRelation(){
		return RV[rand() % RV.size()];
	}
private:
	vector<char*> EV, RV;
	vector<Triple> TV;
	char* addEntity(string& s){
		if (E[s] == NULL){
			E[s] = (char*)(E.find(s)->first.c_str());
			EV.push_back(E[s]);
		}
		return E[s];
	}
	char* addRelation(string& s){
		if (R[s] == NULL){
			R[s] = (char*)(R.find(s)->first.c_str());
			RV.push_back(R[s]);
		}
		return R[s];
	}
};

// 子图谱，其中Tiiple的键以父图谱为准
typedef vector<Triple> SubKG;

#endif
