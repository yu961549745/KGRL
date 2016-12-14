// 工具方法
#ifndef _UTILS_H_
#define _UTILS_H_

#include<time.h>
#include<stack>

stack<clock_t> TimeStack;

void tic(char* s){
	puts(s);
	TimeStack.push(clock());
}

void toc(){
	printf("time = %.3f s\n\n", (clock() - TimeStack.top()) / 1000.0);
	TimeStack.pop();
}

#endif