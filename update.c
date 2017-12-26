#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crc32.h"

/* define update size once transmit */
#define BUFSize 1024*2

int UpdProgess(int pnow,int pcount,int ptotal)
{
    float per;
    if(pcount == 0)
    {
 	pcount = pnow;
    }
    else
    {
	pcount = (pcount-1)*BUFSize + pnow; 
    }
    per = (float)(100 * pcount) / ptotal;
    printf("===========Complish Percent:%4.2f %%==============\n",per);
}

int main(int argc,char **argv)
{
    /* crc head for check update*/
    unsigned char crc32[4] = {0xA5,0x5A,0X88,0X88};
    int crc32Val,len_count = 0;
    /* crear point for update file */
    FILE *fp_in = NULL;
    fp_in = fopen(argv[1],"rb");
    if(NULL == fp_in)
    {
    	printf("fp_in file can not open!\n");
	return 1;
    }
    
    FILE *fp_out = NULL;
    fp_out = fopen(argv[2],"wb");
    if(NULL == fp_out)
    {
    	printf("fp_out file can not open!\n");
	return 1;
    }
    /* write crc head */
    fwrite(crc32,1,4,fp_out);
    /* get total len of update file*/
    fseek(fp_in,0L,SEEK_END);
    int len_total = ftell(fp_in);
    fseek(fp_in,0L,SEEK_SET);
    
    unsigned char *pData = (unsigned char *)malloc(BUFSize);
    InitCrc32();
    /* read buffer data and move point then print progress */
    while(1)
    {
        memset(pData,0,BUFSize);
	int len_Data = fread(pData,1,BUFSize,fp_in);
    	if(len_Data <= 0)	break;
        fwrite(pData,1,len_Data,fp_out);
        UpdCrc32(pData,len_Data);
        UpdProgess(len_Data,len_count++,len_total);
    }    
    free(pData);
    fclose(fp_in);
    /* write the last crc for user check */
    crc32Val = FinalCrc32();

    crc32[0] = (crc32Val & 0x000000FFUL);
    crc32[1] = (crc32Val & 0x0000FF00UL) >> 8;
    crc32[2] = (crc32Val & 0x00FF0000UL) >> 16;
    crc32[3] = (crc32Val & 0xFF000000UL) >> 24;

    fseek(fp_out,0,SEEK_SET);
    fwrite(crc32,1,4,fp_out);

    printf("============Finish==============\n");  
    fclose(fp_out);
    return 0;
}
