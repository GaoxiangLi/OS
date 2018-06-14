#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

char *load_file(const char *filename){
	FILE *fp;
	 static char res[100];
	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("Read file failed!.\n");
		exit(1);
	}else{
		printf("Read file success!\n");
		fgets(res,100,fp);
		return res;
		fclose(fp);
	}
};