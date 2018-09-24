#ifndef  _POLAR_SCL_H_
#define  _POLAR_SCL_H_
#include  "includes.h"
#include  "common_cmd.h"

extern bool out[SEARCH_L_MAX][LENGTH];					
extern int pathNum;										
extern int emptyStack[SEARCH_L_MAX/2];							
extern int fullStack[SEARCH_L_MAX/2];							

float LLR(int LBase,int LTop,bool *uSub,int bitInfo,int order);
void PMPrepare(int i);
void scl();
void Polar_scl();

#endif
