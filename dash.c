#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<ctype.h>

char ERROR_MESSAGE[30] = "An error has occurred\n";
void error();
FILE *file = NULL;

void displayPrompt(){
	printf("dash> ");
}



int getInput(char cmd[], char *parameters[], int batchFlag){
	char *buffer;
	size_t size = 1024;
	char *tok;
	char *array[20];
	
	// allocate space for input buffer
	buffer = (char *)malloc(size * sizeof(char));
	
	// check to see if batch file is being read
	if(batchFlag == 1){
		 if(feof(file)){
                        free(buffer);
			parameters[0] = "exit";
			for(int j = 1; j < 20; j++)
				parameters[j] = "exit";
                        return 1;
                }
		
		 printf("in batch flag\n");

		if(getline(&buffer, &size, file) != -1 ){
			printf("Buffer: %s\n", buffer);
			
		}
		else
			error();
	}
	else{
		// read from stdin
		if(getline(&buffer, &size, stdin) == -1)
			error();
	}
	
	// check if user entered newline, space, or tab
	if(strcmp(buffer, "\n") == 0 || strcmp(buffer, " \n") == 0 || strcmp(buffer, "\t\n") == 0){
		free(buffer);
		if( write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)) == -1);
			
		return -1;
	}

	strcpy(cmd, buffer);
	
	// tokenize command
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
	
	// implement cd
	 if(strcmp(parameters[0],"cd") == 0){
                                if(i==2){
                                        if(chdir(parameters[1])!=0){
                                               if( write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)));
                                        }
                                 }

                        }

	 

	

	free(buffer);
	return 1;
}

void error(){
	if(write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)));
        exit(1);
}



int main(int argc, char* argv[]){
	
	char cmd[100], *cmdPath, *parameters[20];
	char *envp[] = {(char *) "PATH=/bin", 0};	
	int batchFlag = 0;

	// check if multiple arguments were passed when calling DASH
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
	
	
	// command is stored in cmd and parameters in parameters
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
		
		int i = 0;
		for(int j = 0; j<20; j++){
			if(parameters[j] != NULL)
				i++;
		}	
		if(strcmp(cmd, "exit") == 0)
				break;
		else if(strcmp(cmd,"path") == 0){
                               
                                if(i==2){
                                        
                                        cmdPath=strdup(parameters[1]);
                                         if(cmdPath[strlen(cmdPath)-1]!='/'){
                                                strcat(cmdPath,"/");
                                        }      
                                }
		}
                             

	}
	return 0;
		
}
