/*********************************************************************************
*团队：5G-037
*作者：xsfan
*说明：SCL算法主要程序
*********************************************************************************/

/************************************头文件**************************************/
#include "polar_scl.h"

/***********************************变量定义*************************************/
int searchList;											//搜索宽度
bool out[SEARCH_L_MAX][LENGTH];							//可能译码序列
float pmValue[SEARCH_L_MAX<<1];							//路径度量值
float pmStore[SEARCH_L_MAX<<1];							//度量值备份
float LLRMap[SEARCH_L_MAX][LENGTH];						//似然值信息表

int pathNum;											//当前路径搜索宽度
int emptyStack[SEARCH_L_MAX>>1];						//保存零扩展路径标号
int fullStack[SEARCH_L_MAX>>1];							//保存双扩展路径标号
int sortInfo[SEARCH_L_MAX<<1];							//排序结果

/*********************************************************************************
*功能：计算对数似然值
*参数：LBase	L的下标			LTop		L的上标	
*	   uSub		u序列			bitInfo		表中行号
*	   order	路径编号
*返回值：logValue
*********************************************************************************/
float LLR(int LBase,int LTop,bool *uSub,int bitInfo,int order)
{
	int i;
	bool subXor[LENGTH>>1];			//子序列1，异或产生
	bool subEven[LENGTH>>1];		//子序列2，取偶产生

	float likehood_1,likehood_2;    //递归似然值
	float logValue;                
	int changeNum;					//likehood_1和likehood_2是否需交换
	int changeValue;				//likehood_1和likehood_2交换临界值
	int index;						//下标

	changeValue = LBase>>1;
	changeNum = bitInfo % LBase;
	index = codeLength/changeValue - 2 + bitInfo/changeValue;	//查表位置信息计算，基地址加偏移量
	
	if(LBase == 2)
	{
		if(LTop % 2 == 1)						//递归至第一层结束，F运算，调用信道接收序列
		{
			likehood_1 = llr[bitInfo];
			likehood_2 = llr[bitInfo+1];
		}
		else									//递归至第一层结束，G运算，调用信道接收序列
		{
			likehood_1 = llr[bitInfo-1];
			likehood_2 = llr[bitInfo];
		}
	}
	else
	{
		if(LTop%2 == 0)							//其它层G运算，查表计算
		{
			likehood_1 = LLRMap[order][index - 1];	
			likehood_2 = LLRMap[order][index];
		}
		else									//F运算，递归计算，LTop-1 为 uSub 序列长度
		{
			if(LTop != 1)
			{
				for(i=0;i<(LTop-1)>>1;i++)
				{
					subXor[i] = uSub[i<<1] ^ uSub[(i<<1)+1];
					subEven[i] = uSub[(i<<1)+1];
				}
			}

			likehood_1 = LLR(LBase>>1,(LTop+1)>>1,subXor,bitInfo,order);				//递归
			LLRMap[order][index] = likehood_1;											//保存，以供调用
			likehood_2 = LLR(LBase>>1,(LTop+1)>>1,subEven,bitInfo + changeValue,order);
			LLRMap[order][index + 1] = likehood_2;
		}
	}

	/*奇数时，进行F运算*/
	if(LTop % 2 == 1)
	{
		logValue = F(likehood_1,likehood_2);
	}
	/*偶数时，进行G运算*/
	else
	{
		logValue = G(likehood_1,likehood_2,uSub[(LTop-1)-1]);
	}

	return logValue;
}

/*********************************************************************************
*功能：计算第 i 个结点的LLR，由于前 i-1 个节点全为0,等同于SC译码，可不计算，第128
*	   个节点采用全递归计算，可减少运算次数
*参数：i:层数
*返回值：第 i 层度量值
*********************************************************************************/
float LLR_Pre(int LBase,int LTop,bool *uSub,int bitInfo)
{
	int i;
	bool subXor[LENGTH>>1];			//子序列1，异或产生
	bool subEven[LENGTH>>1];		//子序列2，取偶产生

	float likehood_1,likehood_2;    //递归似然值
	float logValue;                
	int changeNum;					//likehood_1和likehood_2是否需交换
	int changeValue;				//likehood_1和likehood_2交换临界值
	int index;						//下标

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
	
		if(changeNum < changeValue)			//F运算
		{
			likehood_1 = LLR_Pre(LBase>>1,(LTop+1)>>1,subXor,bitInfo);
			LLRMap[0][index] = likehood_1;
			likehood_2 = LLR_Pre(LBase>>1,(LTop+1)>>1,subEven,bitInfo + changeValue);
			LLRMap[0][index + 1] = likehood_2;
		}

		else								//G运算
		{
			likehood_1 = LLR_Pre(LBase>>1,LTop>>1,subXor,bitInfo - changeValue);
			LLRMap[0][index - 1] = likehood_1;
			likehood_2 = LLR_Pre(LBase>>1,LTop>>1,subEven,bitInfo);
			LLRMap[0][index] = likehood_2;										
		}		
	}

	/*奇数时，进行f运算*/
	if(LTop % 2 == 1)
	{
		logValue = F(likehood_1,likehood_2);
	}
	/*偶数时，进行g运算*/
	else
	{
		logValue = G(likehood_1,likehood_2,uSub[(LTop-1)-1]);
	}
	
	return logValue;
}

