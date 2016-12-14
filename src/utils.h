// 工具方法
#ifndef _UTILS_H_
#define _UTILS_H_

#include<windows.h>
#include<psapi.h>
#include<time.h>
#include<stack>

// 显示当前内存
void showMemory(){
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	printf("memory = %.3f MB\n", 1.0*pmc.WorkingSetSize / 1024 / 1024);
}

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