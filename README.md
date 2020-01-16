
    Name: Yousef Shaheen
    Login: yousef
    Project: p2a
    Name of this file: wish.c
    
    
    This program implements a shel called wish (Wisconsin Shell)
    Note: the program is built using the Makefile, and
    it has to be built with -lm to link math.h library
    
    Usage:

	it can be invoked from the console with one or zero arguments. eg: 
		-$ wish 
	or 	
		-$ wish batch.txt

	 // it exits if the user has run the shell wih too many arguments

	  // if there is one argument, run in batch-mode
		if the user entered the wrong file, or it didnt open successfuly,
		it exits.

	  // otherwise, enter the interactive mode, which supports linux commands (ls, cd, pwd, etc)
	  and has four built in commands:
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

	// When it is not built-in command, 
	the program forks a child process to execute the command.
	It runs the command, or prints error if it doesn't exist.
	
	// it can redirect output
	// but pipe is still under development

