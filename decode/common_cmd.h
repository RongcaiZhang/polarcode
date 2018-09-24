#ifndef _COMMON_CMD_H_
#define _COMMON_CMD_H_

#include "includes.h"

void Exit();
int CodeComp(bool *code_orig,bool *code_out);
int Sign(float a,float b);
float AbsMin(float a,float b);
float F(float likehood_1,float likehood_2);
float G(float likehood_1,float likehood_2,bool u);
void Sort(float *a,int sortNum);
void ExtractInfo(bool *info);
int CrcCheck();
int searchLengthCheck(int i);

#endif
