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
  char *s = "/;.,' ''\t''\n'";
  char *s2 = "' ''\t''\n'"; // delim for cd, which ignores '/'
  char *token;
  char *tempPath;
  char **path;
  int histCount = 0;
  double histArg;
  char **history;
  char **args;
  char error_message[30] = "An error has occurred\n";
  int cdArg;
  int slash;
  char *strPd1 = ";.' ''\t''\n'";
  char *strPd2 = "/";
  char *subtoken;
  char *strPtr1, *strPtr2;

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

    // allocate memory for the path, to hold search paths and set it to 
    // '/bin/' by default
    path = (char**)malloc(sizeof(char*));
    *path = (char*)malloc(sizeof(char) * strlen("/bin"));
    *path = "/bin";

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
      // add the current line to history, as long as it is not empty line
      if(*line != '\n')
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

      // fourth built-in command, path. It changes the 
      // current path to whatever is specified by user
      // if nothing is specfified, then path is empty
      // and only built in commands can be executed
      else if((ptr = strstr(line, "path")) != NULL
                 && strlen(token = strtok_r(line, strPd1, &strPtr1)) == 4){
        // get the next token
        token = strtok_r(NULL, strPd1, &strPtr1);

        // empty the courrent path arry of all elements
	for(int i = 1; ;i++){
          if(*(path + i) == NULL)
            break;
          free(*(path + i));
        }
        free(path);

        // allocate memory for the path, to hold search path/s
        path = (char**)malloc(sizeof(char*));
 
        // if path is called with no args, set the search path to empty
        if(token == NULL)
          *path = NULL;

        // else, get paths one by one, and store them in the path's array
        else{
          for(int i = 0; ; i++, token = strtok_r(NULL, strPd1, &strPtr1)) {
            // if no more tokens, break from the loop and set the end of
	    // array
            if(token == NULL){
              *(path + i) = NULL;
              break;
            }

            // else, increase the size of the array to hold an element
            path = (char**)realloc(path, (sizeof(char*) * (i + 2)));
            // and allocate memory for the path string in the token
            // and set it to an empty string 
            *(path + i) = (char*)malloc(sizeof(char) * strlen(token));
            strcpy(*(path + i), "");

            // parse through the input of the user, and allign the path 
            // one token at a time
            for(int j = 0; ; j++, token = NULL){
              subtoken = strtok_r(token, strPd2, &strPtr2);
              if(subtoken == NULL || (slash = strcmp(subtoken, "/")) == 0){
              //  strcat(*(path + i), "/");
                break;
              }

              strcat(*(path + i), "/");
              strcat(*(path + i), subtoken);
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

          // allocate memory for args array(which is to be passed to execv)
          // initially, it has only one slot to hold the command's path
          args = (char**)malloc(sizeof(char*) * 1);
          *args = malloc(sizeof(char) * strlen(token));

          // now add the command to the first element of args
          *args = strdup(token);

          // allocate memory in tempPath varialbe to hold the command's
          // full path
          tempPath = (char*)malloc(strlen(*path) + strlen(token));

          // add the full path for the command in tempPath
          strcpy(tempPath, *path);
	  strcat(tempPath, "/");
          strcat(tempPath, token);

          // read the rest of tokens, reallocate memory in the args array
          // and allocate memory for each token, then add it to the array
          for(int j = 1; ; j++){
            // get the next token
            token = strtok(NULL, s2);
            // enlarge args-array by one element
            args = (char**)realloc(args, (sizeof(char*) * (j + 2)));
		
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
          if(access(tempPath, X_OK) != 0){
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
          }

          else{
            execv(tempPath, args);
            write(STDERR_FILENO, error_message, strlen(error_message));
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
	  else{
            fflush(stdin);
            fflush(stdout);
	  }
        }
      }
    }
  }

  // free memroy allcated by malloc
  // for(i = 0; ; i++){

  return 0;
}

