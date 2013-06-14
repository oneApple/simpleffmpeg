#include"GetYVector.h"
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>


static int iframe = 1;
static char *file = NULL;
static char *dir = NULL;
extern char *gfilename;


static char *getfilename()
{
	char *first = strrchr(gfilename,'/') + 1;
	char *last = strchr(gfilename,'.');
	int len = last - first;
	char *filename = malloc(len + 1);
	memset(filename,0,len + 1);
	strncpy(filename,first,len);
	printf("%s,%s,%s,%s\n",gfilename,first,last,filename);
	return filename;
}

int GetYVector(AVFrame picture)
{
	if(picture.pict_type == 1) //如果是i帧
	{
		//定义一个二维数组temp[a][b];
		FILE *fp;
		int flag = 1;

		if(file == NULL)
		{
            char *filename = getfilename();
            mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            int filenamelen = strlen(filename);
            dir = malloc(filenamelen + 12 + 8);
			strncpy(dir,filename,filenamelen);
			file = dir + filenamelen;
			free(filename);
		}
		sprintf(file, "/%d", iframe);
		strcat(file,".yvector");

		if ((fp=fopen(dir,"w"))==NULL) /*打开只写的文本文件*/
		{
			return 1;
		}
		iframe ++;

		int wi ,hi;
		for(hi = 0; flag;++ hi)
		{
			for(wi = 0; wi < picture.linesize[0] - 32;++ wi)
			{
				uint8_t yvector = picture.data[0][hi * picture.linesize[0] + wi];
				if((flag == 1) && (yvector == 128))
				{
					flag = 1;
				}
				else
				{
					flag = 0;
				}
				fprintf(fp,"%d,",yvector);
			}
			fputs("\n",fp);
			if(flag == 1)
			{
				fprintf(fp,"%d,%d\n",wi,hi);
				fclose(fp); /*关文件*/
				return 1;
			}
			else
			{
				flag = 1;
			}
		}
		fclose(fp); /*关文件*/
	}

	return 0;
}
