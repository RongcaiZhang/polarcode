/*********************************************************************************
*�Ŷӣ�5G-037
*���ߣ�xsfan
*˵�������������ļ�
*********************************************************************************/

/************************************ͷ�ļ�**************************************/
#include "common_cmd.h"
#include "includes.h"
//int G_output(float *big, float *small, float *select);
/***********************************��������*************************************/
bool checkInfo[LENGTH>>1];

/*********************************************************************************
*���ܣ����� s �˳�����
*������
*����ֵ��
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
*���ܣ����������бȽ�
*���������������ַ
*����ֵ����ͬԪ�ظ���
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
*���ܣ���������˻��ķ���
*������a,b
*����ֵ��1,0,-1
*********************************************************************************/
int Sign(float a,float b)
{
	if((a>0 && b>0) || (a<0 && b<0))
		return 1;
	else
		return -1;
}

/*********************************************************************************
*���ܣ�������������ֵ��С����
*������a,b
*����ֵ��min(|a|,|b|)
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
*���ܣ�F����
*��������Ȼֵlikehood_1, likehood_2
*����ֵ��log_value
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
*���ܣ�G����
*��������Ȼֵlikehood_1, likehood_2;	���� u
*����ֵ��log_value
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
*���ܣ�G����״̬���
*����������ֵ�ϴ��abs_big, ����ֵ��С��abs_small, ѡ��
*����ֵ��log_value
*********************************************************************************/
//int G_output(float *big, float *small, float *select)
//{
//	FILE *fp;
//	//�洢abs_big
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
//	//�洢abs_small
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
//	//�洢select
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
*���ܣ����򣬶�sortNum��������searchList��
*������
*����ֵ��
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
*���ܣ���ȡ��Ϣλ����
*������1024λ��������
*����ֵ��
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
*���ܣ�24λ CRC У��
*������У������
*����ֵ���ɹ� 0		ʧ�� 1
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
*���ܣ��ж���������Ƿ����Ҫ��
*��������׼�����ȡ���������
*����ֵ���ɹ� 0		ʧ�� 1
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