/*********************************************************************************
*功能：计算不同路径的度量值
*参数：i:层数  order路径编号
*返回值：
*********************************************************************************/
void PM_scl(int i,int order)
{	
	float logResult;

	logResult = LLR(codeLength,i,out[order],invInfo[i-1],order);  //计算当前比特对数似然值

	/*查询位置信息，若为固定比特，则直接舍弃右孩子*/
	if(location[i-1] == 0)
	{	
		/*根据 1-2u 与对数似然值的符号选择相应表达式*/
		if(logResult < 0)
		{
			pmValue[order] = pmValue[order] + logResult;
		}
		out[order][i-1] = 0;
	}
	/*分别计算左右孩子度量值，根据 1-2u 与对数似然值的符号选择相应表达式*/
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
*功能：计算第 i 个结点的度量值，由于前 i-1 个节点全为0,等同于SC译码，可不计算，直
*	   接计算第 i 个节点
*参数：i:层数
*返回值：第 i 层度量值
*********************************************************************************/
void PMPrepare(int i)
{	
	float logResult;

	logResult = LLR_Pre(codeLength,i,out[0],invInfo[i-1]);  //当前比特对数似然值
	if (searchList == 1)									//搜索宽度为1，SC译码
	{
		/*根据 logResult 符号选择译码结果*/
		decode[i - 1] = logResult >= 0 ? 0 : 1;				//直接决定译码结果
		out[0][i - 1] = decode[i - 1];
	}
	else
	{
		/*分别计算左右孩子度量值*/
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
*功能：译码准备工作，计算第一个信息位
*参数：i:层数
*返回值：
*********************************************************************************/
void Prepare(int i)
{
	PMPrepare(i);
	if (searchList != 1)
	{
		out[0][i - 1] = 0;
		out[1][i - 1] = 1;
		memcpy(LLRMap[1], LLRMap[0], sizeof(LLRMap[0]));	//复制似然值信息表
		pathNum = 2;										//路径数扩展为2
	}
}

/*********************************************************************************
*功能：未达到搜索宽度时的路径扩展
*参数：i:层数
*返回值：无
*********************************************************************************/
void ExtendClone(int i)
{
	int j;
	for(j=pathNum-1;j>=0;j--)
	{
		memcpy(out[(j<<1)+1],out[j],sizeof(out[0]));			//复制路径
		memcpy(out[j<<1],out[j],sizeof(out[0]));
		out[(j<<1)+1][i] = 1;
		out[j<<1][i] = 0;

		memcpy(LLRMap[(j<<1)+1],LLRMap[j],sizeof(LLRMap[0]));	//复制似然值列表
		memcpy(LLRMap[j<<1],LLRMap[j],sizeof(LLRMap[0]));
	}
}

/*********************************************************************************
*功能：L条路径译码
*参数：
*返回值：
*********************************************************************************/
void scl()
{
	int i;
	int j;
	int m;
	int emptyIndex;									//零扩展结点顺序号
	int fullIndex;									//双扩展结点顺序号
	int extendPath[SEARCH_L_MAX] = {0};				//扩展方式判定

	pathNum = 1;									//当前搜索宽度

	if (codeLength == 1024)
	{
		Prepare(128);								//计算第128位
		i = 128;
	}
	else
	{
		Prepare(1);								//计算第60位
		i = 1;
	}

	for(;i<codeLength;i++)
	{
		/*求当前层所有路径的pm值*/
		for(j=pathNum-1;j>=0;j--)
		{
			PM_scl(i+1,j);
		}
		
		if(location[i] == 1)
		{
			if(pathNum < searchList)				//未达到搜索宽度时，遇到信息位则扩展
			{
				ExtendClone(i);						//路径复制
			}
			pathNum = pathNum<<1;
		}
		if(pathNum > searchList)					//达到搜索宽度时，遇到信息位需裁剪
		{
			pathNum = searchList;					//裁剪后的宽度

			memcpy(pmStore,pmValue,sizeof(pmValue));

			Sort(pmValue,(searchList<<1));			//返回前L条路径编号（0—L-1）

			for(m=0;m<searchList;m++)
			{
				extendPath[sortInfo[m]>>1]++;		//各父节点扩展数统计
			}
			emptyIndex = 0;
			fullIndex = 0;
			
			for(m=0;m<searchList;m++)
			{
				if(extendPath[m] == 0)				//未扩展，需删除
				{
					emptyStack[emptyIndex++] = m;	//保存父节点编号
				}
				else if(extendPath[m] == 2)			//扩展
				{
					fullStack[fullIndex++] = m;
				}
				else								//单条扩展，传递pm值和u值
				{
					pmValue[m] = pmStore[m<<1] >= pmStore[(m<<1)+1]? pmStore[m<<1]:pmStore[(m<<1)+1];
					out[m][i] = pmStore[m<<1] >= pmStore[(m<<1)+1]? 0:1;
				}
			}

			for(j=emptyIndex-1;j>=0;j--)
			{				
				/*无效路径替换为双路径的1*/
				memcpy(out[emptyStack[j]],out[fullStack[j]],sizeof(out[0]));
				out[emptyStack[j]][i] = 1;
				memcpy(LLRMap[emptyStack[j]],LLRMap[fullStack[j]],sizeof(LLRMap[0]));
				pmValue[emptyStack[j]] = pmStore[(fullStack[j]<<1)+1];
				/*双路径中的0继续延伸*/
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
*功能：SCL算法
*参数：
*返回值：译码结果
*********************************************************************************/
void Polar_scl()
{
	scl();
	memcpy(decode,out[sortInfo[0]],sizeof(out[0]));	//PM值最大的候选路径做最终译码结果
}
