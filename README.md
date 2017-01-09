# KGRL
知识图谱嵌入框架，内含TransD实现。
依赖库 [armadillo](http://arma.sourceforge.net/)

# 直接运行
运行环境： Windows 64位 （Windows 10 64位测试通过）
运行说明： 
+ 可运行文件在 app 目录下
+ 输出结果在 out 目录下
+ 训练结果在 model 目录下

因为训练结果以保存在 model 目录下，所以可以直接进行测试

app 目录下可运行脚本说明如下：

文件           | 功能
--------------|--------------------
FB_A_Train    | FB_A 训练
FB_A_Test     | FB_A 测试
FB_B_Train    | FB_B 训练
FB_B_Test     | FB_B 测试
WN_A_Train    | WN_A 训练
WN_A_Test     | WN_A 测试
WN_B_Train    | WN_B 训练
WN_B_Test     | WN_B 测试

# 编译运行
## Windows 64位操作系统
### 运行
可以直接运行 app 目录下已经编译好的结果。
+ `B_A_Train.bat` 表示 FB_A 的训练
+ `FB_A_Test.bat`  表示 FB_A 的测试
以此类推。

### 编译
在安装 Visual Studio 2013 的情况下打开 `KGRL.sln` 。
可能需要重新配置`armadillo`环境：
+ 设置附加包含目录为 `/armadillo/include`
+ 设置附加库目录为 `/armadillo/lib`
+ 设置附加依赖项为 `blas_win64_MT.lib` `lapack_win64_MT.lib`
+ 编译完成后手动复制 `blas_win64_MT.dll` `lapack_win64_MT.dll` 到生成目录下
+ 然后可以在 Visual Studio 2013 中运行

## Linux
安装 `armadillo` 
```
cd ./src_utf8
g++ -std=c++11 main.cpp -w -I /usr/include -larmadillo -o ../app/KGRL
```
然后在 `app` 目录下执行，bat文件也适用于linux的sh环境，例如
```
sh ./FB_A_Train.bat
```

## Mac
安装 HomeBrew 
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
安装 armadillo，会顺带安装 gcc
```
brew install homebrew/science/armadillo
```
编译
```
cd ./src_utf8
g++ -std=c++11 main.cpp -w -larmadillo -o ../app/KGRL
```
然后在 `app` 目录下执行，bat文件也适用于linux的sh环境，例如
```
sh ./FB_A_Train.bat
```