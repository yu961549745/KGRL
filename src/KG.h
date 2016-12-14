/*
	Knowledge Graph 相关数据结构
*/
#ifndef _KG_H_
#define _KG_H_

#include "stdafx.h"

typedef int idtype;
/*
三元组数据结构
保存的是KG中string对象的c_str()指针
*/
class Triple{
public:
	idtype h, r, t;
	Triple(idtype _h, idtype _r, idtype _t){
		h = _h; r = _r; t = _t;
	}
};
struct TripleHash{
	size_t operator()(const Triple& k) const{
		hash<idtype> h;
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
*/
class KG{
public:
	unordered_map<Triple, bool, TripleHash, TripleEqual> T;
	unordered_map<string, idtype> E, R;
	vector<Triple> TV;
	vector<string> EV, RV;
	// 添加三元组，自动去重
	void addTriple(string h, string r, string t, bool type = true){
		idtype ph = addEntity(h);
		idtype pr = addRelation(r);
		idtype pt = addEntity(t);
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
	idtype rndEntity(){
		return (rand() % (idtype)EV.size() + 1);
	}
	// 随机关系
	idtype rndRelation(){
		return -(rand() % (idtype)RV.size() + 1);
	}
private:
	idtype addEntity(string& s){
		if (E.count(s)==0){
			EV.push_back(s);
			E[s] = (idtype)EV.size();
		}
		return E[s];
	}
	idtype addRelation(string& s){
		if (R.count(s) == 0){
			RV.push_back(s);
			R[s] = -(idtype)RV.size();
		}
		return R[s];
	}
};

// 子图谱，其中Tiiple的键以父图谱为准
typedef vector<Triple> SubKG;

#endif
