/*********************************************************************************
*�Ŷӣ�5G-037
*���ߣ�xsfan
*˵���������ƺ��������������볤���㷨��������ȵ�ѡ��
*********************************************************************************/

/************************************ͷ�ļ�**************************************/
#include "ctr.h"

/***********************************��������*************************************/
int codeLength;
int	selectAlgorithm;   
int	strNum;

/*********************************************************************************
*���ܣ���ʼ����������ȡ�����볤�������㷨����Ӧ�㷨���������
*������
*����ֵ��1
*********************************************************************************/
int Initial()
{
	char str[4];
	printf("/////////////////////////////////////////////////////////////////////\n");
	printf("//                                                                 //\n");
	printf("//   This is an applet about polar decoding. Please do as follows. //\n");
	printf("//                                                                 //\n");
	printf("//   First: Select the length of polar code.                       //\n");
	printf("//   1. 1024                                                       //\n");
	printf("//   2. 64                                                         //\n");
	printf("//                                                                 //\n");
	printf("//   Second: Select the algorithm you the want to simulate.        //\n");
	printf("//   1. Polar SC                                                   //\n");
	printf("//   2. Polar SCL                                                  //\n");
	printf("//   3. Polar CA-SCL                                               //\n");
	printf("//   4. Polar aCA-SCL                                              //\n");
	printf("//                                                                 //\n");
	printf("/////////////////////////////////////////////////////////////////////\n");
	printf("\n");
	
	printf("First:->Please select the length of Polar code(1,2):");		//��ȡ�볤
	scanf("%s",str);
	strNum = atoi(str);
	while((strNum!=1)&&(strNum!=2))
	{
		printf("There is not the code length you selected, please select again:");
		scanf("%s",str);
		strNum = atoi(str);
		if(strNum == -38 )      
		{
			scanf("%s",str);
		}
	}
	printf("The length of Polar code you selected is:");
	switch(strNum)
	{
		case 1: printf(" 1024\n");codeLength = 1024;	break;
		case 2 : printf(" 256\n");codeLength = 64;		break;

		default :printf(" \nThe algorithm does not exits!\n");
	}
	printf("\n");

	printf("Second->Please select the algorithm(1,2,3,4):");	//��ȡ�����㷨����
	scanf("%s",str);
	selectAlgorithm = atoi(str);
	while((selectAlgorithm<0) ||(selectAlgorithm>4))
	{
		printf("There is not the algorithm you selected, please select again:");
		scanf("%s",str);
		selectAlgorithm = atoi(str);
		if(selectAlgorithm == -38 )      
		{
			scanf("%s",str);
			selectAlgorithm = atoi(str);
		}
	}
	printf("The algorithm you selected is: "); 
	switch(selectAlgorithm)
	{
		case 1 : printf(" Polar SC \n"); acaLength = 1; break;
		case 2 : printf(" Polar SCL \n");				break;
		case 3 : printf(" Polar CA-SCL\n");				break;
		case 4 : printf(" Polar aCA-SCL\n");			break;

		default :printf(" \nThe algorithm does not exits!\n");
	}
	printf("\n");

	if(selectAlgorithm != 1)
	{
		printf("Please input Search List(1-%d), which must be the power of 2:", SEARCH_L_MAX);		//��ȡ�������
		scanf("%s",str);
		strNum = atoi(str);
		while(searchLengthCheck(strNum))
		{
			printf("There is not the search length you selected, please select again:");
			scanf("%s",str);
			strNum = atoi(str);
			if(strNum == -38 )      
			{
				scanf("%s",str);
				strNum = atoi(str);
			}
		}
		if(strNum > SEARCH_L_MAX)			//������Ȳ��ܴ�������趨ֵ
		{
			printf("The List you select is bigger, please modified SEARCH_L_MAX in includes.h");
			return 0;
		}
		searchList = strNum;				//������ȳ�ʼ��
		acaLength = strNum;
	}

	printf("\n");
	
	return 1;
}

/*********************************************************************************
*���ܣ����ƺ��������ݳ�ʼ��������ѡ��ִ����Ӧ����
*������
*����ֵ��1
*********************************************************************************/
int Ctr()
{
	int					frameDiffer;       
	unsigned long		bitErrorCounter = 0;		//������
	int					blockErrorCounter = 0;		//�����				
	float				bitErrorRate;				//������
	float				blockErrorRate;				//�����
	int					frameNum;
	FILE *fp;

	if (GetLocation() == 0 )						//��ȡλ����Ϣ
	{
		exit (1);
	}

	if (GetInv() == 0 )								//��ȡ�����
	{
		exit (1);
	}
	printf("Decoding, please wait ...\n\n");

	for(frameNum=START_FRAME;frameNum<STOP_FRAME;frameNum++)  
	{
		if (GetInputFrame(frameNum,codeLength) == 1)//��ȡ�ŵ�����ֵ 
		{
			switch(selectAlgorithm)  
			{
			    case 1  : Polar_sc();				break;
				case 2  : Polar_scl();				break;
				case 3  : Polar_ca_scl();			break;
				case 4  : Polar_aca_scl();			break;

				default : printf("The algorithm does not exits!\n");
			}
		}
		else
		{
			printf("The %d frame does not exits!\n",frameNum);
		}
		/*ͳ�����롢���*/
		if (GetSendFrame(frameNum,codeLength) == 1)  //��ȡ�ŵ�����ֵ
		{
			frameDiffer = CodeComp(send,decode);	 //�Ƚ�
			bitErrorCounter = bitErrorCounter + frameDiffer;
			if(frameDiffer)
			{
				frameDiffer = 0;
				blockErrorCounter++;
			}
		}	
	}
	printf("bitErrorCounter:%d\n", bitErrorCounter);

	bitErrorRate = bitErrorCounter/(float)TOTAL_FRAME/(codeLength/2-24);
	blockErrorRate = blockErrorCounter/(float)TOTAL_FRAME;
	
	printf("/////////////////////////////////////////////////////////////////////\n");
	printf(">   Result\n");
	printf(">   N = %d, K = %d, L = %d\n", codeLength, codeLength/2, acaLength);
	printf(">   Tot Blk = %d, ErrBlk =  %d\n", TOTAL_FRAME, blockErrorCounter);
	printf(">   BER = %.3e, BLER = %.3e\n",bitErrorRate, blockErrorRate);

	fp = fopen("./result.txt", "a+" ); 
	fprintf(fp,"The bit error rate of %d is %1.8f \n",TOTAL_FRAME,bitErrorRate);
	fprintf(fp,"The block error rate of %d is %f \n",TOTAL_FRAME,blockErrorRate);
	fclose(fp);

	return 1;
}
