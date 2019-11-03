
    Name: Yousef Shaheen
    Login: yousef
    Project: p2a
    Namf of this file: wish.c

    This program implement a shel called wish
    the program is built using the Makefile
    it has to be built with -lm to link math.h

it can be invoked with one or zero arguments.

  // exit if the user has run the shell wih too many arguments

  // if there is one argument, run in batch-mode
	if the user entered the wrong file, or it didnt open successfuly,
	it exits.
    
  // otherwise, enter the interactive mode,which has four built in commands:
      // first built-in command, exit. exits the shell when the user type exit
	or hit Ctrl+D

      // second built-in command, cd. changes the current directory 

      // third built-in command, history. it takes zero or one argument.
	it returns the history of lines entered by the user, or the last
	'number' specified by the user. if number is greater than the count
	of total lines, then return all lines.

      // fourth built-in command, path. It changes the 
      // current path to whatever is specified by user
      // if nothing is specfified, then path is empty
      // and only built in commands can be executed

      // if not user typed a no built-in command, 
	the program forks a child process to execute command
	it runs the command, or prints error if it doesn't exist.
	it can redirect output and pipe output to a different command.
	
