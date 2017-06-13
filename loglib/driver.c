/* $Author: o3-lael $
 * $Date: 2016/02/13 18:56:06 $
 * $Log: driver.c,v $
 * Revision 1.13  2016/02/13 18:56:06  o3-lael
 * fixed opterr omission to prevent duplicate error reporting on invalid flag
 *
 * Revision 1.12  2016/02/13 18:44:12  o3-lael
 * enhanced getopt()
 *
 * Revision 1.11  2016/02/11 21:03:56  o3-lael
 * fixed string malloc issue
 *
 * Revision 1.10  2016/02/09 15:57:28  o3-lael
 * added more comments and formatted terminal output
 *
 * Revision 1.9  2016/02/09 03:16:23  o3-lael
 * changed include statements to reflect archive use
 *
 * Revision 1.5  2016/02/03 20:26:39  o3-lael
 * fixed command length issue
 *
 * Revision 1.3  2016/02/03 03:12:32  o3-lael
 * added message for printing empty log
 *
 * Revision 1.2  2016/02/03 01:35:13  o3-lael
 * fixed includes
 *
 * Revision 1.1  2016/02/02 22:30:39  o3-lael
 * simple driver program for loglib.c
 *
 * $State: Exp $
 * $Revision: 1.13 $
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "loglib.c"

extern int errno;

/* function to run "buggy code" exit status of command is stored in errnum
 * and used to provide detailed error string */
int run (char* command){
	/* prepare timestamp amd allocate space for associated string */
	data_t current;
	current.string = command;
	current.time = time(NULL);
	char *tmp = malloc(strlen(command) + 10);
	int errnum = 0;

/* each part of the following if/then builds a string to store in
 * a data_t element. Which string is built depends upon the exit 
 * status of the command run */
	//system indicates command other than Success
	if(system(command) != 0){
	errnum = system(command);
	strcpy(tmp,"");
	strcat(tmp, command);
	strcat(tmp, ": Error: ");
	tmp = realloc(tmp, strlen(tmp) + strlen(strerror(errnum)));
	strcat(tmp, strerror(errnum));
	current.string = tmp;
	}
	/* system indicated Success. 
 	* Since success, no "Error: " in message string */
	else{
	strcpy(tmp,"");
	strcat(tmp, command);
	strcat(tmp, ": ");
	tmp = realloc(tmp, strlen(tmp) + strlen(strerror(errnum)));
	strcat(tmp, strerror(errnum));
	current.string = tmp;
	}
/* add the message/timestamp that was just built to the linked list */
return addmsg(current);
}

/* function to display log contents
 * this function handles empty log with appropriate message
 * oterwise, getlog() is used to retrieve the log as a single string
 * before printf() displays on the console */
void displaylog(void){
	char* logContents = getlog();
	if(getlog() == NULL)
	printf("Log is empty. Nothing to output.\n");
	else
	printf("%s\n", logContents);
}

int main( int argc, char **argv){

	opterr = 0;
	int flag = 0;
	int opt;
	
	/* getopt() used to provide command line switch 
 	* which is later used to display the README if desired */
	while ((opt = getopt(argc, argv, "hu")) != -1){
		switch(opt){
			case 'h':
				flag = 1;
				break;
			case 'u':
				printf("Usage:\n");
				printf("./driver	- Runs the program with default settings\n");
				printf("./driver -h	- Displays the README file\n");
				printf("./master -u	- Displays usage information\n");
			case'?':
				if(isprint(optopt)){
					printf("Invalid option -%c\n", optopt);
					printf("Use ./driver -u to display usage information\n");
				}
			exit(1);
		}
	}
	
	/* if getopt() catches -h flag on command line, display README */
	if(flag == 1){
		run("cat README");
		return 0;
	}
/* perform actions to test logging capabilities and display progress on terminal */	
	printf("**************************************************\n");
	printf("Running \"Buggy Code\"...\n");
	printf("**************************************************\n");
	printf("Running disk usage command \n");
	printf("**************************************************\n");
	run("du -ah | sort -rh | head -10");
	printf("**************************************************\n");
	printf("Running list command \n");
	printf("**************************************************\n");
	run("ls -la");
	printf("**************************************************\n");
	printf("Running print working directory command \n");
	printf("**************************************************\n");
	run("pwd");
	printf("**************************************************\n");
	printf("Displaying log contents from memory after initial run...\n");
	printf("**************************************************\n");
	displaylog();
	printf("**************************************************\n");
	printf("Saving log to disk file (o3-lael.log)...\n");
	printf("**************************************************\n");
	savelog("o3-lael.log");
	printf("**************************************************\n");
	printf("Clearing log elements from memory...\n");
	printf("**************************************************\n");
	clearlog();
	printf("**************************************************\n");
	printf("Displaying log after clearing linked list...\n");
	printf("**************************************************\n");
	displaylog();
	printf("**************************************************\n");
	printf("Running more \"Buggy Code\"...\n");
	printf("**************************************************\n");
	printf("Running make directory command that should produce error\n");
	printf("**************************************************\n");
	run("mkdir ../../../frogger");
	printf("**************************************************\n");
	printf("Saving log to disk file (o3-lael2.log)...\n");
	printf("**************************************************\n");
	savelog("o3-lael2.log");
	printf("**************************************************\n");
	printf("Displaying log after secondary run...\n");
	printf("**************************************************\n");
	displaylog();
return 0;
}
