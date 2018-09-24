/*********************************************************************************
*团队：5G-037
*作者：xsfan
*说明：主控制函数，包括测试码长、算法、搜索宽度的选择；
*********************************************************************************/

/************************************头文件**************************************/
#include "ctr.h"

/***********************************变量定义*************************************/
int codeLength;
int	selectAlgorithm;   
int	strNum;

/*********************************************************************************
*功能：初始化函数，获取测试码长、测试算法及相应算法的搜索宽度
*参数：
*返回值：1
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
	
	printf("First:->Please select the length of Polar code(1,2):");		//获取码长
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

	printf("Second->Please select the algorithm(1,2,3,4):");	//获取仿真算法类型
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
		printf("Please input Search List(1-%d), which must be the power of 2:", SEARCH_L_MAX);		//获取搜索宽度
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
		if(strNum > SEARCH_L_MAX)			//搜索宽度不能大于最大设定值
		{
			printf("The List you select is bigger, please modified SEARCH_L_MAX in includes.h");
			return 0;
		}
		searchList = strNum;				//搜索宽度初始化
		acaLength = strNum;
	}

	printf("\n");
	
	return 1;
}

/*********************************************************************************
*功能：控制函数，根据初始化参数，选择执行相应测试
*参数：
*返回值：1
*********************************************************************************/
int Ctr()
{
	int					frameDiffer;       
	unsigned long		bitErrorCounter = 0;		//误码数
	int					blockErrorCounter = 0;		//误块数				
	float				bitErrorRate;				//误码率
	float				blockErrorRate;				//误块率
	int					frameNum;
	FILE *fp;

	if (GetLocation() == 0 )						//读取位置信息
	{
		exit (1);
	}

	if (GetInv() == 0 )								//读取反序表
	{
		exit (1);
	}
	printf("Decoding, please wait ...\n\n");

	for(frameNum=START_FRAME;frameNum<STOP_FRAME;frameNum++)  
	{
		if (GetInputFrame(frameNum,codeLength) == 1)//读取信道接收值 
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
		/*统计误码、误块*/
		if (GetSendFrame(frameNum,codeLength) == 1)  //读取信道发送值
		{
			frameDiffer = CodeComp(send,decode);	 //比较
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
