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
#include <math.h>
#include <ctype.h>
int
main(int argc, char *argv[])
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  char *ptr = NULL;
  ssize_t nread;
  const char s[20] = "/' ''\t''\n'";
  const char s2[10] = "' ''\t''\n'"; // delim for cd, wich ignores /
  char *token;
  char defPath[] = "/bin/";
  char *path;
  int histCount = 0;
  double histArg;
  char **history;
  char error_message[30] = "An error has occurred\n";
  int cdArg;
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
    // allocate memory for the array of strings to store the history
    history = (char**)malloc(sizeof(char*));

    // now in the interactive mode
    while(1){
      printf("wish> "); // print the prompt for the user

      fflush(stdin);
      fflush(stdout);

      // get the user input and store it in 'line'
      getline(&line, &len, stdin);

      // first built-in command, exit. check if user typed exit
      if((ptr = strstr(line, "exit")) != NULL){
        if(strlen(line) != 5) // its error to type exit w/arguments
          write(STDERR_FILENO, error_message, strlen(error_message));
        else{
          exit(0);
        }
      }

      // to avoid prinnting 'wish> ' before exiting when stdin == (Ctrl + D)
      else if(feof(stdin)){
        printf("\n");
        exit(0);
      }

      // allocate memory to hold line in history
      *(history + histCount)= (char*)malloc(sizeof(char) * strlen(line));
      // add the current line to history
      *(history + histCount) = strdup(line);
      // increment history counter and reallocate memory for next line
      histCount++;
      history = (char**)realloc(history, (sizeof(char*) * (histCount + 1)));

      // second built-in command, cd. check if user typed cd
      if((ptr = strstr(line, "cd")) != NULL){
        // get the first token, i.e. the command, and ignor it
        token = strtok(line, s2);

        // get the next token, the path (needs to be a full path or else 
        // the chdir will fail, also, if the path is empty thorw an error
        if((token = strtok(NULL, s2)) == NULL){
          write(STDERR_FILENO, error_message, strlen(error_message));
        }
        // now call the chdir syscall with user specified path in 'token'
        else if((cdArg = chdir(token)) != 0){      
          write(STDERR_FILENO, error_message, strlen(error_message));
        }
      }

      // third built-in command, history. check if user typed history
      // get the first token, i.e. the command, and check syntax
      else if((ptr = strstr(line, "history")) != NULL 
          && strlen(token = strtok(line, s)) == 7){
        // get the next token
        token = strtok(NULL, s);

        // if there is no argument, print all history
        if(token == NULL){
          for(int i = 0; i < histCount; i++){
            printf("%s", *(history + i));
          }
        }

        // if there is an argument, but it is not a number throw an error
        else if((strcmp(token, "0")) != 0 && (histArg = atof(token)) == 0){
          write(STDERR_FILENO, error_message, strlen(error_message));
          //histArg = (int)atoi(token);
        }

        // if the argument is a number, then round it up and print
        // last 'number' of history lines, according to 
        // number specified by user
        else{
          histArg = atof(token);
          // if there is more than one argument, throw an error
          if((token = strtok(NULL, s)) != NULL){
            write(STDERR_FILENO, error_message, strlen(error_message));
          }

          // if value is not positive then do not do anyting
          else if(histArg <= 0){
          }

          else{ 
            histArg = ceil(histArg);
            // if the number specified is greater than history element,
	    // print the whole history
            if(histArg >= histCount){
              for(int i = 0; i < histCount; i++){
                printf("%s", *(history + i));
              }
            }
            else{
              int count = histCount - histArg;
              for(int i = count; i  <  histCount; i++){
                printf("%s", *(history + i));
              }
            }
          } 
        }
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
          if(access(*args, X_OK) != 0){
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
          }
          else{
            execv(*args, args);
          }
        }  
        else{
          // in parent process, waiting for child to terminate
          wait(NULL);
          // check if user typed exit, and exit if true
          if((ptr = strstr(line, "exit")) != NULL){
            if(strlen(line) != 5) // its error to type exit w/arguments
              write(STDERR_FILENO, error_message, strlen(error_message));
            else{
              exit(0);
            }
          }

          // also exit, if stdin == (Ctrl + D)
          else if(feof(stdin)){
            printf("\n");
            exit(0);
          }
        }
      }
    }
  }
  return 0;
}

