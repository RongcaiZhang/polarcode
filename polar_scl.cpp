/*********************************************************************************
*�Ŷӣ�5G-037
*���ߣ�xsfan
*˵����SCL�㷨��Ҫ����
*********************************************************************************/

/************************************ͷ�ļ�**************************************/
#include "polar_scl.h"

/***********************************��������*************************************/
int searchList;											//�������
bool out[SEARCH_L_MAX][LENGTH];							//������������
float pmValue[SEARCH_L_MAX<<1];							//·������ֵ
float pmStore[SEARCH_L_MAX<<1];							//����ֵ����
float LLRMap[SEARCH_L_MAX][LENGTH];						//��Ȼֵ��Ϣ��

int pathNum;											//��ǰ·���������
int emptyStack[SEARCH_L_MAX>>1];						//��������չ·�����
int fullStack[SEARCH_L_MAX>>1];							//����˫��չ·�����
int sortInfo[SEARCH_L_MAX<<1];							//������

/*********************************************************************************
*���ܣ����������Ȼֵ
*������LBase	L���±�			LTop		L���ϱ�	
*	   uSub		u����			bitInfo		�����к�
*	   order	·�����
*����ֵ��logValue
*********************************************************************************/
float LLR(int LBase,int LTop,bool *uSub,int bitInfo,int order)
{
	int i;
	bool subXor[LENGTH>>1];			//������1��������
	bool subEven[LENGTH>>1];		//������2��ȡż����

	float likehood_1,likehood_2;    //�ݹ���Ȼֵ
	float logValue;                
	int changeNum;					//likehood_1��likehood_2�Ƿ��轻��
	int changeValue;				//likehood_1��likehood_2�����ٽ�ֵ
	int index;						//�±�

	changeValue = LBase>>1;
	changeNum = bitInfo % LBase;
	index = codeLength/changeValue - 2 + bitInfo/changeValue;	//���λ����Ϣ���㣬����ַ��ƫ����
	
	if(LBase == 2)
	{
		if(LTop % 2 == 1)						//�ݹ�����һ�������F���㣬�����ŵ���������
		{
			likehood_1 = llr[bitInfo];
			likehood_2 = llr[bitInfo+1];
		}
		else									//�ݹ�����һ�������G���㣬�����ŵ���������
		{
			likehood_1 = llr[bitInfo-1];
			likehood_2 = llr[bitInfo];
		}
	}
	else
	{
		if(LTop%2 == 0)							//������G���㣬������
		{
			likehood_1 = LLRMap[order][index - 1];	
			likehood_2 = LLRMap[order][index];
		}
		else									//F���㣬�ݹ���㣬LTop-1 Ϊ uSub ���г���
		{
			if(LTop != 1)
			{
				for(i=0;i<(LTop-1)>>1;i++)
				{
					subXor[i] = uSub[i<<1] ^ uSub[(i<<1)+1];
					subEven[i] = uSub[(i<<1)+1];
				}
			}

			likehood_1 = LLR(LBase>>1,(LTop+1)>>1,subXor,bitInfo,order);				//�ݹ�
			LLRMap[order][index] = likehood_1;											//���棬�Թ�����
			likehood_2 = LLR(LBase>>1,(LTop+1)>>1,subEven,bitInfo + changeValue,order);
			LLRMap[order][index + 1] = likehood_2;
		}
	}

	/*����ʱ������F����*/
	if(LTop % 2 == 1)
	{
		logValue = F(likehood_1,likehood_2);
	}
	/*ż��ʱ������G����*/
	else
	{
		logValue = G(likehood_1,likehood_2,uSub[(LTop-1)-1]);
	}

	return logValue;
}

