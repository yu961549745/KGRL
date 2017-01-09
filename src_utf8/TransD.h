// TransD 的实现
#ifndef _TRANSD_H_
#define _TRANSD_H_

#include"stdafx.h"

class TransD :public Model{
public:
	TransD(ModelParam param) :Model(param){
		I = eye(rDim, eDim);
	}

	// 初始化
	void init(){
		for (auto i = kg.E.begin(); i != kg.E.end(); i++){
			es[i->second] = randvec(eDim);
			es[pid(i->second)] = randvec(eDim);
		}
		for (auto i = kg.R.begin(); i != kg.R.end(); i++){
			es[i->second] = randvec(rDim);
			es[pid(i->second)] = randvec(rDim);
		}
	}

	// 评分函数
	double fscore(Triple tri){
		mat &h = es[tri.h], &hp = es[pid(tri.h)], &r = es[tri.r],
			&rp = es[pid(tri.r)], &t = es[tri.t], &tp = es[pid(tri.t)];
		// 这么写可以加速矩阵运算
		mat v = rp*(hp.t()*h - tp.t()*t) + I*(h - t) + r;
		return dot(v, v);
	}

	mat h_hat(Triple& tri) {
		mat &h = es[tri.h], &hp = es[pid(tri.h)], &rp = es[pid(tri.r)];
		return rp*(hp.t()*h) + I*h;
	}

	mat r_hat(Triple& tri) {
		return es[tri.r];
	}

	mat t_hat(Triple& tri) {
		mat &rp = es[pid(tri.r)], &t = es[tri.t], &tp = es[pid(tri.t)];
		return rp*(tp.t()*t) + I*t;
	}

	// 评分函数的梯度
	void gradient(Triple tri, ES& e){
		mat &h = es[tri.h], &hp = es[pid(tri.h)], &r = es[tri.r],
			&rp = es[pid(tri.r)], &t = es[tri.t], &tp = es[pid(tri.t)];
		mat Mrh = rp*hp.t() + I;
		mat Mrt = rp*tp.t() + I;
		mat f = Mrh*h + r - Mrt*t;

		e[tri.h] = 2 * Mrh.t()*f;
		e[tri.t] = -2 * Mrt.t()*f;
		e[tri.r] = 2 * f;
		e[pid(tri.r)] = 2 * f*(hp.t()*h - tp.t()*t);
		e[pid(tri.h)] = 2 * h*(rp.t()*f);
		e[pid(tri.t)] = -2 * t*(rp.t()*f);
	}

private:
	mat I;
	mat randvec(int n){
		mat v = 2 * mat(n, 1, fill::randu) - 1;
		nomalize(v);
		return v;
	}
	// 获取投影向量的id
	idtype pid(idtype id){
		if (id > 0) return id + (idtype)kg.EV.size();
		else        return id - (idtype)kg.RV.size();
	}
};

#endif
