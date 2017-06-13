/*$Author: o3-lael $
  $Date: 2016/03/10 02:34:47 $
  $Log: master.c,v $
  Revision 5.4  2016/03/10 02:34:47  o3-lael
  cleaned up

  Revision 5.3  2016/03/09 20:03:28  o3-lael
  added comments to new and/or uncommented sections

  Revision 5.2  2016/03/09 19:17:04  o3-lael
  *** empty log message ***

  Revision 5.1  2016/03/09 19:13:56  o3-lael
  folder structure and associated makefile implemented

  Revision 4.1  2016/03/09 19:06:49  o3-lael
  *** empty log message ***

  Revision 3.1  2016/03/09 18:10:47  o3-lael
  master implementation complete after two days of beating head on desk
  fixed shared memory allocation/attachment error which was causing semaphore
  ids to be zeroed out. The base monitor/semaphore code was near correct all along
  it was memory issues that casused all my problems

  Revision 2.3  2016/03/07 20:29:15  o3-lael
  monitor library nearly working

  Revision 2.2  2016/03/07 18:53:00  o3-lael
  *** empty log message ***

  Revision 2.1  2016/03/07 15:31:45  o3-lael
  Branch to rev. 2
  clean break from old strategy. Now implementing monitor from class notes
  master remains unchanged at this point

  Revision 1.6  2016/03/07 14:55:40  o3-lael
  still working with monitor implementation
  changing everything to reflect class notes and examples

  Revision 1.5  2016/03/06 21:44:45  o3-lael
  beginnins of monitor implemenation

  Revision 1.4  2016/03/06 19:02:14  o3-lael
  passing properties struct via shmem rather than
  passing multiple variables via multiple segments

  Revision 1.3  2016/03/06 17:25:09  o3-lael
  fiddling with shared memory and status flags

  Revision 1.2  2016/03/06 16:25:56  o3-lael
  cleaned up RCS headers and added RCS keywords where missing
  headers were funky on copy from project 2 since I didn't clean them up correctly before copy

  Revision 1.1  2016/03/04 15:37:19  o3-lael
  Initial revision

  $State: Exp $
  $Revision: 5.4 $
 */
#include "master.h"

