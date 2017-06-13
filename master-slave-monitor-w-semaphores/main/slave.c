/*$Author: o3-lael $
  $Date: 2016/03/10 02:34:47 $
  $Log: slave.c,v $
  Revision 5.5  2016/03/10 02:34:47  o3-lael
  cleaned up

  Revision 5.4  2016/03/10 02:14:56  o3-lael
  removed unused items

  Revision 5.3  2016/03/09 20:03:28  o3-lael
  added comments to new and/or uncommented sections

  Revision 5.2  2016/03/09 19:17:04  o3-lael
  *** empty log message ***

  Revision 5.1  2016/03/09 19:13:56  o3-lael
  folder structure and associated makefile implemented

  Revision 4.1  2016/03/09 19:06:49  o3-lael
  *** empty log message ***

  Revision 3.1  2016/03/09 18:10:47  o3-lael
  slave implementation completed

  Revision 2.3  2016/03/07 20:29:15  o3-lael
  monitor library nearly working

  Revision 2.2  2016/03/07 18:53:00  o3-lael
  *** empty log message ***

  Revision 2.1  2016/03/07 15:31:45  o3-lael
  Branch to rev. 2
  clean break from old strategy. Now implementing monitor from class notes
  slave unchanged at this point

  Revision 1.5  2016/03/07 14:55:40  o3-lael
  implementing Monitoe class to reflect notes

  Revision 1.4  2016/03/06 21:44:45  o3-lael
  beginnins of monitor implemenation

  Revision 1.3  2016/03/06 17:25:09  o3-lael
  fiddling with shared memory and status flags

  Revision 1.2  2016/03/06 16:25:56  o3-lael
  cleaned up RCS headers and added RCS keywords where missing
  headers were funky on copy from project 2 since I didn't clean them up correctly before copy

  Revision 1.1  2016/03/04 15:37:19  o3-lael
  Initial revision

  $State: Exp $
  $Revision: 5.5 $
*/
#include "slave.h"

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr,"\n*******************************************************************************\n");
		fprintf(stderr, "* %s is intended to be called by master with arguments passed from master. *\n", argv[0]);
		fprintf(stderr,"*******************************************************************************\n\n");
	exit(1);
	}
	Monitor *monitor;
	// install signal handler
	signal(SIGINT, ctrlCHandler);
	// seed random
	srand(time(&t));

	/* Below passed from master via execlp arguments */
	// child number (0-18)
	int who = atoi(argv[1]);
	// child_pid
	int myID = atoi(argv[2]);
	// number of slaves
	int slaves = atoi(argv[3]);
	// filename
	if(argv[4] != NULL)
		filename = argv[4];
	// semaphore counter shared memory id
	int shmem_id = atoi(argv[5]);
	int shmem2_id = atoi(argv[6]);
	/* End passed arguments */
	logname = fopen(filename, "a");
	// make log writes immediately - no buffering (seems to have a big hit on running time)
	setbuf(logname, NULL);
	//attach to entry condition and initialize
	if((entry = (int *)shmat(shmem2_id, NULL, 0)) == (void * )-1){
		perror("Failed to attach to conditions shared memory segment.\n");
	return 1;
	}
	else
	 	printf("Child: %i with PID: %i now attached to entry condition shared memory segment %i.\n", (who + 1), myID, shmem2_id);
	*entry = 0;
	// attach to Monitor shared memory segment
	if((monitor = (Monitor *)shmat(shmem_id, NULL, 0)) == (void *) -1){
    		perror("Failed to attach to required Master Monitor shared memory segment.\n");
    	return 1;
  	}
	else
	 	printf("Child: %i with PID: %i now attached to Master Monitor shared memory segment %i.\n", (who + 1), myID, shmem_id);

	// This is where the magic happens
	for(i = 0; i < 3; i++){
		// setup mutual exclusion via monitor and execute CS
		init_cs(monitor, entry);
		critical_sec(logname, who);
		exit_cs(monitor, entry);
		printf("Process %i with PID %i says Hello from the \"Remainder Section\".\n", (who + 1), myID);
	}
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

// Ctrl-C handler
void ctrlCHandler(int sig){
	// sleep added to clean up message display order
	sleep(1);
	fprintf(stderr, "\nSIGINT detected in the child process. Process %i is dying.\n", getpid());
exit(0);
}

// initiate entry into critical section
void init_cs(Monitor *m, int *e_cond){
        enter_monitor(m);
        if(*e_cond)
        wait_condition(m, e_cond);
}

//exit critical section
void exit_cs(Monitor *m, int *e_cond){
        *e_cond = 0;
        signal_condition(m, e_cond);
}
