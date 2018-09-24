/*********************************************************************************
*团队：5G-037
*作者：xsfan
*说明：主函数，可在VS环境或Linux系统下编译
*********************************************************************************/

/************************************头文件**************************************/
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
	Initial();					//初始化
	first=time(NULL);			//计时
	Ctr();						//译码
	second=time(NULL);			//计时结束

	printf(">   Total time = %.0f sec (%.2f ms/blk)\n",difftime(second,first),difftime(second,first)/(float)TOTAL_FRAME*1000);		//打印译码总时间和平均时间
	printf("/////////////////////////////////////////////////////////////////////\n");

	fp = fopen("./result.txt", "a+"); 
	fprintf(fp,"Total time = %.0f sec (%.2f ms/blk)\n\n",difftime(second,first),difftime(second,first)/(float)TOTAL_FRAME*1000);	//输出到文件
	fclose(fp);

	Exit();						//按 "s" 退出

	return 0;
}
