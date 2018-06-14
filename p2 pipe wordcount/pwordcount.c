#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define BUFFER_SIZE 100
#define READ_END 0
#define WRITE_END 1


	int main(int argc, char *argv[]){
		if(argc==0){
			printf("Please enter a file name.\n Usage: ./pwordcount <file_name>");
			return 0;
		}
		if(argc!=2&&argc!=0){//invalid input 
			printf("Invalid input please check your input.\n Usage: ./pwordcount <file_name>");
			return 0;
		}
		if(argc==2){
			printf("Begin to load file.\n");
			if(!load_file(argv[1])){// load file failed
				return 0;
			}else{
			printf("Load file into buffer.\n");
			char write_msg[BUFFER_SIZE];
			write_msg=load_file(argv[1]);
			
			}
			int child_result=0; //count result in child process
			int result=0;       //final result
			char read_msg[BUFFER_SIZE];
			pid_t pid;
			int fd1[2];
			
			printf("Try to create pipe");
			/* create the first pipe to send msg to child */
			pipe(fd1);
			if (pipe(fd1) == -1) {
				fprintf(stderr,"Pipe failed");
				return 1;
			}
			
			/* now fork a child process */
			printf("Try to fork a child process")
			pid = fork();
			
			if (pid < 0) {
				fprintf(stderr, "Fork failed");
				return 1;
			}
			
			if (pid > 0) { /* parent process */
					close(fd1[READ_END]);
					
					/* write to the pipe */
					printf("Parent process begin to write msg to pipe");
					write(fd1[WRITE_END], write_msg, strlen(write_msg)+1);
					
					/* close the write end of the pipe */
					close(fd1[WRITE_END]);
			}else { /* child process */
					close(fd1[WRITE_END]);
					
					/* read from the pipe */
					printf("Child process begin to read msg from pipe");
					read(fd1[READ_END], read_msg, BUFFER_SIZE);
					
					/* close the write end of the pipe */
					close(fd1[READ_END]);
					
					printf("Child begin to count words.\n")
					child_result=word_count(read_msg);
					
					/* write to the pipe */
					printf("Child process begin to send result to parent by pipe");
					write(fd[WRITE_END], child_result, 1);
					/* close the write end of the pipe */
					close(fd[WRITE_END]);
			}
				
			int fd2[2];
			printf("Try to create another pipe");
			/* create the second pipe to send msg to parent */
			pipe(fd2);
			if (pipe(fd2) == -1) {
				fprintf(stderr,"Pipe failed");
				return 1;
			}
				
				
			if (pid < 0) {
				printf(stderr, "Fork failed");
				return 1;
			}
			
			if (pid ==0) {/* child process */
					/* close the unused end of the pipe */
					close(fd2[READ_END]);
					
					printf("Child begin to count words.\n")
					child_result=word_count(read_msg);
					
					/* write to the pipe */
					printf("Child process begin to send result to parent by pipe.\n");
					write(fd2[WRITE_END], child_result, 1);
					
					/* close the write end of the pipe */
					close(fd2[WRITE_END]);
					
			}else {  /* parent process */
			
					/* close the unused end of the pipe */
					close(fd2[WRITE_END]);
					
					/* read from the pipe */
					printf("parent process get result from pipe.\n");
					read(fd2[READ_END], result, 1);
					
					//print the result
					printf("The result is %s.\n"result);
					
					/* close the write end of the pipe */
					close(fd2[READ_END]);
			}

		}	
		return 0;
	}