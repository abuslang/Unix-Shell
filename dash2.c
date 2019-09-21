#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<ctype.h>

char ERROR_MESSAGE[30] = "An error has occurred\n";
void error();

void displayPrompt(){
	printf("dish> ");
}

FILE *file = NULL;

int getInput(char cmd[], char *parameters[], int batchFlag){
	char *buffer;
	size_t size = 1024;
	char *tok;
	char *array[20];

	buffer = (char *)malloc(size * sizeof(char));
	
	if(batchFlag == 1){
		 if(feof(file)){
                        free(buffer);
                        return -1;
                }

		getline(&buffer, &size, file);
	}
	else
		getline(&buffer, &size, stdin);
	
	
	
	if(strcmp(buffer, "\n") == 0 || strcmp(buffer, " \n") == 0 || strcmp(buffer, "\t\n") == 0){
		free(buffer);
		 write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
		return -1;
	}

	strcpy(cmd, buffer);
		
	tok = strtok(buffer, " \n");
	
	int i = 0;
	while(tok != NULL){
		array[i++] = strdup(tok);
		tok = strtok(NULL, " \n");
	}
	
	strcpy(cmd, array[0]);
	for(int j=0; j < i; j++){
		parameters[j] = array[j];	
	}
	parameters[i] = NULL;
	
	 if(strcmp(parameters[0],"cd") == 0){//cd
                                if(i==2){
                                        if(chdir(parameters[1])!=0){
                                                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                                                printf("In input fx\n");
                                        }
                                 }

                        }
	

	free(buffer);
	return 1;
}

void error(){
	write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
}

void redirect(char *parameters[]){
	
	for(int i = 0; i < 20; i++){
		if(parameters[i] == NULL)
			return;
		else if(strcmp(parameters[i], ">") == 0){

		}
	}
}

int main(int argc, char* argv[]){
	
	char cmd[100], cmdPath[100], *parameters[20];
	char *envp[] = {(char *) "PATH=/bin", 0};	
	int batchFlag = 0;
	if(argc == 1){
		file=stdin;
	}
	else if(argc == 2){
		batchFlag = 1;
		char *bFile = strdup(argv[1]);
		file = fopen(bFile, "r");
		if(file == NULL) {
			error();
		}
	
	}
	
	
	// ok so now we have the command stores in cmd and parameters in parameters
	

	while(1){
		int inputReturn = 0;
		if(batchFlag != 1)
			displayPrompt();
		inputReturn = getInput(cmd, parameters, batchFlag);
	

		if(fork() != 0)
			wait(NULL);
		else{
			strcpy(cmdPath, "/bin/");
			strcat(cmdPath, cmd);
	
			if(inputReturn != -1 &&  strcmp(cmd, "cd") != 0 && strcmp(cmd, "exit") != 0){
			if(execve(cmdPath, parameters, envp) == -1){
				error();
				exit(0);
			}
			}
			else
				exit(0);
		}
			
		if(strcmp(cmd, "exit") == 0)
				break;

                             

	}
	return 0;
	
}