/*********************************************************************************
*���ܣ������ i ������LLR������ǰ i-1 ���ڵ�ȫΪ0,��ͬ��SC���룬�ɲ����㣬��128
*	   ���ڵ����ȫ�ݹ���㣬�ɼ����������
*������i:����
*����ֵ���� i �����ֵ
*********************************************************************************/
float LLR_Pre(int LBase,int LTop,bool *uSub,int bitInfo)
{
	int i;
	bool subXor[LENGTH>>1];			//������1��������
	bool subEven[LENGTH>>1];		//������2��ȡż����

	float likehood_1,likehood_2;    //�ݹ���Ȼֵ
	float logValue;                
	int changeNum;					//likehood_1��likehood_2�Ƿ��轻��
	int changeValue;				//likehood_1��likehood_2�����ٽ�ֵ
	int index;						//�±�

	changeValue = LBase>>1;
	changeNum = bitInfo % LBase;
	index = codeLength/changeValue - 2 + bitInfo/changeValue;

	if(LBase == 2)
	{
		if(LTop % 2 == 1)
		{
			likehood_1 = llr[bitInfo];
			likehood_2 = llr[bitInfo+1];
		}
		else
		{
			likehood_1 = llr[bitInfo-1];
			likehood_2 = llr[bitInfo];
		}
	}
	else
	{
		if(LTop != 1)
		{
			for(i=0;i<(LTop-1)>>1;i++)
			{
				subXor[i] = uSub[i<<1] ^ uSub[(i<<1)+1];
				subEven[i] = uSub[(i<<1)+1];
			}
		}
	
		if(changeNum < changeValue)			//F����
		{
			likehood_1 = LLR_Pre(LBase>>1,(LTop+1)>>1,subXor,bitInfo);
			LLRMap[0][index] = likehood_1;
			likehood_2 = LLR_Pre(LBase>>1,(LTop+1)>>1,subEven,bitInfo + changeValue);
			LLRMap[0][index + 1] = likehood_2;
		}

		else								//G����
		{
			likehood_1 = LLR_Pre(LBase>>1,LTop>>1,subXor,bitInfo - changeValue);
			LLRMap[0][index - 1] = likehood_1;
			likehood_2 = LLR_Pre(LBase>>1,LTop>>1,subEven,bitInfo);
			LLRMap[0][index] = likehood_2;										
		}		
	}

	/*����ʱ������f����*/
	if(LTop % 2 == 1)
	{
		logValue = F(likehood_1,likehood_2);
	}
	/*ż��ʱ������g����*/
	else
	{
		logValue = G(likehood_1,likehood_2,uSub[(LTop-1)-1]);
	}
	
	return logValue;
}

/*********************************************************************************
*���ܣ����㲻ͬ·���Ķ���ֵ
*������i:����  order·�����
*����ֵ��
*********************************************************************************/
void PM_scl(int i,int order)
{	
	float logResult;

	logResult = LLR(codeLength,i,out[order],invInfo[i-1],order);  //���㵱ǰ���ض�����Ȼֵ

	/*��ѯλ����Ϣ����Ϊ�̶����أ���ֱ�������Һ���*/
	if(location[i-1] == 0)
	{	
		/*���� 1-2u �������Ȼֵ�ķ���ѡ����Ӧ���ʽ*/
		if(logResult < 0)
		{
			pmValue[order] = pmValue[order] + logResult;
		}
		out[order][i-1] = 0;
	}
	/*�ֱ�������Һ��Ӷ���ֵ������ 1-2u �������Ȼֵ�ķ���ѡ����Ӧ���ʽ*/
	else 
	{
		if(logResult >= 0)
		{
			pmValue[(order<<1)+1] = pmValue[order] - logResult;
			pmValue[order<<1] = pmValue[order];
		}
		else
		{
			pmValue[(order<<1)+1] = pmValue[order];
			pmValue[order<<1] = pmValue[order] + logResult;
		}
	}
}

/*********************************************************************************
*���ܣ������ i �����Ķ���ֵ������ǰ i-1 ���ڵ�ȫΪ0,��ͬ��SC���룬�ɲ����㣬ֱ
*	   �Ӽ���� i ���ڵ�
*������i:����
*����ֵ���� i �����ֵ
*********************************************************************************/
void PMPrepare(int i)
{	
	float logResult;

	logResult = LLR_Pre(codeLength,i,out[0],invInfo[i-1]);  //��ǰ���ض�����Ȼֵ
	if (searchList == 1)									//�������Ϊ1��SC����
	{
		/*���� logResult ����ѡ��������*/
		decode[i - 1] = logResult >= 0 ? 0 : 1;				//ֱ�Ӿ���������
		out[0][i - 1] = decode[i - 1];
	}
	else
	{
		/*�ֱ�������Һ��Ӷ���ֵ*/
		if (logResult >= 0)
		{
			pmValue[1] = pmValue[0] - logResult;
		}
		else
		{
			pmValue[1] = pmValue[0];
			pmValue[0] = pmValue[0] + logResult;
		}
	}	
}

