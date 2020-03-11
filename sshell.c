/*
   Name: Alex Rigl
   ID: 2300146
   Class: Operating Systems
   Assignment: #2
   Email: crigl@chapman.edu

   Project Description:
   The simple shell program (sshell.c) is a simple text-based program that takes user input from
   the parent then sends that input to the child process for execution. Both the parent and
   child process should loop waiting on either user input (parent) or pipe input (child).
   The program should run until the user types ‘quit’ or ‘q’ at the command prompt.

   References:
   - fork script from BlackBoard

   Worked with:
   - Tristan Chilvers

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{

	char write_msg[BUFFER_SIZE];
	char read_msg[BUFFER_SIZE];

	/* Identifies the fork */
	pid_t pid;
	int fd[2];

	/* Strings to check if user wants to quit*/
	char qString[3] = "q\n";
	char quitString[6] = "quit\n";

	/* Create the Pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	/*	Fork the child */
	pid = fork();

	/* Error Checking */
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	/*
    Parent Block Waiting on User Input
      Close the unused end of the pipe
  */
	if (pid > 0) {
		close(fd[READ_END]);
		printf("osh> ");

		/*
      Repeat steps until 'q' or 'quit' is typed by the user
        Block waiting on user input
        Then write the user input
    */
		while((strcmp(write_msg, qString) != 0) && (strcmp(write_msg, quitString) != 0))
		{

			fgets(write_msg, BUFFER_SIZE, stdin);

			write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		}

		// Close the write end of the pipe when exiting
		close(fd[WRITE_END]);
	}

	/*
    Child Process Utilizing User Input:
      Close the unused (write) end of the pipe
      Then read input from parent
  */
	else {
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);

		/*
      Loop program until the input is "q" or "quit":
        Run the input from user
        Clear the output
        Then Read the input from the parent
    */
		while((strcmp(read_msg, qString) != 0) && (strcmp(read_msg, quitString) != 0))
		{
			system(read_msg);
			printf("osh> ");

			fflush(stdout);

			read(fd[READ_END], read_msg, BUFFER_SIZE);
		}

		/* Close the write end of the pipe */
		close(fd[READ_END]);
	}

	return 0;
}
