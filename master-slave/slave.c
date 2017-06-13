/* $Author: o3-lael $
 * $Date: 2016/02/20 15:43:29 $
 * $Log: slave.c,v $
 * Revision 4.1  2016/02/20 15:43:29  o3-lael
 * final branch ready to turn in
 * cleaned up punctuation in messages
 *
 * Revision 3.6  2016/02/19 22:35:22  o3-lael
 * set log writes to unbuffered so they go in in time order rather than process order
 *
 * Revision 3.5  2016/02/19 20:14:59  o3-lael
 * checked to make sure slave wasn't being called stand-alone
 *
 * Revision 3.4  2016/02/19 17:47:36  o3-lael
 * altered Peterson Algorithm to make turn passing mor random
 * changes are explained in comments and README
 *
 * Revision 3.3  2016/02/19 16:37:38  o3-lael
 * endless tinkering with comments, terminal output and timer placement
 *
 * Revision 3.2  2016/02/19 01:17:27  o3-lael
 * fixed timestamp issue in slave.c
 * now ready for release
 *
 * Revision 3.1  2016/02/19 00:58:31  o3-lael
 * Branch to release
 *
 * Revision 2.4  2016/02/18 22:48:30  o3-lael
 * added timestamp to enter/exit of CS
 *
 * Revision 2.3  2016/02/18 18:54:04  o3-lael
 * general tune up and comments
 * also fixed message printed to file from processes to match specified time format
 *
 * Revision 2.2  2016/02/18 16:48:04  o3-lael
 * removed fcloseall() causing compilation warnings
 * replaced with exit(0[1])
 *
 * Revision 2.1  2016/02/18 14:57:52  o3-lael
 * Branch
 * Program is functionally complete
 * Now it is time to clean up and fine tune
 *
 * Revision 1.13  2016/02/18 04:05:47  o3-lael
 * segmented peterson algorithm to facilitate 3 writes per process
 * added signal messaging for process kill from master
 * edited messaging to output file
 *
 * Revision 1.12  2016/02/17 21:48:38  o3-lael
 * timestamp working.
 *
 * Revision 1.10  2016/02/17 04:48:08  o3-lael
 * poking at mutex still
 *
 * Revision 1.9  2016/02/17 03:56:13  o3-lael
 * everything is in place and working except for peterson block
 * code is there, but critical section is not executing
 *
 * Revision 1.8  2016/02/16 22:04:25  o3-lael
 * added base code for testing and ironing out execlp functionality
 * random timers added for testing
 * mutex not working yet and processes are sequentially at this time
 *
 * Revision 1.7  2016/02/16 03:07:31  o3-lael
 * text tweaks
 *
 * Revision 1.6  2016/02/13 22:49:53  o3-lael
 * fixed a comment marker
 *
 * Revision 1.5  2016/02/13 22:01:00  o3-lael
 * fixed while parens per Mark
 *
 * Revision 1.4  2016/02/13 14:37:42  o3-lael
 * added bare layout
 *
 * Revision 1.3  2016/02/12 22:36:11  o3-lael
 * added peterson's section
 *
 * Revision 1.2  2016/02/12 21:56:23  o3-lael
 * added RCS keywords
 *
 * $State: Exp $
 * $Revision: 4.1 $
*/
#include "slave.h"

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr,"\n*******************************************************************************\n");
		fprintf(stderr, "* %s is intended to be called by master with arguments passed from master. *\n", argv[0]);
		fprintf(stderr,"*******************************************************************************\n\n");
	exit(1);
	}
	// install signal handler
	signal(SIGINT, ctrlCHandler);
	// seed random
	srand(time(&t));

	/* Below passed from master via execlp arguments */
	// shared memory segment number
	int myShmemId = atoi(argv[1]);
	// child number (0-18)
	int who = atoi(argv[2]);
	// child_pid
	int myID = atoi(argv[3]);
	// number of slaves
	int slaves = atoi(argv[4]);
	// filename
	if(argv[5] != NULL)
		filename = argv[5];
	/* End passed arguments */
	logname = fopen(filename, "a");
	// make log writes immediately - no buffering (seems to have a big hit on running time)
	setbuf(logname, NULL);

	// attach child process to shared memory and report status
	if((sFlag = (int *)shmat(myShmemId, NULL, 0)) == (void *) -1){
    		perror("Failed to attach shared memory segment of flag array.\n");
    	return 1;
  	}
	else
		printf("Child: %i with PID: %i now attached to shared memory segment: %i.\n", (who + 1), myID, myShmemId);
	
	// This is where the magic happens
	do{
		// limit writes to log to 3 (max_writes in assignment handout)
		// this could be easily configurable by replacing i with a var
		// but wasn't in the scope of this project
		do{
			// setup mutual exclusion and execute CS
			setup_exc(sFlag, who, slaves, j, turn, logname);
			// handoff trun to someone else
			handoff(sFlag, who, slaves, j, turn);
			i++;
			printf("Process %i with PID %i says Hello from the \"Remainder Section\".\n", (who + 1), myID);
		}while(i < 3);
	return 0;
	}while(1);
