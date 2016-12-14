/*
	Knowledge Graph ������ݽṹ
*/
#ifndef _KG_H_
#define _KG_H_

#include "stdafx.h"
/*
��Ԫ�����ݽṹ
�������KG��string�����c_str()ָ��
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
֪ʶͼ�����ݽṹ��
T ��Ԫ��map����Ϊ��Ԫ�飬ֵΪ��ȷ���
E ʵ��map����Ϊʵ�壬ֵΪ��Ӧ�ַ���ָ��
R ��ϵmap����Ϊʵ�壬ֵΪ��Ӧ�ַ���ָ��
*/
class KG{
public:
	unordered_map<Triple, bool, TripleHash, TripleEqual> T;
	unordered_map<string, char*> E, R;
	// �����Ԫ�飬�Զ�ȥ��
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
	// �����Ԫ��
	Triple rndTriple(){
		return TV[rand() % TV.size()];
	}
	// ���ʵ��
	char* rndEntity(){
		return EV[rand() % EV.size()];
	}
	// �����ϵ
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

// ��ͼ�ף�����Tiiple�ļ��Ը�ͼ��Ϊ׼
typedef vector<Triple> SubKG;

#endif
