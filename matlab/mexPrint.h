#ifndef _PRINT_H_
#define _PRINT_H_

#include<cstdio>
#include<cstdarg>
#include<mex.h>
void myPrintf(const char* fmt, ...){
	va_list args;
	char buf[2048];
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	mxArray* prhs[1];
	prhs[0] = mxCreateString(buf);
	mexCallMATLAB(0, NULL, 1, prhs, "append");
}
#ifdef printf
#undef printf
#endif
#define printf myPrintf
#define puts(x) myPrintf("%s\n",x)

#endif