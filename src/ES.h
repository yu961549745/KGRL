// Ƕ��ռ����ݽṹ��SGDѵ���㷨
#ifndef _ES_H_
#define _ES_H_

#include "stdafx.h"

/*
Ƕ��ռ����ݽṹ
map��key=ʵ��/��ϵ�� �� value=Ƕ������
*/
typedef unordered_map<idtype, mat> ES;

struct ModelParam{
	int eDim;// ʵ��������ά��
	int rDim;// ��ϵ������ά��
	double margin;// Ŀ�꺯���е� margin
	int batchSize;// SGD �� batchSize
	int trainPeriod;// ����Ŀ�꺯��ֵ������
	vector<double> stepSizes;// �����仯����
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
	int trainPeriod;// ����Ŀ�꺯��ֵ������
	vector<double> stepSizes;// �����仯����

	Model(ModelParam param){
		eDim = param.eDim;
		rDim = param.rDim;
		margin = param.margin;
		batchSize = param.batchSize;
		trainPeriod = param.trainPeriod;
		stepSizes = param.stepSizes;
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

	// SGD��һ�θ���
	void update(double stepSize){
		ES e;
		for (int k = 0; k < batchSize; k++){
			Triple t1 = kg.rndTriple();
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
		for (auto i = e.begin(); i != e.end(); i++){
			mat& v = es[i->first];
			v -= stepSize*(i->second);
			double nv = norm(v);
			if (nv > 1){
				v /= nv;
			}
		}
	}

	// ģ��ѵ��
	void train(){
		int i = 0;
		double pf = opt(), cf;
		printf("t=%08d , f=%g\n", i*trainPeriod, pf);
		i++;
		int pStepSize = 0;
		double stepSize = stepSizes[pStepSize++];
		while (true){
			for (int j = 0; j < trainPeriod; j++){
				update(stepSize);
			}
			cf = opt();
			printf("t=%08d , f=%g\n", i*trainPeriod, cf);
			if (cf >= pf){
				stepSize = stepSizes[pStepSize++];
			}
			if (pStepSize>=stepSizes.size()){
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

	// Ԥ��ǰ n �����ܵ� head
	void predictHeads(vector<idtype>& out , Triple tri, int n){
		vector<pair<double, idtype>> fs;
		for (auto j = kg.E.begin(); j != kg.E.end(); j++){
			Triple tt = tri;
			tt.h = j->second;
			fs.push_back(make_pair(fscore(tt), tt.h));
		}
		sort(fs.begin(), fs.end(), cmp);
		for (int k = 0; k < n; k++){
			out.push_back(fs[k].second);
		}
	}

	// ����ES
	void saveES(char* fname){
		FILE* fid = fopen(fname, "w");
		for (auto i = es.begin(); i != es.end(); i++){
			mat& v = i->second;
			fprintf(fid, "%d %d %d\n", i->first, v.n_rows, v.n_cols);
			fprintMat(fid, v);
		}
		fclose(fid);
	}
	// ��ȡES
	void loadES(char* fname){
		FILE* fid = fopen(fname, "r");
		int id, m, n;
		while (fscanf(fid, "%d %d %d", &id, &m, &n) != EOF){
			es[id] = mat(m, n);
			fscanMat(fid, es[id]);
		}
		fclose(fid);
	}

private:
	static bool cmp(const pair<double, idtype>&a, const pair<double, idtype>& b){
		return a.first < b.first;
	}
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
	void fscanMat(FILE*fid, mat& v){
		int m = v.n_rows;
		int n = v.n_cols;
		for (int i = 0; i < m; i++){
			for (int j = 0; j < n; j++){
				fscanf(fid, "%lf", &v(i, j));
			}
		}
	}
};

#endif