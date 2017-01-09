# KGRL
֪ʶͼ��Ƕ���ܣ��ں�TransDʵ�֡�
������ [armadillo](http://arma.sourceforge.net/)

# ֱ������
���л����� Windows 64λ ��Windows 10 64λ����ͨ����
����˵���� 
+ �������ļ��� app Ŀ¼��
+ �������� out Ŀ¼��
+ ѵ������� model Ŀ¼��

��Ϊѵ������Ա����� model Ŀ¼�£����Կ���ֱ�ӽ��в���

app Ŀ¼�¿����нű�˵�����£�

�ļ�           | ����
--------------|--------------------
FB_A_Train    | FB_A ѵ��
FB_A_Test     | FB_A ����
FB_B_Train    | FB_B ѵ��
FB_B_Test     | FB_B ����
WN_A_Train    | WN_A ѵ��
WN_A_Test     | WN_A ����
WN_B_Train    | WN_B ѵ��
WN_B_Test     | WN_B ����

# ��������
## Windows 64λ����ϵͳ
### ����
����ֱ������ app Ŀ¼���Ѿ�����õĽ����
+ `B_A_Train.bat` ��ʾ FB_A ��ѵ��
+ `FB_A_Test.bat`  ��ʾ FB_A �Ĳ���
�Դ����ơ�

### ����
�ڰ�װ Visual Studio 2013 ������´� `KGRL.sln` ��
������Ҫ��������`armadillo`������
+ ���ø��Ӱ���Ŀ¼Ϊ `/armadillo/include`
+ ���ø��ӿ�Ŀ¼Ϊ `/armadillo/lib`
+ ���ø���������Ϊ `blas_win64_MT.lib` `lapack_win64_MT.lib`
+ ������ɺ��ֶ����� `blas_win64_MT.dll` `lapack_win64_MT.dll` ������Ŀ¼��
+ Ȼ������� Visual Studio 2013 ������

## Linux
��װ `armadillo` 
```
cd ./src_utf8
g++ -std=c++11 main.cpp -w -I /usr/include -larmadillo -o ../app/KGRL
```
Ȼ���� `app` Ŀ¼��ִ�У�bat�ļ�Ҳ������linux��sh����������
```
sh ./FB_A_Train.bat
```

## Mac
��װ HomeBrew 
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
��װ armadillo����˳����װ gcc
```
brew install homebrew/science/armadillo
```
����
```
cd ./src_utf8
g++ -std=c++11 main.cpp -w -larmadillo -o ../app/KGRL
```
Ȼ���� `app` Ŀ¼��ִ�У�bat�ļ�Ҳ������linux��sh����������
```
sh ./FB_A_Train.bat
```