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
  const char s[20] = "/' ''\t'";
  char *token;
  char path[250] = "/bin/";
  char error_message[30] = "An error has occurred\n";
   
  // exit if the user has run the shell wih too many arguments
  // NEED TO FIX this for final value of the the input that the suser will acutally input when they launch the shell
  if(argc > 2){
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }

  while(1){
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
      printf("wish> "); // print the prompt for the user
      getline(&line, &len, stdin);
      //command  = malloc(len);
      //fork()
      
      int rc = fork();
      if(rc == 0){
        // this is new child process
	printf("in child process now\n");
 
        token = strtok(line, s);
        char **args = malloc(sizeof(char*) * 10);
        for(int i = 0; i < 10; i++){
          args[i] = malloc(sizeof(char) * 15);
        }
        args[0] = strdup(strcat(path, token));
        
        for (int j = 1; ; j++, token = strtok(NULL, s)){
          if(token == NULL){
            args[j] = NULL;
	    break;
	  }
	 // printf("token:%s\n", token);
	  args[j] = strdup(token);
          //printf("path:%s", path);
          //token = strtok(NULL, s);     
	//printf("path: %s", command);

        }
	if(access(args[0], X_OK) == -1)
          printf("command doesn't exist in path\n");

//	arguments[0] = "-la";
//	arguments[1] = NULL;
//       	}
	/*//
	for(int i = 0; ; i++){
          if(args[i] == NULL)
            break;
          printf("%s\n", args[i]);
	}
        // if reached the end of file/ stdin == (Ctrl + D), OR
        // if user typed exit, then exit the shell
        if(feof(stdin) || ((ptr = strstr(line, "exit")) != NULL))
          exit(0);   
      }
      else{
        int rc_wait = wait(NULL);
        printf("in parent process now (rc_waitID:%d)\n", rc_wait);
      }
      */ 
      // if reached the end of file/ stdin == (Ctrl + D), OR
      // if user typed exit, then exit the shell
      if(feof(stdin) || ((ptr = strstr(line, "exit")) != NULL))
        exit(0);   
      }
    }
  }
}

