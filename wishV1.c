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

int
main(int argc, char *argv[])
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  char *ptr = NULL;
  ssize_t nread;

  // exit if the user has run the shell wih too many arguments
  // NEED TO FIX this for final value of the the input that the suser will acutally input when they launch the shell
  if(argc > 2){
    printf("Usage: arguments are more than accepted\n");
    exit(0);
  }

  while(1){
    // if there is an argument, run in batch-mode
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
      printf("prompt> "); // print the prompt for the user
      getline(&line, &len, stdin);

      printf("%s", line);
    }
    // if reached the end of file/ stdin == (Ctrl + D), OR
    // if user typed exit, then exit the shell
    if(feof(stdin) || ((ptr = strstr(line, "exit")) != NULL))
      exit(0);   
   }
}

