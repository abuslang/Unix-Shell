#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

char ERROR_MESSAGE[30] = "An error has occurred\n";

void displayPrompt(){
	printf("dish> ");
}

void getInput(char cmd[], char *parameters[]){
	char *buffer;
	size_t size = 1024;
	char *tok;
	char *array[20];

	buffer = (char *)malloc(size * sizeof(char));
	getline(&buffer, &size, stdin);
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
}

void error(){
	write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
}

int main(){
	
	char cmd[100], cmdPath[100], *parameters[20];
	char *envp[] = {(char *) "PATH=/bin", 0};
//	printf("dish> ");

/*	
	// get command and parameters
	getInput(cmd, parameters);
	printf("Printing from main\nCommand: %s\n", cmd);
	
	for(int i = 0; i < 20; i++){
		if(parameters[i] == NULL)
			break;
		else
			printf("Parameter: %s\n", parameters[i]);
	}

	// ok so now we have the command stores in cmd and parameters in parameters
	
*/

	while(1){
		
		displayPrompt();
		getInput(cmd, parameters);
	

		if(fork() != 0)
			wait(NULL);
		else{
			strcpy(cmdPath, "/bin/");
			strcat(cmdPath, cmd);
	
			if(strcmp(cmd, "cd") != 0 && strcmp(cmd, "exit") != 0){
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