int main(int argc, char **argv) {
	printf("\n**************************************************\n");
	// parse command line for optional switches
	parseCmd(argc, argv, &timer, &slaves, &fname);
/* Begin timed section now that we have parsed timer
 * value from command line time used to this point
 * should be negligable */
	alarm(timer);
	if(slaves == 19 && timer == 60)
		printf("Program will spawn %i slaves and run for a maximum of %i seconds before terminating.\n", slaves, timer);
	else if(slaves < 19 && timer == 60)
		printf("Program will run for a maximum of %i seconds before terminating.\n", timer);
	else if(slaves == 19 && timer < 60)
		printf("Program will spawn %i slaves.\n", slaves);
	if(strcmp(fname, "cstest") == 0)
		printf("Output will be written to cstest.\n");
	printf("**************************************************\n");
	// install signal handlers
	signal(SIGINT, ctrlCHandler);
	signal(SIGALRM, on_alarm);
	// seedrandom
	srand(time(&t));
	// initialize parent pid
	master_pid = getpid();
	if((segment_id  = shmget(IPC_PRIVATE, sizeof(Monitor), IPC_CREAT | 0644)) == -1){
		perror("Failed to allocate shared memory segment.\n");
	return 1;
	}
	// attach to newly allocated shared memory and notify status
	if((monitor = (Monitor *)shmat(segment_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to Master Monitor shared memory segment %i.\n", segment_id);
	return 1;
	}
	else
	printf("Master Monitor shared memory segment %d attached at address %p.\n", segment_id, monitor);
	
	init_monitor(monitor);
	// keep track of sems and shared memory
	monitor->shmem_id = segment_id;
	sem1_id = monitor->mutex;
	sem2_id = monitor->sem_id;
	if((segment2_id = shmget(IPC_PRIVATE, 1, IPC_CREAT | 0644)) == -1){
		perror("Failed to allocate shared memory segment.\n");
	return 1;
	} 


	// create some chillens
	for(i = 0; i < slaves; i++){
		child_pid = fork();

		// catch forking errors
		if(child_pid == -1){
			fprintf(stderr, "Forking child %i failed.\n", i);
		exit(1);
		}
		if(child_pid == 0){
			// create child pid and child vars all char have a pad of 1 beyond expected data
			int myId = getpid();
			int currChild = i;
			// semaphore id to be passed
			// char arg1[20];
			// sprintf(arg1, "%i", sem_id);
			// children limited to 19 max, so size is 2
			char arg1[3];
			sprintf(arg1, "%i", currChild);
			// Limited PID to size 5 since 16 bit int only has valid range to 32,767
			char arg2[6];
			sprintf(arg2, "%i", myId);
			// limited to size 2 for same reason as arg2
			char arg3[3];
			sprintf(arg3, "%i", slaves);
			// file names longer than 20 character seem a bit ridiculous in this scope, hence the limit
			char arg4[21];
			sprintf(arg4, "%s", fname);
			// shared memory segment id for props
			char arg5[20];
			sprintf(arg5, "%i", segment_id);
			char arg6[20];
			sprintf(arg6, "%i", segment2_id);

			printf("Child: %i has PID: %s with master/parent: %i.\n", (atoi(arg1) + 1),  arg2, getppid());
			// execute slave process in relative path with segment_id, child number and child_pid arguments
			execl("./slave", "slave", arg1, arg2, arg3, arg4, arg5, arg6, NULL);
		return 0;
		}
	}
	//wait for children to complete
	for(i = 0; i < slaves; i++){
		wait(NULL);
	}

/* End Timed Section */

	// if we got here, we're done timing
	// snooze the alarm, notify the user and clean up.
	alarm_stop = 1;
	int time_left = alarm(0);
	if(time_left > 0){
		printf("Task completed with %i seconds remaining. Cleaning up.\n", time_left);
		FILE *log;
		log = fopen(fname, "a");
		fprintf(log, "Above terminated normally with %i seconds left.\n", time_left);
		cleanup();
	}
exit(0);
}

// function to parse command line arguments
// uses pointer to time so that arguments can set the timer variable
void parseCmd(int argc, char **argv, int *timer, int *slaves, char **fname){
	int c;
	opterr = 0;
	char *avalue = NULL;
	char *bvalue = NULL;
	char *cvalue = NULL;

	while ((c = getopt(argc, argv, "t:s:f:hr")) != -1){
		switch(c){
			// switch to display help/usage of args
			case 'h':
				printf("Usage:\n");
				printf("./master	- Runs the program with default settings.\n");
				printf("./master -r	- Displays the README file.\n");
				printf("./master -f str	- Changes logging file name to the provided string (str).\n");
				printf("./master -t n	- Runs the program for a maximum time (n). 1-60 seconds. Integers only.\n");
				printf("./master -s n	- Spawns (n) slaves. 1-19 slaves. Integers only.\n");
				printf("./master -h	- Displays help/usage information.\n");
			exit(0);
			break;
			// switch to cat README to terminal
			case 'r':
				execlp("/bin/cat", "-c", "README", NULL);
			exit(0);
			break;
			// max time config switch
			case 't':
				avalue = optarg;
				// set time variable to argument after converting ascii to int
				// temp variable is to coalesce into a trunc'd int
				// to handle people who cannot provide proper integer input
				int ttemp = atoi(avalue);
				*timer = ttemp;
				// validate input is in allowable range 1-60 seconds
				if(*timer > 60 || *timer < 1){
					fprintf(stderr,"Invalid time (%s) entered. Minimum time is 1. Maximum time is 60. Integers only.\n", avalue);
					exit(1);
				}
				// prevent duplicate messaging about timer value
				else if(*timer != 60)
				printf("Program will run for %i second[s] maximum before terminating and cleaning up.\n", *timer);
			break;
			// max slaves config switch
			case 's':
				bvalue = optarg;
				// set slaves variable to argument after converting ascii to int
				// temp variable is to coalesce into a trunc'd int
				// to handle people who cannot provide proper integer input
				int stemp = atoi(bvalue);
				*slaves = stemp;
				// validate input is in allowable range 1-60 seconds
				if(*slaves > 19 || *slaves < 1){
					fprintf(stderr,"Invalid number of slaves (%s) entered. Minimum is 1. Maximum is 19. Integers only.\n", bvalue);
					exit(1);
				}
				// prevent duplicate messaging about slave numbers
				else if(*slaves != 19)
				printf("Program will spawn %i slave[s].\n", *slaves);
			break;
			// filename config switch
			case 'f':
				cvalue = optarg;
				// validate file name provided
				if(cvalue == NULL){
					fprintf(stderr,"No file name provided using default of %s.\n", *fname);
					exit(1);
				}
				else{
					// set filename variable to -f argument
					*fname = cvalue;
					printf("Output will be written to %s.\n", *fname);
				}
			break;
			// handle missing required arguments and unknown arguments
			case'?':
				if(optopt == 't' || optopt == 's' || optopt == 'f')
					fprintf(stderr,"Switch -%c requires an additional argument.\n", optopt);
				else if(isprint(optopt)){
					fprintf(stderr,"Invalid option -%c.\n", optopt);
					fprintf(stderr,"Use ./master -h to display usage information.\n");
				}
			exit(1);
		}
	}
}
// Ctrl-C handler
void ctrlCHandler(int signal){
	// pid comparison to display only the parent's clean-up messages
	if(getpid() ==  master_pid){
		fprintf(stderr, "\nCtrl-C detected in master. Cleaning up and stopping.\n");
		fprintf(stderr, "Killing children.\n");
		FILE *log;
		log = fopen(fname, "a");
		fprintf(log, "Abnormal termination. Partial output above. User interrupted with Ctrl-C or the processes ran out of time.\n");
		// call cleanup to terminate and clean up
		cleanup();
	}
exit(0);
}
// function to kill processes and release memory
void cleanup(){
	// kill processes
	signal(SIGINT, SIG_IGN);
	kill(-master_pid, SIGINT);
	// sleep allows clean stdout/stderr display
	sleep(2);
	// detach and release shared memory
	printf("Detaching and releasing shared memory segment %d.\n", segment_id);
	// remove shared memory segments
	if((shmctl(segment_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release shared memory segment.\n");
	}
	else	// report success
		printf("Shared memory segment successfully released.\n");
	if((shmctl(segment2_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release shared memory segment.\n");
	}
	else	// report success
		printf("Shared memory segment successfully released.\n");
	// remove semaphores
	if((semctl(sem1_id, 0, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "Failed to rerlease semaphore.\n");
	}
	else	// report success
		printf("Semaphore successfully released.\n");
	if((semctl(sem2_id, 0, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "Failed to rerlease semaphore.\n");
	}
	else	// report success
		printf("Semaphore successfully released.\n");
exit(0);
}
// timer mechanism used to terminate on specified time
// using SIGALRM
void on_alarm(int signal){
	// if snoozed step out
	if(alarm_stop)
	return;
	// else inform time is up and take action
	else if(alarm(timer) == 0){
		printf("Time is up. Stopping and cleaning up.\n");
		kill(master_pid, SIGINT);
	}
exit(0);
}