return 0;
}
//Critical Section code 
int critical_sec(FILE *logname, int who){
	// get time and create timestamp for log entry
	time_t  current_time;
	struct tm *tmp;
	char timestamp[9];
	// get current time
	current_time = time(NULL);
	tmp = localtime(&current_time);
	// clear contents of timestamp
	memset(&timestamp[0], 0, sizeof(timestamp));
	// build formatted string from timestamp HH:MM 24 Hour for readability
	strftime(timestamp, sizeof(timestamp), "%T", tmp);
	// Notify of process' entrance into CS and sleep random up to 2 sec
	fprintf(stderr, "Process %d entered critical section at %s.\n", (who + 1), timestamp);
	// sleep from 0 - 2 seconds randomly
	sleep(rand() % 3);
	// get current time
	current_time = time(NULL);
	tmp = localtime(&current_time);
	// clear contents of timestamp
	memset(&timestamp[0], 0, sizeof(timestamp));
	// build formatted string from timestamp HH:MM 24 Hour for readability
	strftime(timestamp, sizeof(timestamp), "%T", tmp);
	// print to log and sleep random up to 2 sec
	fprintf(logname, "File modified by process number %i at time %s.\n", (who + 1), timestamp);
	// sleep from 0 - 2 seconds randomly
	sleep(rand() % 3);
	// get current time
	current_time = time(NULL);
	tmp = localtime(&current_time);
	// clear contents of timestamp
	memset(&timestamp[0], 0, sizeof(timestamp));
	// build formatted string from timestamp HH:MM 24 Hour for readability
	strftime(timestamp, sizeof(timestamp), "%T", tmp);
	// Notify of process' exit from CS
	fprintf(stderr, "Process %d leaving critical section at %s.\n", (who + 1), timestamp);
return 0;
}
// function to set up mutual exclusion
// partial implementation of Peterson Algorithm
int setup_exc(int *sFlag, int who, int slaves, int j, int turn, FILE *logname){
		do{
			//raise my flag
			sFlag[who] = WANT_IN;
			//set local variable
			j = turn;
			//wait until it's my turn
			while(j != who)
				j = (sFlag[j] != IDLE ? turn: (j + 1) % slaves);
			//declare intention to enter CS
			sFlag[who] = IN_CS;
			// check that no one else is in CS
			for(j = 0; j < slaves; j++)
				if((j != who) && (sFlag[j] == IN_CS))
				break;
		}while ((j < slaves) || (turn != who && sFlag[turn] != IDLE));

		//assign turn to self and enter CS
		turn = who;
		critical_sec(logname, who);
		//exit CS
return 0;
}
// function to give up turn after Critical Section has been executed
// this is the rest of the Peterson Algorithm
int handoff(int *sFlag, int who, int slaves, int j, int turn){
	//assign turn to random process in n
	// I altered this section to make turn passing more random
	// Tradeoff is increased execution time due to spin waiting
	j = (rand() % slaves);		//was j = (turn + 1) % slaves;
	while(sFlag[j] == IDLE)
		j = (rand() % slaves);	// was j = (turn + 1) % slaves;
	//change own flag to IDLE
	turn = j;
	sFlag[who] = IDLE;
return 0;
}
// Ctrl-C handler
void ctrlCHandler(int sig){
	// sleep added to clean up message display order
	sleep(1);
	fprintf(stderr, "\nSIGINT detected in the child process. Process %i is dying.\n", getpid());
exit(0);
}
