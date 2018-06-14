/*************************************************************************
	> File Name: project3.c
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Feb 2018 03:25:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
#include<sys/wait.h>

#define QUESIZE 100           //Job queue size
#define FIN_QUEUE 100       //finished queue sizeof
#define ARR_RATE 1          //ARRIVE RATE IS 1 Job per second
typedef struct Process   //job queue and finished queue data type
{
    char name[10];      //job name 
    char elaTime[10];      //burst time
    time_t arrTime;      //arrive time
    time_t staTime;     //start time;
    time_t finTime;      //finish time
    double cpuTime;     //cpu time 
    double watTime;      //waiting time
    int priority;       //pirority

}PCB;

typedef struct 
{
    char operation[10];
    char job_name[10];
    char burstTime[10];
    int pri;
    char policy[10];
    int job_num;
    int pri_level;
    int max_cpu;
    int min_cpu;
}command_t;

PCB Job_Queue[QUESIZE];
PCB Fin_Queue[FIN_QUEUE];
int stop=0; //quit signal
int in=0;
int out=0;
int count=0;
int fin_count=0;
int total_job=0;
char sys_policy[10]="fcfs";
pthread_mutex_t queue_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full_cv=PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty_cv=PTHREAD_COND_INITIALIZER;

command_t* command_line_parser(char* input)
{
    command_t* command;
    command=(command_t*) malloc(sizeof(command_t));
    memset(command,0,sizeof(command_t));
    //command->pri=1;
    sscanf(input,"%s",command->operation);
    if (strcmp(command->operation,"run")==0)
    {  
        sscanf(input,"%s %s %s %d",command->operation,command->job_name,command->burstTime,&command->pri);
        return command;
    }
    else if (strcmp(command->operation,"help")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"test")==0)
    {
        //test <benchmark> <policy> <num_of_jobs> <priority_levels><min_CPU_time> <max_CPU_time>a
        sscanf(input,"%s %s %s %d %d %d %d",command->operation,command->job_name,command->policy,&command->job_num,&command->pri_level,&command->min_cpu,&command->max_cpu);
        return command;
    }
    else if (strcmp(command->operation,"list")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"sjf")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"fcfs")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"priority")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"quit")==0)
    {
        return command;
    }
    else
    {
        strcpy(command->operation,"error");
        return command;
    }
}

void fcfs()
{   pthread_mutex_lock(&queue_mutex);
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==QUESIZE)
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==QUESIZE)
            {
                j=0;
            }
            if(Job_Queue[i].arrTime > Job_Queue[j].arrTime)  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"fcfs");
    pthread_mutex_unlock(&queue_mutex);
    
}
void sjf()
{   
    pthread_mutex_lock(&queue_mutex);
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==QUESIZE)
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==QUESIZE)
            {
                j=0;
            }
            if(atoi(Job_Queue[i].elaTime)>atoi(Job_Queue[j].elaTime))  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"sjf");
    pthread_mutex_unlock(&queue_mutex);
    }





void priority_sort()
{   
    pthread_mutex_lock(&queue_mutex);
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==QUESIZE)
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==QUESIZE)
            {
                j=0;
            }
            if(Job_Queue[i].priority < Job_Queue[j].priority)  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"priority");
   
    pthread_mutex_unlock(&queue_mutex);
}


void list_queue()
{   
    struct tm *info;
    int i;
    pthread_mutex_lock(&queue_mutex);
    printf("Total number of jobs in the waiting queue:%d\n",count);
    printf("scheduling Policy:%s\n",sys_policy);
    if(count==0)
    {
        printf("Waiting Job Queue is currently empty!\n");

    }
    else
    printf("Name\tCPU_Time\tPri\tArr_Time\tStatus\n");
    for(i=out;i!=in;i++)
    {   
        if(i==(QUESIZE))
        {
            i=0;
        }
        printf("%s\t%s\t%10d\t",Job_Queue[i].name,Job_Queue[i].elaTime,Job_Queue[i].priority);
        info=gmtime(&Job_Queue[i].arrTime);
        printf("%2d:%2d:%2d\t",(info->tm_hour+19)%24,info->tm_min,info->tm_sec);
        if(i==out)
        {
            printf("running\n");
        }
        else
        printf("\n");
    }
    /*printf("finished Job Queue:\n");
    for(i=0;i<fin_count;i++)
    {   
        printf("%s,%s,%d\n",Fin_Queue[i].name,Fin_Queue[i].elaTime,Fin_Queue[i].priority);
        printf("%s\t%s\t%10d\t",Fin_Queue[i].name,Fin_Queue[i].elaTime,Fin_Queue[i].priority);
        info=gmtime(&Job_Queue[i].arrTime);
        printf("%d:%d:%d\t",(info->tm_hour+19)%24,info->tm_min,info->tm_sec);
        if(i==out)
        {
            printf("running\n");
        }
        else
        printf("\n");
    }*/
    pthread_mutex_unlock(&queue_mutex);
}




