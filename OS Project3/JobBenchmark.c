/*************************************************************************
	> File Name: JobBenchmark.c
	> Author:Xueyang Hu 
	> Mail: xzh0051
	> Created Time: Sun 25 Feb 2018 09:21:48 PM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{   
    int exetime;
    if(argc=!2)
    {
        perror("JobBenchmark:");

    }
    else
    {
        exetime=atoi(argv[1]);
        sleep(exetime);
        //printf("finished running!\n");
    }
 return 0; 
}
