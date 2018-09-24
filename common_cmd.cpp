/*********************************************************************************
*团队：5G-037
*作者：xsfan
*说明：辅助函数文件
*********************************************************************************/

/************************************头文件**************************************/
#include "common_cmd.h"
#include "includes.h"
//int G_output(float *big, float *small, float *select);
/***********************************变量定义*************************************/
bool checkInfo[LENGTH>>1];

/*********************************************************************************
*功能：输入 s 退出程序
*参数：
*返回值：
*********************************************************************************/
void Exit()
{
	char str;
	printf("\nPlease input <s> to exit:");
	str = getchar();
	str = getchar();
	while(str != 's')
	{
		printf("Please input the correct letter:"); 
		str = getchar();
		str = getchar();
	}
}

/*********************************************************************************
*功能：两组数进行比较
*参数：两个数组地址
*返回值：不同元素个数
*********************************************************************************/
int CodeComp(bool *code_orig,bool *code_out)
{  
	int i;
	int differ;
    
	differ = 0;
	for (i=0; i<codeLength-24; i++)
	{
		if(location[i] == 1)
		{
    		if(code_orig[i] != code_out[i])
			{
				differ = differ + 1;	
			}
		}
	}

	return(differ);
}
 
/*********************************************************************************
*功能：求两数相乘积的符号
*参数：a,b
*返回值：1,0,-1
*********************************************************************************/
int Sign(float a,float b)
{
	if((a>0 && b>0) || (a<0 && b<0))
		return 1;
	else
		return -1;
}

/*********************************************************************************
*功能：求两个数绝对值较小的数
*参数：a,b
*返回值：min(|a|,|b|)
*********************************************************************************/
float AbsMin(float a,float b)
{
	if(a < 0)
		a = -a;
	if(b < 0)
		b = -b;

	return a <= b? a:b;
}

/*********************************************************************************
*功能：F运算
*参数：似然值likehood_1, likehood_2
*返回值：log_value
*********************************************************************************/
float F(float likehood_1,float likehood_2)
{
	float log_value;

	if(Sign(likehood_1,likehood_2) ==1)
	{
		log_value = AbsMin(likehood_1,likehood_2);
	}  
      else
	{
		log_value = -AbsMin(likehood_1,likehood_2);
	}

	return log_value;
}

/*********************************************************************************
*功能：G运算
*参数：似然值likehood_1, likehood_2;	码字 u
*返回值：log_value
*********************************************************************************/
float G(float likehood_1,float likehood_2,bool u)
{
	int pow_value;
	float log_value;

	float log_value_add = likehood_2 + likehood_1;
	float log_value_sub = likehood_2 - likehood_1;

	if(u ==1)
	{
		log_value = likehood_2 - likehood_1;
	}	
	else
	{
		log_value = likehood_2 + likehood_1;
	}

	return log_value;
}
/*********************************************************************************
*功能：G运算状态输出
*参数：绝对值较大的abs_big, 绝对值较小的abs_small, 选择
*返回值：log_value
*********************************************************************************/
//int G_output(float *big, float *small, float *select)
//{
//	FILE *fp;
//	//存储abs_big
//	fopen_s(&fp, "abs_big.bin", "ab");
//	if (fp != NULL)
//	{
//		fwrite(big, sizeof(float), 1, fp);
//		fclose(fp);
//	}
//	else
//	{
//		printf("Error in open abs_big.bin!\n");
//	}
//	//存储abs_small
//	fopen_s(&fp, "abs_small.bin", "ab");
//	if (fp != NULL)
//	{
//		fwrite(small, sizeof(float), 1, fp);
//		fclose(fp);
//	}
//	else
//	{
//		printf("Error in open abs_small.bin!\n");
//	}
//
//	//存储select
//	fopen_s(&fp, "select.bin", "ab");
//	if (fp != NULL)
//	{
//		fwrite(select, sizeof(float), 1, fp);
//		fclose(fp);
//	}
//	else
//	{
//		printf("Error in open select.bin!\n");
//	}
//
//	return 0;
//}
/*********************************************************************************
*功能：排序，对sortNum个数排序searchList次
*参数：
*返回值：
*********************************************************************************/
void Sort(float *a,int sortNum)
{
	int i;
	int j;
	int num;
	float temp;

	for(i=0;i<sortNum;i++)
	{
		sortInfo[i] = i;
	}

	for(i=0;i<searchList;i++)
	{
		num = 0;
		for(j=sortNum -1;j>i;j--)
		{
			if(a[j] > a[j-1])
			{
				num++;
				temp = a[j];
				a[j] = a[j-1];
				a[j-1] = temp;

				temp = sortInfo[j];
				sortInfo[j] = sortInfo[j-1];
				sortInfo[j-1] = temp;
			}
		}
		if(num == 0)
		{
			break;
		}
	}
}

/*********************************************************************************
*功能：提取信息位码字
*参数：1024位译码序列
*返回值：
*********************************************************************************/
void ExtractInfo(bool *info)
{
	int i;
	int j;
	
	for(i=0,j=0;i<codeLength;i++)
	{
		if(location[i] == 1)
		{
			checkInfo[j++] = info[i];
		}
	}
}

/*********************************************************************************
*功能：24位 CRC 校验
*参数：校验序列
*返回值：成功 0		失败 1
*********************************************************************************/
int CrcCheck()
{
	int i;
	int j;
	int sum = 0;
	bool crc[2];
	bool gen[2] = {1,0};

	for(i=0;i<1;i++)
	{
		crc[i] = checkInfo[i];
	}

	for(i=24;i<(codeLength>>1);i++)
	{
		crc[24] = checkInfo[i];
		if(crc[0] == 1)
		{
			for(j=1;j<25;j++)
			{
				crc[j-1] = crc[j] ^ gen[j];
			}
		}
		else
		{
			for(j=1;j<25;j++)
			{
				crc[j-1] = crc[j];
			}
		}
	}

	for(i=0;i<24;i++)
	{
		sum = sum + crc[i];
	}

	if(sum == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}	
}

/*********************************************************************************
*功能：判断搜索宽度是否符合要求
*参数：标准输入获取的搜索宽度
*返回值：成功 0		失败 1
*********************************************************************************/
int searchLengthCheck(int i)
{
	int j;
	if(i < 1)
	{
		printf("Search List must no less than 1.\n");
		return 1;
	}
	if (i > SEARCH_L_MAX)
	{
		printf("Search List must no more than %d.\n", SEARCH_L_MAX);
		printf("If you want to simulate bigger List, please modified SEARCH_L_MAX in includes.h\n");
		return 1;
	}
	while(i>1)
	{
		j = i % 2;
		if (j == 1)
		{
			printf("Search List must be the power of 2.\n");
			return 1;
		}
		i = i / 2;
	}
	return 0;
}