void help()
{
    printf("run<job><time> <pri>:submit a job named <job>, execution time is <time>, priority is <pri>.\nlist:display the job status.\nfcfs: change the scheduling policy to FCFS.\nsjf: change the scheduling policy to SJF.\npriority: change the scheduling policy to priority.\ntest <benchmark><policy> <num_of_jobs><priority_levels> <min_CPU_time><max_CPU_time>\nquit: exit AUbatch\n");
}



void *scheduler(void *argv)
{   
    double EstTime=0;
    int i,j;
    time_t now;
    command_t *new_command=(command_t*)argv;
    if(strcmp(new_command->operation,"run")==0)
    {  
        pthread_mutex_lock(&queue_mutex);
        while(count==QUESIZE)
        {
            printf("The Job Queue is full, waiting for dispatch\n");
            pthread_cond_wait(&not_full_cv,&queue_mutex);
        }
        if(count==0)
        {
            pthread_cond_signal(&not_empty_cv);
        }
        count++;
        strcpy(Job_Queue[in].name,new_command->job_name);
        strcpy(Job_Queue[in].elaTime,new_command->burstTime);
        Job_Queue[in].priority=new_command->pri;
        Job_Queue[in].arrTime=time(NULL);
        printf("Job <%s> was submitted.\n ",Job_Queue[in].name);
        printf("Total job in the queue:%d.\n",count);  
        for(i=out;i!=in;i++)
        { 
            if(i==QUESIZE)
             {
                i=0;
             }
            EstTime+=atoi(Job_Queue[i].elaTime);
        }
        in++;
        if(in==QUESIZE)
        {
            in=0;
        }
        pthread_mutex_unlock(&queue_mutex);
        if(strcmp(sys_policy,"sjf")==0)
        {
            sjf();
        }
        else if(strcmp(sys_policy,"priority")==0)
        {
            priority_sort();
        }
        else if(strcmp(sys_policy,"fcfs")==0)
        {
            fcfs();
        }
        //now=time(NULL);
        //EstTime=EstTime+difftime(now,Job_Queue[out].staTime);
        printf("Expected waiting time:%.0f\n",EstTime);
    }
    else if(strcmp(new_command->operation,"sjf")==0)
        {
            sjf();
            printf("Scheduling policy is switch to sjf\n");
        }
    else if(strcmp(new_command->operation,"fcfs")==0)
        {
            fcfs();
            printf("Scheduling policy is switch to fcfs\n");
        }
    else if(strcmp(new_command->operation,"priority")==0)
        {
            priority_sort();
            printf("Scheduling policy is switch to priority.\n");
        }
    else if(strcmp(new_command->operation,"test")==0)
        {
            printf("Start running auto test module\n");
            pthread_mutex_lock(&queue_mutex);
            fin_count=0;
            total_job=new_command->job_num;
            pthread_mutex_unlock(&queue_mutex);
            //test <benchmark> <policy> <num_of_jobs> <priority_levels><min_CPU_time> <max_CPU_time>a
            printf("Test Information:\nThe benchmark name is: %s\nPolicy is:%s\nNumber of job: %d\npriority levels are %d\nMIN_CPU time: %d\nMAX_CPU time: %d\n",new_command->job_name,new_command->policy,new_command->job_num,new_command->pri_level,new_command->min_cpu,new_command->max_cpu);
            printf("default arrive rate is:%dNo/sec\n",ARR_RATE);
            srand(time(NULL));
            for(j=0;j<new_command->job_num;j++)
            {   
                pthread_mutex_unlock(&queue_mutex);
                strcpy(sys_policy,new_command->policy);
                while(count==QUESIZE)
                {
                    //printf("The Job Queue is full, waiting for dispatch\n");
                    pthread_cond_wait(&not_full_cv,&queue_mutex);
                }
                if(count==0)
                {
                    pthread_cond_signal(&not_empty_cv);
                }
                count++;
                strcpy(Job_Queue[in].name,new_command->job_name);
                //srand(time(NULL));
                //j=rand()%(new_command->min_cpu);
                //j=j+(new_command->max_cpu-new_command->min_cpu);
                sprintf(Job_Queue[in].elaTime,"%d",((rand())%(new_command->max_cpu-new_command->min_cpu)+(new_command->min_cpu)));
                //sprintf(Job_Queue[in].elaTime,"%d",j);
                //itoa(Job_Queue[in].elaTime,(rand()%new_command->min_cpu)+(new_command->max_cpu-new_command->min_cpu),10);
                //srand(time(NULL));
                Job_Queue[in].priority=rand()%new_command->pri_level;
                Job_Queue[in].arrTime=time(NULL);
                in++;
                if(in==QUESIZE)
                {
                    in=0;
                }
                pthread_mutex_unlock(&queue_mutex);
                if(strcmp(sys_policy,"sjf")==0)
                {
                    sjf();
                }
                else if(strcmp(sys_policy,"priority")==0)
                {
                    priority_sort();
                }
                else if(strcmp(sys_policy,"fcfs")==0)
                {
                    fcfs();
                } 
                pthread_mutex_unlock(&queue_mutex);
                sleep(ARR_RATE);
        //now=time(NULL);
        //EstTime=EstTime+difftime(now,Job_Queue[out].staTime);
            }
        printf("Finished scheduling all %d test job, Please wait for result.\n",new_command->job_num);
        }
    pthread_exit(NULL);
}
void *dispatcher(void *argv)
{ 
    pid_t pid;
    pid_t temp;
    int i;
    char *Exearg[3];
    //printf("The process name is:%s, estimate running time: %s\n",Job_Queue[0].name,Job_Queue[0].elaTime);
    while(stop==0)
    {
        pthread_mutex_lock(&queue_mutex);
        while(count==0)
        {
            //printf("The Job Queue is currently empty, waiting for new job\n");
            pthread_cond_wait(&not_empty_cv,&queue_mutex);
           
        }
        if(stop==1)
        {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        Exearg[0]=Job_Queue[out].name;
        Exearg[1]=Job_Queue[out].elaTime;    
        Exearg[2]=NULL;
        Job_Queue[out].staTime=time(NULL);
        pthread_mutex_unlock(&queue_mutex);
        switch((pid=fork()))
        {
            case -1:
            perror("fork");
            break;
            case 0:     //child process replaced by the job
            execv(Exearg[0],Exearg);
            puts("Error during execv");
            exit(-1);
            break;
            default:  //parent process print 
            //puts("dispatcher running\n");
            break;
        }
        wait(NULL);
        pthread_mutex_lock(&queue_mutex);
        Job_Queue[out].finTime=time(NULL);
        Job_Queue[out].cpuTime=difftime(Job_Queue[out].finTime,Job_Queue[out].staTime);
        Job_Queue[out].watTime=difftime(Job_Queue[out].staTime,Job_Queue[out].arrTime);
        if(count==QUESIZE)
        {
            pthread_cond_signal(&not_full_cv);
        }
        count--;
        memcpy(Fin_Queue+fin_count,Job_Queue+out,sizeof(PCB)); //move job to finished queue
        //FIN_QUEUE[fin_count]=Job_Queue[out];
        fin_count++;
        out++;
        if(out==QUESIZE)
        {
            out=0;
        }
        if(fin_count==total_job)
        {   
            float avg_turnarond=0;
            float avg_cpu=0;
            float avg_wait=0;
            float throughput=0;
            for(i=0;i<total_job;i++)
            {
                avg_cpu+=(Fin_Queue[i].cpuTime/total_job);
                avg_wait+=(Fin_Queue[i].watTime/total_job);
            }
            avg_turnarond=avg_cpu+avg_wait;
            printf("Total number of job finised:%d\n",total_job);
            printf("Average Turn around Time:%3.3f\n",avg_turnarond);
            printf("Average CPU Time:%3.3f\n",avg_cpu);
            printf("Average waiting Time:%3.3f\n",avg_wait);
            printf("Throughput:%3.3f\n",1/avg_turnarond);

        }
        pthread_mutex_unlock(&queue_mutex);
    }
    //printf("dispatcher stopped!\n");
    pthread_exit(NULL);
}


int main()
{
    int i;
    char c;
    pthread_t tid[2];
    command_t *input_command=NULL;
    char input_buff[100];
    pthread_create(&tid[1],NULL,dispatcher,NULL);// dispatcher running
    printf("Welcome to Xueyang Hu's batchjob scheduler Version 1.0\nType ‘help’ to find more about AUbatch commands.\n");
    while(1)
    {   
        printf(">");
        scanf("%[^\n]",input_buff);
        c=getchar();
        input_command=command_line_parser(input_buff);
        if(strcmp(input_command->operation,"run")==0)
        {
             pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
             pthread_join(tid[0],NULL); 
        }
        if(strcmp(input_command->operation,"list")==0)
        {
             list_queue();
        }
        if(strcmp(input_command->operation,"help")==0)
        {
             help();
        }
       if(((strcmp(input_command->operation,"sjf"))&&(strcmp(input_command->operation,"fcfs"))&&(strcmp(input_command->operation,"priority")))==0)
        {    
            pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
            pthread_join(tid[0],NULL); 
        } 
        if(strcmp(input_command->operation,"test")==0)
        {
            pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
            pthread_join(tid[0],NULL); 
        }
        if(strcmp(input_command->operation,"error")==0)
        {
            printf("Unknown command! Type <help> to get more information\n");
        }
        if(strcmp(input_command->operation,"quit")==0)
        {   
            
            printf("System closing, please wait\n");
            stop=1;
            float avg_turnarond=0;
            float avg_cpu=0;
            float avg_wait=0;
            float throughput=0;
            //pthread_mutex_lock(&queue_mutex);
            for(i=0;i<fin_count;i++)
            {
                avg_cpu+=(Fin_Queue[i].cpuTime/fin_count);
                avg_wait+=(Fin_Queue[i].watTime/fin_count);
            }
            avg_turnarond=avg_cpu+avg_wait;
            printf("Total number of job finised:%d\n",fin_count);
            printf("Average Turn around Time:%3.3f\n",avg_turnarond);
            printf("Average CPU Time:%3.3f\n",avg_cpu);
            printf("Average waiting Time:%3.3f\n",avg_wait);
            printf("Throughput:%3.3f\n",1/avg_turnarond);
            //pthread_mutex_unlock(&queue_mutex);
            pthread_cond_signal(&not_empty_cv);
            //printf("dispatcher stoped successfully_1!\n");
            //pthread_mutex_destroy(&queue_mutex);
            //pthread_cond_destroy(&not_full_cv);
            //pthread_cond_destroy(&not_empty_cv);
            //printf("dispatcher stoped successfully!_2\n");
            //pthread_exit(NULL); 
            break;

        }
    }
    printf("Thanks for using!\n");
    return 0;
}
