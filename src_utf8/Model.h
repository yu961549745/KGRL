// 嵌入空间数据结构及SGD训练算法
#ifndef _MODEL_H_
#define _MODEL_H_

#include "stdafx.h"

/*
嵌入空间数据结构
map：key=实体/关系名 ， value=嵌入向量
*/
typedef unordered_map<idtype, mat> ES;

// 模型参数
struct ModelParam{
	int eDim;// 实体向量的维数
	int rDim;// 关系向量的维数
	double margin;// 目标函数中的 margin
	int batchSize;// SGD 的 batchSize
	vector<double> stepSizes;// 步长变化向量
	int errSize;// 一个正确三元组采样几个错误三元组
};

/*
采用SGD进行训练的基于平移的知识图谱嵌入方法的基类
*/
class Model{
public:
	// 模型数据
	KG kg;// 关联的知识图谱
	ES es;// 嵌入向量空间

	// 模型参数
	int eDim;// 实体向量的维数
	int rDim;// 关系向量的维数
	double margin;// 目标函数中的 margin
	int batchSize;// SGD 的 batchSize
	vector<double> stepSizes;// 步长变化向量
	int errSize;// 一个正确三元组采样几个错误三元组
	double thresh = 0;// 三元组分类的阈值

	Model(ModelParam param){
		eDim = param.eDim;
		rDim = param.rDim;
		margin = param.margin;
		batchSize = param.batchSize;
		stepSizes = param.stepSizes;
		errSize = param.errSize;
	}

	// 初始化
	virtual void init() = 0;

	// 评分函数关于各个嵌入向量的偏导
	virtual void gradient(Triple t, ES& out) = 0;

	// 评分函数
	virtual double fscore(Triple t) = 0;

	// 约束嵌入向量的模
	virtual void nomalize(mat& v){
		double nv = norm(v);
		if (nv > 1){
			v /= nv;
		}
	}

	// 计算训练集上的目标函数
	double opt(){
		double s = 0;
		for (auto i = kg.T.begin(); i != kg.T.end(); i++){
			double f = fscore(i->first) - fscore(fTriple(i->first)) + margin;
			s += f > 0 ? f : 0;
		}
		return s / kg.T.size() / margin;
	}

	// 随机生成错误三元组
	Triple fTriple(Triple t1){
		Triple t2 = t1;
		if (rand() % 2 == 0){
			do{
				t2.h = kg.rndEntity();
			} while (kg.T.count(t2) != 0);
		}
		else{
			do{
				t2.t = kg.rndEntity();
			} while (kg.T.count(t2) != 0);
		}
		return t2;
	}

	// min-batch update
	void minBatchUpdate(double stepSize,const SubKG::iterator& begin,const SubKG::iterator& end){
		ES e;
		for (auto i = begin; i != end; i++){
			Triple t1 = *i;
			for (int j = 0; j < errSize; j++){
				Triple t2 = fTriple(t1);
				if (fscore(t1) - fscore(t2) + margin>0){
					ES e1; gradient(t1, e1);
					for (auto i = e1.begin(); i != e1.end(); i++){
						if (e.count(i->first) == 0){
							e[i->first] = i->second;
						}
						else{
							e[i->first] += i->second;
						}
					}
					ES e2; gradient(t2, e2);
					for (auto i = e2.begin(); i != e2.end(); i++){
						if (e.count(i->first) == 0){
							e[i->first] = i->second;
						}
						else{
							e[i->first] -= i->second;
						}
					}
				}
			}
		}
		int numBatch = (int)(end - begin);
		for (auto i = e.begin(); i != e.end(); i++){
			mat& v = es[i->first];
			mat& g = i->second;
			v -= stepSize*g / numBatch / errSize;
			nomalize(v);
		}
	}

	// update epoch
	void epochUpdate(double stepSize){
		random_shuffle(kg.TV.begin(), kg.TV.end());
		SubKG::iterator begin = kg.TV.begin(), end = begin + batchSize;
		while (end < kg.TV.end()){
			minBatchUpdate(stepSize, begin, end);
			begin += batchSize; 
			end += batchSize;
		}
		if (begin < kg.TV.end()){
			minBatchUpdate(stepSize, begin, kg.TV.end());
		}
	}

	// 模型训练
	void train(){
		int i = 0;
		double pf = opt(), cf;
		printf("t=%03d , f=%g\n", i, pf);
		i++;
		int pStepSize = 0;
		double stepSize = stepSizes[pStepSize++];
		while (true){
			epochUpdate(stepSize);
			cf = opt();
			printf("t=%03d , f=%g\n", i, cf);
			if (cf >= pf){
				stepSize = stepSizes[pStepSize++];
			}
			if (pStepSize >= stepSizes.size()){
				break;
			}
			pf = cf;
			i++;
		}
	}

	// 计算 Hits@10
	double predictHead(SubKG& data, int n){
		int hits = 0;
		size_t N = data.size();
		for (int i = 0; i < N;i++){
			Triple t = data[i];
			vector<idtype> out;
			predictHeads(out, t, n);
			for (int k = 0; k < n; k++){
				t.h = out[k];
				if (kg.T.count(t)) {
					hits++;
					break;
				}
			}
			if ((i + 1) % 50 == 0){
				printf("%d / %d -> %g\r", i + 1, N, 1.0*hits / (i + 1));
			}
		}
		printf("%30s\r", " ");
		return 1.0*hits / N;
	}

	// 用于加速链接预测

	// 头部实体估计值
	virtual mat h_hat(Triple& tri) = 0;
	// 关系估计值
	virtual mat r_hat(Triple& tri) = 0;
	// 尾部实体估计值
	virtual mat t_hat(Triple& tri) = 0;

	// 预测前 n 个可能的 head
	void predictHeads(vector<idtype>& out , Triple tri, int n){
		vector<pair<double, idtype>> fs;
		mat h_ = t_hat(tri) - r_hat(tri);
		for (auto j = kg.E.begin(); j != kg.E.end(); j++){
			tri.h = j->second;
			mat d = h_hat(tri) - h_;
			fs.push_back(make_pair(dot(d, d), j->second));
		}
		sort(fs.begin(), fs.end(), cmp);
		for (int k = 0; k < n; k++){
			out.push_back(fs[k].second);
		}
	}

	// 保存模型
	void save(char* fname){
		FILE* fid = fopen(fname, "w");
		saveES(fid);
		fclose(fid);
	}

	// 加载模型
	void load(char* fname){
		FILE* fid = fopen(fname, "r");
		loadES(fid);
		fclose(fid);
	}
private:
	static bool cmp(const pair<double, idtype>&a, const pair<double, idtype>& b){
		return a.first < b.first;
	}
	// 保存ES
	void saveES(FILE* fid){
		for (auto i = es.begin(); i != es.end(); i++){
			mat& v = i->second;
			fprintf(fid, "%d %d %d\n", i->first, v.n_rows, v.n_cols);
			fprintMat(fid, v);
		}
	}
	// 读取ES
	void loadES(FILE* fid){
		int id, m, n;
		while (fscanf(fid, "%d %d %d", &id, &m, &n) != EOF){
			es[id] = mat(m, n);
			fscanMat(fid, es[id]);
		}
		fclose(fid);
	}
};

#endif
