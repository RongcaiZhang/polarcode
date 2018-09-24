/*********************************************************************************
*�Ŷӣ�5G-037
*���ߣ�xsfan
*˵�����ļ���ȡ����
*********************************************************************************/

/************************************ͷ�ļ�**************************************/
#include "gen_tx.h"
#include "common_cmd.h"
/*********************************************************************************
*���ܣ���ȡ���볤ƥ���λ����Ϣ�ļ�
*������
*����ֵ����ȷ 1		���� exit
*********************************************************************************/
int GetLocation()
{
	FILE  *fp1;
	int i;
	int a;
	int b;

	if(codeLength == 1024)
	{
		fp1 = fopen("../../../Encode/1024/location.bin","rb");
	}
	else
	{
		fp1 = fopen("../../../Encode/256/location2.bin","rb");
	}
	
	if(fp1 != NULL )
	{ 	
		for (i=0; i<codeLength; i++)
 		{	   	
			a=fread(&b,sizeof(int),1,fp1);
			if(a == 0)
			{
				fclose(fp1);
				printf("The data in location.bin is not enough!\n");
				Exit();
				return 0;
			}
			else
			{
        		location[i] = b;   //��ȡ
			}
		}			
		fclose(fp1);         
	}
	else
	{
 		printf("Error in open location.bin!\n");
		Exit();
		exit (1);
	}

	return 1;
}

/*********************************************************************************
*���ܣ���ȡ���볤ƥ��ķ�����Ϣ�ļ�
*������
*����ֵ����ȷ 1		���� exit
*********************************************************************************/
int GetInv()
{
	FILE  *fp1;
	int i;
	int a;
	int b;

	if(codeLength == 1024)
	{
		fp1 = fopen("../../../Encode/1024/inv1024.bin","rb");
	}
	else
	{
		fp1 = fopen("../../../Encode/256/inv.bin","rb");
	}
	
	if(fp1 != NULL )
	{ 	
		for (i=0; i<codeLength; i++)
 		{	   	
			a=fread(&b,sizeof(int),1,fp1);
			if(a == 0)
			{
				fclose(fp1);
				printf("The data in inv.bin is not enough!\n");
				Exit();
				return 0;
			}
			else
			{
        		invInfo[i] = b;   //��ȡ
			}
		}
		fclose(fp1); 
	}
	else
	{
 		printf("Error in open inv.bin!\n");
		Exit();
		exit (1);
	}
	return 1;
}
 
/*********************************************************************************
*���ܣ���ȡ���볤ƥ����ŵ����������ļ�
*������
*����ֵ����ȷ 1		���� exit
*********************************************************************************/
int GetInputFrame(uint frameNum, uint frameLength)
{
	FILE  *fp1;
	int i;
	int a;
	float b;
	int c;

	if(codeLength == 1024)
	{
		fp1 = fopen("../../../Encode/1024/input.bin","rb");
	}
	else
	{
		fp1 = fopen("../../../Encode/256/input.bin","rb");
	}
	
	if(fp1 != NULL )
	{ 	
		a = fseek(fp1,frameNum * frameLength *sizeof(float),SEEK_SET);   
		if(a == 0)	
		{
 			for (i=0; i<codeLength; i++)
 			{	   	
				c=fread(&b,sizeof(float),1,fp1);
				if(c == 0)
				{
					fclose(fp1);
					printf("The data in input.bin is not enough!\n");
					return 0;
				}
				else
				{
        			llr[i] = b;   //��ȡ
				}
			}
			fclose(fp1);         
		}
		else
		{
			fclose(fp1);
			printf("Error in location input.bin!\n");
			return 0;
		}
	}
	else
	{
 		printf("Error in open input.bin!\n");
		Exit();
		exit (1);
	}
	return 1;
}

/*********************************************************************************
*���ܣ���ȡ���볤ƥ�����ȷ���������ļ�
*������
*����ֵ����ȷ 1		���� exit
*********************************************************************************/
int GetSendFrame(uint frameNum, uint frameLength)
{
	FILE  *fp1;
	int i;
	int a;
	int b;
	int c;

	if(codeLength == 1024)
	{
		fp1 = fopen("../../../Encode/1024/send.bin","rb");
	}
	else
	{
		fp1 = fopen("../../../Encode/256/send.bin","rb");
	}
	
	if(fp1 != NULL )
	{ 	
		a = fseek(fp1,frameNum * frameLength *sizeof(int),SEEK_SET); 
		if(a == 0)	
		{
 			for (i=0; i<codeLength; i++)
 			{	   	
				c=fread(&b,sizeof(int),1,fp1);
				if(c == 0)
				{
					fclose(fp1);
					printf("The data in send.bin is not enough!\n");
					return 0;
				}
				else
				{
        			send[i] = b;   //��ȡ	
				}
			}
			fclose(fp1); 
		}
		else
		{
			fclose(fp1);
			printf("Error in location send.bin!\n");
			return 0;
		}
	}
	else
	{
 		printf("Error in open send.bin!\n");
		Exit();
		exit (1);
	}

	return 1;
}