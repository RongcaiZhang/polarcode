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
#define		LENGTH			1024							//����볤
#endif

#define		SEARCH_L_MAX	32								//�������
#define		START_FRAME		0								//��ʼ֡
#define		STOP_FRAME		1000						//����֡
#define		TOTAL_FRAME		(STOP_FRAME - START_FRAME)		//������֡��

/*common*/
extern int codeLength;										//�볤
extern int searchList;										//�������
extern float llr[LENGTH];									//�ŵ�����ֵ
extern bool location[LENGTH];								//λ����Ϣ
extern int invInfo[LENGTH];									//������Ϣ
extern bool send[LENGTH];									//�ŵ�����ֵ
extern bool decode[LENGTH];									//�����������

/*Base on SCL*/		
extern int sortInfo[2*SEARCH_L_MAX];						//������
extern int acaLength;										//aCA����������
#endif
