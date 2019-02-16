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
  //int argsCount;
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

      // exit if reached the end of file/i.e. stdin == (Ctrl + D)
      if(feof(stdin)){
        exit(0);
      }

      // otherwise get the input and store it in 'line'
      getline(&line, &len, stdin);

      // first built-in command, exit. check if user typed exit
      if((ptr = strstr(line, "exit")) != NULL){
        if(strlen(line) != 5) // its error to type exit w/arguments
          write(STDERR_FILENO, error_message, strlen(error_message));
        else
          exit(0);
      }

      // also exit if reached the end of file/i.e. stdin == (Ctrl + D)
      else if(feof(stdin)){
        exit(0);
      }

      // if not a built-in command, fork a child process to execute command
      else{
        int rc = fork();
	// now in the child process, if negative then fork failed
        if(rc < 0){
          write(STDERR_FILENO, error_message, strlen(error_message));
          exit(1);
        }
        else if(rc == 0){
          // this is the new child process
          // get the first token, i.e. the command to be executed 
          token = strtok(line, s);

          // allocate memory in path varialbe to hold the command full path
          path = malloc(strlen(defPath) + strlen(token));

          // path now has the full path for the command (along with default path)
          strcpy(path, defPath);
          strcat(path, token);

          // allocate memory for args array(which is to be passed to execv)
          // initially, it has only one slot to hold the command's path
          char **args = (char**)malloc(sizeof(char*) * 1);
          *args = malloc(sizeof(char) * strlen(path));
          // now add the command's full path to the first element of args
          strcpy(*args, path);

          // read the rest of tokens, reallocate memory in the args array
          // and allocate memory for each token, then add it to the array
          for(int j = 1; ; j++){
            // get the next token
            token = strtok(NULL, s);
            // enlarge args-array by one element
            args = (char**)realloc(args, (sizeof(char*) * (j + 1)));

            // if the token is NULL, add it to end of array and break
            if(token == NULL){
              *(args + j) = NULL;
              break;
            }
            // otherwise, allocate mem in the args-array and add token
            *(args + j) = (char*)malloc(sizeof(char) * strlen(token));
            *(args + j) = strdup(token);
          }

          // test if the command exist in the current search path
          if(access(*args, X_OK) != 0)
            write(STDERR_FILENO, error_message, strlen(error_message));
          else{
            execv(*args, args);
          }
        }
        else{
          // in parent process, waiting for child to terminate
          wait(NULL);
        }
      }
    }
  }
  return 0;
}

