#ifndef  _INCLUDES_H_
#define  _INCLUDES_H_
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>	

#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint  unsigned int  
#endif

#ifndef		LENGTH
#define		LENGTH			1024							//最大码长
#endif

#define		SEARCH_L_MAX	32								//搜索宽度
#define		START_FRAME		0								//起始帧
#define		STOP_FRAME		1000						//结束帧
#define		TOTAL_FRAME		(STOP_FRAME - START_FRAME)		//总译码帧数

/*common*/
extern int codeLength;										//码长
extern int searchList;										//搜索宽度
extern float llr[LENGTH];									//信道接收值
extern bool location[LENGTH];								//位置信息
extern int invInfo[LENGTH];									//反序信息
extern bool send[LENGTH];									//信道发送值
extern bool decode[LENGTH];									//译码输出序列

/*Base on SCL*/		
extern int sortInfo[2*SEARCH_L_MAX];						//排序结果
extern int acaLength;										//aCA最大搜索宽度
#endif
