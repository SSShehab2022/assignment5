#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void task1(char*);
void task2(char* ,int*);
void task3(char***,int*);
void task4(char*,char***,int*);

static char*** statement = NULL;
char* op = NULL;

int main(){
	char buf[255];
	int status;
	int paragraph_count = 0;
	while(1){
		printf("I am ready > ");
		gets(buf);
		int ret_pid = fork();
		if(ret_pid == 0){
			task1(buf);
			break;
		}
		wait(&status);
		task2(buf,&paragraph_count);
		if(strcmp(buf,"set") == 0){
			task3(statement,&paragraph_count);
		}
		task4(buf,statement,&paragraph_count);
	}
	return 0;
}


/************************************************ Task1 Implementation***********************************************/
/******************************************************************************
*Description: - check if there is valid command, example: ls.
******************************************************************************/
void task1(char * buf){	
	int i = 0,str_count = 1,flag = 0,j = 0;
	char** paragraph = (char**) malloc(sizeof(char*) * str_count);
	paragraph[str_count - 1] = (char*) malloc(sizeof(char) * 255);
	while(buf[i] != '\0'){
		if(buf[i] != ' '){
			paragraph[str_count - 1][j] = buf[i];
			flag = 1;
		}else{
			if(flag == 1){
				paragraph[str_count - 1][j] = '\0';
				/* Every space, it makes new string pointer to act as purser */
				paragraph = (char**) realloc(paragraph,sizeof(char*) * ++str_count);
				paragraph[str_count - 1] = (char*) malloc(sizeof(char) * 255);
				j = -1;
			}
			flag = 0;
		}
		i++;
		j++;
	}
	paragraph = (char**) realloc(paragraph,sizeof(char*) * ++str_count);
	paragraph[str_count - 1] = NULL;
	execvp(paragraph[0],paragraph);
	free(paragraph); 
}



/************************************************ Task2 Implementation***********************************************/
/*************************************************************************************
*Description: - If equal '=' exists in buf, this function saves the equality operands.
*************************************************************************************/
void task2(char * buf,int* paragraph_count){	
	int i = 0,j = 0,index = 0;
	int flag = 0;
	while(buf[i] != '\0'){
		if(buf[i] == '=')
			flag = 1;
		i++;
	}
	if(flag == 0)
		return;
	i = 0;
	*paragraph_count += 1;
	statement = (char***) realloc(statement,sizeof(char**) * *paragraph_count);
	statement[(*paragraph_count) - 1] = (char**) malloc(sizeof(char*) * 2);
	statement[(*paragraph_count) - 1][0] = (char*) malloc(sizeof(char) * 255);
	statement[(*paragraph_count) - 1][1] = (char*) malloc(sizeof(char) * 255);
	while(buf[i] != '\0'){
		if( (buf[i] >= 'a' && buf[i]<= 'z') ||  (buf[i] >= 'A' && buf[i]<= 'Z') 
                                   || (buf[i] >= '0' && buf[i]<= '9') || buf[i] == '_'){
			statement[(*paragraph_count) - 1][index][j] = buf[i];
			j++;
		
		}else if(buf[i] == '='){
			statement[(*paragraph_count) - 1][index][j] = '\0';
			index = 1;
			j = 0;
		}
		i++;	
	}
}


/************************************************ Task3 Implementation***********************************************/
/*************************************************************************************
*Description: - Print the local variables which obtained by task2 to display them.
*************************************************************************************/
void task3(char*** statement,int* paragraph_count){
	for(int par_index = 0; par_index < *paragraph_count; par_index++){
		printf("local_vars[%d]: ",par_index);
		for(int op_index = 0; op_index < 2; op_index++){
			printf("%s ",statement[par_index][op_index]);
			if(op_index == 1)
				continue;
			printf("= ");
		}
		printf("\n");
	}
}

/************************************************ Task4 Implementation***********************************************/
/***********************************************************************************************
*Description: - Provides builtin command "export" and set its operand as env variable by setenv.
***********************************************************************************************/
void task4(char* buf,char*** statement,int* paragraph_count){
	int i = 0, j = 0,flag = 0,index = 0;
	char** purser = (char**) malloc(2 * sizeof(char*));
	purser[0] = (char*) malloc(250 * sizeof(char));
	purser[1] = (char*) malloc(250 * sizeof(char));
	while(buf[i] != ' ' && buf[i] != '\0'){
		purser[0][j] = buf[i];
		i++;j++;
	}
	purser[0][j] = '\0';
	while(buf[i] != '\0' && buf[i] == ' ')
		i++;
	j = 0;
	while(buf[i] != '\0'){
		if(buf[i] != ' '){
			purser[1][j] = buf[i];	
		}else
			break;
		i++;j++;
	}
	purser[1][j] = '\0';
	if(strcmp(purser[0],"export") == 0){
		for(int k = 0; k < *paragraph_count; k++){
				if(strcmp(statement[k][0],purser[1]) == 0)
					setenv(purser[1],statement[k][1],1);
			}
	}
}