/*********************************************************************************
*���ܣ�����׼�������������һ����Ϣλ
*������i:����
*����ֵ��
*********************************************************************************/
void Prepare(int i)
{
	PMPrepare(i);
	if (searchList != 1)
	{
		out[0][i - 1] = 0;
		out[1][i - 1] = 1;
		memcpy(LLRMap[1], LLRMap[0], sizeof(LLRMap[0]));	//������Ȼֵ��Ϣ��
		pathNum = 2;										//·������չΪ2
	}
}

/*********************************************************************************
*���ܣ�δ�ﵽ�������ʱ��·����չ
*������i:����
*����ֵ����
*********************************************************************************/
void ExtendClone(int i)
{
	int j;
	for(j=pathNum-1;j>=0;j--)
	{
		memcpy(out[(j<<1)+1],out[j],sizeof(out[0]));			//����·��
		memcpy(out[j<<1],out[j],sizeof(out[0]));
		out[(j<<1)+1][i] = 1;
		out[j<<1][i] = 0;

		memcpy(LLRMap[(j<<1)+1],LLRMap[j],sizeof(LLRMap[0]));	//������Ȼֵ�б�
		memcpy(LLRMap[j<<1],LLRMap[j],sizeof(LLRMap[0]));
	}
}

/*********************************************************************************
*���ܣ�L��·������
*������
*����ֵ��
*********************************************************************************/
void scl()
{
	int i;
	int j;
	int m;
	int emptyIndex;									//����չ���˳���
	int fullIndex;									//˫��չ���˳���
	int extendPath[SEARCH_L_MAX] = {0};				//��չ��ʽ�ж�

	pathNum = 1;									//��ǰ�������

	if (codeLength == 1024)
	{
		Prepare(128);								//�����128λ
		i = 128;
	}
	else
	{
		Prepare(1);								//�����60λ
		i = 1;
	}

	for(;i<codeLength;i++)
	{
		/*��ǰ������·����pmֵ*/
		for(j=pathNum-1;j>=0;j--)
		{
			PM_scl(i+1,j);
		}
		
		if(location[i] == 1)
		{
			if(pathNum < searchList)				//δ�ﵽ�������ʱ��������Ϣλ����չ
			{
				ExtendClone(i);						//·������
			}
			pathNum = pathNum<<1;
		}
		if(pathNum > searchList)					//�ﵽ�������ʱ��������Ϣλ��ü�
		{
			pathNum = searchList;					//�ü���Ŀ��

			memcpy(pmStore,pmValue,sizeof(pmValue));

			Sort(pmValue,(searchList<<1));			//����ǰL��·����ţ�0��L-1��

			for(m=0;m<searchList;m++)
			{
				extendPath[sortInfo[m]>>1]++;		//�����ڵ���չ��ͳ��
			}
			emptyIndex = 0;
			fullIndex = 0;
			
			for(m=0;m<searchList;m++)
			{
				if(extendPath[m] == 0)				//δ��չ����ɾ��
				{
					emptyStack[emptyIndex++] = m;	//���游�ڵ���
				}
				else if(extendPath[m] == 2)			//��չ
				{
					fullStack[fullIndex++] = m;
				}
				else								//������չ������pmֵ��uֵ
				{
					pmValue[m] = pmStore[m<<1] >= pmStore[(m<<1)+1]? pmStore[m<<1]:pmStore[(m<<1)+1];
					out[m][i] = pmStore[m<<1] >= pmStore[(m<<1)+1]? 0:1;
				}
			}

			for(j=emptyIndex-1;j>=0;j--)
			{				
				/*��Ч·���滻Ϊ˫·����1*/
				memcpy(out[emptyStack[j]],out[fullStack[j]],sizeof(out[0]));
				out[emptyStack[j]][i] = 1;
				memcpy(LLRMap[emptyStack[j]],LLRMap[fullStack[j]],sizeof(LLRMap[0]));
				pmValue[emptyStack[j]] = pmStore[(fullStack[j]<<1)+1];
				/*˫·���е�0��������*/
				out[fullStack[j]][i] = 0;
				pmValue[fullStack[j]] = pmStore[(fullStack[j]<<1)];
			}
		}
		memset(extendPath,0,sizeof(extendPath));		
	}
	Sort(pmValue, searchList);
	memset(pmValue,0,sizeof(pmValue));
}

/*********************************************************************************
*���ܣ�SCL�㷨
*������
*����ֵ��������
*********************************************************************************/
void Polar_scl()
{
	scl();
	memcpy(decode,out[sortInfo[0]],sizeof(out[0]));	//PMֵ���ĺ�ѡ·��������������
}
