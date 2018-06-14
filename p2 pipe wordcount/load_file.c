#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

char *load_file(const char *filename){
	FILE *fp;
	static char res[30];
	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("Read file failed!");
		exit(1);
	}else{
		printf("Read file success!");
		fgets(res,200,fp);
		return res;
		fclose(fp);
	}
};