/*********************************************************************************
*�Ŷӣ�5G-037
*���ߣ�xsfan
*˵����������������VS������Linuxϵͳ�±���
*********************************************************************************/

/************************************ͷ�ļ�**************************************/
#include    "ctr.h"
#include	<time.h> 
#ifdef __GNUC__
#include    <curses.h>
#else
#include    <conio.h>
#endif

int main()
{
	FILE *fp;
	time_t first,second;
	Initial();					//��ʼ��
	first=time(NULL);			//��ʱ
	Ctr();						//����
	second=time(NULL);			//��ʱ����

	printf(">   Total time = %.0f sec (%.2f ms/blk)\n",difftime(second,first),difftime(second,first)/(float)TOTAL_FRAME*1000);		//��ӡ������ʱ���ƽ��ʱ��
	printf("/////////////////////////////////////////////////////////////////////\n");

	fp = fopen("./result.txt", "a+"); 
	fprintf(fp,"Total time = %.0f sec (%.2f ms/blk)\n\n",difftime(second,first),difftime(second,first)/(float)TOTAL_FRAME*1000);	//������ļ�
	fclose(fp);

	Exit();						//�� "s" �˳�

	return 0;
}
