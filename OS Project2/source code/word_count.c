//Reference:https://stackoverflow.com/questions/12698836/counting-words-in-a-string-c-programming
//Take the code on the website only for reference but I finish this function by myself
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int word_count(const char word[]){
	static int count=0; //result
	const char *pos =word;//postion to show the state
	int inword=0;
	do switch(*pos){ // use do-while  loop at least run once
		 case '\0': 
        case ' ': case '\t': case '\n': case '\r': 
            if (inword) { inword = 0; count++; }
            break;
        default: inword = 1;
	}while(*pos++); //continue to next position
	
	return count;
}
