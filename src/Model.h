// Ƕ��ռ����ݽṹ��SGDѵ���㷨
#ifndef _MODEL_H_
#define _MODEL_H_

#include "stdafx.h"

/*
Ƕ��ռ����ݽṹ
map��key=ʵ��/��ϵ�� �� value=Ƕ������
*/
typedef unordered_map<idtype, mat> ES;

// ģ�Ͳ���
struct ModelParam{
	int eDim;// ʵ��������ά��
	int rDim;// ��ϵ������ά��
	double margin;// Ŀ�꺯���е� margin
	int batchSize;// SGD �� batchSize
	vector<double> stepSizes;// �����仯����
	int errSize;// һ����ȷ��Ԫ���������������Ԫ��
};

/*
����SGD����ѵ���Ļ���ƽ�Ƶ�֪ʶͼ��Ƕ�뷽���Ļ���
*/
class Model{
public:
	// ģ������
	KG kg;// ������֪ʶͼ��
	ES es;// Ƕ�������ռ�

	// ģ�Ͳ���
	int eDim;// ʵ��������ά��
	int rDim;// ��ϵ������ά��
	double margin;// Ŀ�꺯���е� margin
	int batchSize;// SGD �� batchSize
	vector<double> stepSizes;// �����仯����
	int errSize;// һ����ȷ��Ԫ���������������Ԫ��
	double thresh = 0;// ��Ԫ��������ֵ

	Model(ModelParam param){
		eDim = param.eDim;
		rDim = param.rDim;
		margin = param.margin;
		batchSize = param.batchSize;
		stepSizes = param.stepSizes;
		errSize = param.errSize;
	}

	// ��ʼ��
	virtual void init() = 0;

	// ���ֺ������ڸ���Ƕ��������ƫ��
	virtual ES gradient(Triple t) = 0;

	// ���ֺ���
	virtual double fscore(Triple t) = 0;

	// ����ѵ�����ϵ�Ŀ�꺯��
	double opt(){
		double s = 0;
		for (auto i = kg.T.begin(); i != kg.T.end(); i++){
			double f = fscore(i->first) - fscore(fTriple(i->first)) + margin;
			s += f > 0 ? f : 0;
		}
		return s / kg.T.size() / margin;
	}

	// ������ɴ�����Ԫ��
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
	void minBatchUpdate(double stepSize, SubKG::iterator& begin, SubKG::iterator& end){
		ES e;
		for (auto i = begin; i != end; i++){
			Triple t1 = *i;
			for (int j = 0; j < errSize; j++){
				Triple t2 = fTriple(t1);
				if (fscore(t1) - fscore(t2) + margin>0){
					ES e1 = gradient(t1);
					for (auto i = e1.begin(); i != e1.end(); i++){
						if (e.count(i->first) == 0){
							e[i->first] = i->second;
						}
						else{
							e[i->first] += i->second;
						}
					}
					ES e2 = gradient(t2);
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
			double nv = norm(v);
			if (nv > 1){
				v /= nv;
			}
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

	// ģ��ѵ��
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

	// ���� Hits@10
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

	// ���ڼ�������Ԥ��

	// ͷ��ʵ�����ֵ
	virtual mat h_hat(Triple& tri) = 0;
	// ��ϵ����ֵ
	virtual mat r_hat(Triple& tri) = 0;
	// β��ʵ�����ֵ
	virtual mat t_hat(Triple& tri) = 0;

	// Ԥ��ǰ n �����ܵ� head
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

	// ����ģ��
	void save(char* fname){
		FILE* fid = fopen(fname, "w");
		fprintf(fid, "%d %d %lf %d %d\n", eDim, rDim, margin, batchSize, errSize);
		fprintf(fid, "%d ", stepSizes.size());
		for (size_t k = 0; k < stepSizes.size(); k++){
			fprintf(fid, "%lf ", stepSizes[k]);
		}
		puts("");
		saveES(fid);
		fclose(fid);
	}

	// ����ģ��
	void load(char* fname){
		FILE* fid = fopen(fname, "r");
		fscanf(fid, "%d %d %lf %d %d\n", &eDim, &rDim, &margin, &batchSize, &errSize);
		int n, x;
		fscanf(fid, "%d", &n);
		for (int k = 0; k < n; k++){
			fscanf(fid, "%lf", &x);
			stepSizes.push_back(x);
		}
		loadES(fid);
		fclose(fid);
	}
private:
	static bool cmp(const pair<double, idtype>&a, const pair<double, idtype>& b){
		return a.first < b.first;
	}
	// ����ES
	void saveES(FILE* fid){
		for (auto i = es.begin(); i != es.end(); i++){
			mat& v = i->second;
			fprintf(fid, "%d %d %d\n", i->first, v.n_rows, v.n_cols);
			fprintMat(fid, v);
		}
	}
	// ��ȡES
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