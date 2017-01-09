# KGRL
֪ʶͼ��Ƕ���ܣ��ں�TransDʵ�֡�

������ [armadillo](http://arma.sourceforge.net/)

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