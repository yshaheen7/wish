/**
 *    Name: Yousef Shaheen
 *    Login: yousef
 *    Project: p2a
 *    Namf of this file: wish.c
 *
 *    This file is the program that implement a shel called wish
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int
main(int argc, char *argv[])
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  char *ptr = NULL;
  ssize_t nread;
  const char s[20] = "/' ''\t''\n'";
  char *token;
  char defPath[] = "/bin/";
  char *path;
  char error_message[30] = "An error has occurred\n";
   
  // exit if the user has run the shell wih too many arguments
  if(argc > 2){
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }

  // if there is one argument, run in batch-mode
  if(argc == 2){
    fp = fopen(argv[1], "r");
    if(fp == NULL){
      printf("error: cant open file\n");
        exit(0);
    }
    while((nread = getline(&line, &len, fp)) != -1){
      printf("%s", line);
    }
    if(feof(fp)) 
      exit(0);
    }
    
  // otherwise, enter the interactive mode
  else{
    while(1){
      printf("wish> "); // print the prompt for the user
      getline(&line, &len, stdin);
      
      // first built-in command, exit
      /*if((ptr = strstr(line, "exit")) != NULL){
        if(strlen(line) != 5) // its error to type exit w/argyment
          write(STDERR_FILENO, error_message, strlen(error_message));
        else
          exit(0);
      }*/

      int rc = fork();
      if(rc < 0){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
      }
      else if(rc == 0){
        // this is new child process
	printf("in child process now\n");
 
        token = strtok(line, s);
       // char **args = malloc(sizeof(char*) * 10);
       // for(int i = 0; i < 10; i++){
       //   args[i] = malloc(sizeof(char) * 15);
       // }
       	printf("token is:%s with length %ld", token, strlen(token));
        path = malloc(strlen(defPath) + strlen(token));
        char **args = malloc(sizeof(char *) * 2);
	strcpy(path, token);
	strcpy(path, defPath);
	printf("path is:%s and length is: %ld\n", path,strlen(path));
        strcat(path, token);
	 printf("path is now:%s with length%ld\n", path, strlen(path));
     
	args[0] = malloc(strlen(path)); 
	strcpy(args[0], path);
 printf("args[0] is %s with length%ld\n", args[0], strlen(args[0])); 
        /*for(int j = 1; ; j++, token = strtok(NULL, s),
                               args[j] = strdup(token)){
          if(token == NULL){
            args[j] = NULL;
            printf("now breaking out of the loop\n");
	    break;
	  }
	  printf("token:%s\n", token);

        }*/
	/*
	int counter = 0;
	while(token != NULL){
          token = strtok(NULL, s);
	  counter++;
	  args[counter] = strdup(token);
	  printf("%s\n", args[counter]);
	}
	token = strtok(NULL, s);
	 printf("1\n");
	args[1] = strdup(token);
	printf("2\n");

	token = strtok(NULL, s);
	printf("3\n");

//        args[2] = strdup(token);
//	printf("4\n");
*/
	args[1] = NULL;
	printf("2\n");

char *argu[2];
argu[0] = "/bin/ls";
argu[1] = NULL;
int n;
	for(int i = 0; i < 2; i++){
          printf("args[%d] is:%s\n", i, args[i]);
        }
	printf("argu[0] is:%s\n", argu[0]);
	if(access(argu[0], X_OK) == -1)
          printf("command doesn't exist in path\n");
if((n = strcmp(argu[0], args[0])) != 0){
	printf("string do not mathch, value is:%d\n",n);
	printf("args length:%ld", strlen(args[0]));
	printf("argu length:%ld", strlen(argu[0]));
		}
	
	else{
          printf("now calling ls function\n");
          execv(argu[0], argu);
	}
      }
      else{
	printf("now waiting\n");
        wait(NULL);
	printf("now in parent process\n");
      }

      // if user typed exit, then exit the shell
      if((ptr = strstr(line, "exit")) != NULL){
        if(strlen(line) != 5) // its error to type exit w/argyment
          write(STDERR_FILENO, error_message, strlen(error_message));
        else
          exit(0);
      }
      // if reached the end of file/ stdin == (Ctrl + D)
      if(feof(stdin)) 
        exit(0);   
    }
  }
  return 0; 
}

