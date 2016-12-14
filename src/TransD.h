// TransD 的实现
#ifndef _TRANSD_H_
#define _TRANSD_H_

#include"stdafx.h"

class TransD :public Model{
public:
	TransD(int _eDim, int _rDim, double _margin, int _batchSize, int _trainPeriod, vector<double> _stepSizes)
		:Model(_eDim, _rDim, _margin, _batchSize, _trainPeriod, _stepSizes){
		I = eye(rDim, eDim);
	}

	// 初始化
	void init(){
		for (auto i = kg.E.begin(); i != kg.E.end(); i++){
			es[i->second] = randvec(eDim);
			es[i->second + 1] = randvec(eDim);
		}
		for (auto i = kg.R.begin(); i != kg.R.end(); i++){
			es[i->second] = randvec(rDim);
			es[i->second + 1] = randvec(rDim);
		}
	}

	// 评分函数
	double fscore(Triple tri){
		mat &h = es[tri.h], &hp = es[tri.h + 1], &r = es[tri.r],
			&rp = es[tri.r + 1], &t = es[tri.t], &tp = es[tri.t + 1];
		mat Mrh = rp*hp.t() + I;
		mat Mrt = rp*tp.t() + I;
		mat v = Mrh*h + r - Mrt*t;
		return dot(v, v);
	}

	// 评分函数的梯度
	ES gradient(Triple tri){
		mat &h = es[tri.h], &hp = es[tri.h + 1], &r = es[tri.r],
			&rp = es[tri.r + 1], &t = es[tri.t], &tp = es[tri.t + 1];
		mat Mrh = rp*hp.t() + I;
		mat Mrt = rp*tp.t() + I;
		mat f = Mrh*h + r - Mrt*t;

		ES e;
		e[tri.h] = 2 * Mrh.t()*f;
		e[tri.t] = -2 * Mrt.t()*f;
		e[tri.r] = 2 * f;
		e[tri.r + 1] = 2 * f*(hp.t()*h - tp.t()*t);
		e[tri.h + 1] = 2 * h*(rp.t()*f);
		e[tri.t + 1] = -2 * t*(rp.t()*f);
		return e;
	}

private:
	mat I;
	mat randvec(int n){
		mat v = 2 * mat(n, 1, fill::randu) - 1;
		double nv = norm(v);
		if (nv > 1)v /= nv;
		return v;
	}
};

#endif