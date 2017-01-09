#!/usr/bin/python
# -*- coding: UTF-8 -*-
import subprocess
'''
KGRL
-eDim
        实体向量维数
-rDim
        关系向量维数
-margin
        目标函数margin
-batchSize
        SGD采样量
-errSize
        一个正确三元组采样几个错误三元组
-validSize
        链接预测的验证数量
-data 
        数据集名称，
        可选 FB_A|FB_B|WN_A|WN_B
-task 
        任务名称，
        可选 LP_Train|LP_Test|TC_Train|TC_Test
        LP表示链接预测，TC表示三元组分类，Train表示训练并验证，Test表示测试并输出
        必须同样的参数训练之后，才能进行测试
'''
def KGRL(data="FB_A",task="LP_Train",eDim=20,rDim=20,margin=0.5,batchSize=200,errSize=1,validSize=200):
    subprocess.call (["KGRL","-eDim",str(eDim),"-rDim",str(rDim),"-data",str(data),"-task",str(task),"-margin",str(margin),"-batchSize",str(batchSize),"-errSize",str(errSize),"-validSize",str(validSize)],shell=True)

KGRL()