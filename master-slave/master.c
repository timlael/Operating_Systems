/* $Author: o3-lael $
 * $Date: 2016/02/20 15:43:29 $
 * $Log: master.c,v $
 * Revision 4.1  2016/02/20 15:43:29  o3-lael
 * final branch ready to turn in
 * added termination status print to output file
 * cleaned uppunctuation in messages
 *
 * Revision 3.7  2016/02/19 22:35:22  o3-lael
 * formatted screen output to b emore readable
 *
 * Revision 3.6  2016/02/19 18:16:04  o3-lael
 * added lines to make initial conditions more readable when output to terminal
 *
 * Revision 3.5  2016/02/19 17:57:55  o3-lael
 * added message about default output location
 *
 * Revision 3.4  2016/02/19 17:14:37  o3-lael
 * commented out libraried that were unused in the final product
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
 * Revision 2.4  2016/02/18 18:54:04  o3-lael
 * general tune up and comments
 *
 * Revision 2.3  2016/02/18 16:48:04  o3-lael
 * removed fcloseall() causing compilation warnings
 * replaced with exit(0[1])
 *
 * Revision 2.2  2016/02/18 16:23:30  o3-lael
 * adopted new timer method and implemented changes to compliment
 *
 * Revision 2.1  2016/02/18 14:57:52  o3-lael
 * Branch
 * Program is functionally complete
 * Now it is time to clean up and fine tune
 *
 * Revision 1.26  2016/02/18 04:05:47  o3-lael
 * timer and signal handler updated
 *
 * Revision 1.25  2016/02/17 21:48:07  o3-lael
 * timestamp working
 *
 * Revision 1.23  2016/02/17 04:48:08  o3-lael
 * poking at mutex still
 *
 * Revision 1.22  2016/02/17 03:56:13  o3-lael
 * everything is in place and working except for peterson block
 * code is there, but critical section is not executing
 *
 * Revision 1.21  2016/02/16 22:04:25  o3-lael
 * ironed out execlp functionality
 * added child process wait code
 *
 * Revision 1.20  2016/02/16 03:21:23  o3-lael
 * fixed optional arguments bug
 *
 * Revision 1.19  2016/02/16 03:07:31  o3-lael
 * implemented configuration changes
 * filename and number of slaves now controlled optionally
 * via command line switch
 *
 * Revision 1.18  2016/02/15 22:16:08  o3-lael
 * fine tuned timer command line input to handle users who attempt other than integer input
 *
 * Revision 1.17  2016/02/15 21:53:53  o3-lael
 * created a function for cleanup of memory and processes
 * implemented in normal, timeout and Ctrl-C cases
 * to reduce and stremline code
 *
 * Revision 1.16  2016/02/15 21:17:18  o3-lael
 * fixed bug in Ctrl-C handler regarding memory deallocation reporting
 * added animation during normal and time exceeded kill
 *
 * Revision 1.15  2016/02/15 20:42:34  o3-lael
 * multiple messages on Ctrl-C fixed in 1.14
 * only showing parent message
 * comments updated in this checkin
 *
 * Revision 1.13  2016/02/15 20:20:38  o3-lael
 * implemented and tested Ctrl-C signal handling
 * double stdout output at this time, but it works otherwise
 *
 * Revision 1.12  2016/02/15 18:29:16  o3-lael
 * cleaning up timer
 *
 * Revision 1.11  2016/02/15 17:17:55  o3-lael
 * created and tested running time timer
 *
 * Revision 1.10  2016/02/14 14:54:47  o3-lael
 * start of overall timer added/not yet complete
 *
 * Revision 1.9  2016/02/13 22:42:16  o3-lael
 * removed sleep timer used during fork testing
 *
 * Revision 1.8  2016/02/13 22:38:14  o3-lael
 * fixed forking issues
 *
 * Revision 1.6  2016/02/13 20:20:12  o3-lael
 * shared memory implemented, testing and working
 *
 * Revision 1.5  2016/02/13 19:10:39  o3-lael
 * *** empty log message ***
 *
 * Revision 1.4  2016/02/13 16:06:01  o3-lael
 * cleaned up, made header file and moved includes and prototypes there
 * added/ironed out command line switches for optional arguments
 *
 * Revision 1.3  2016/02/13 14:57:35  o3-lael
 * added includes and main with time ans slave variables set
 *
 * Revision 1.2  2016/02/12 21:54:43  o3-lael
 * added RCS keywords
 *
 * $State: Exp $
 * $Revision: 4.1 $
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
	// allocate shared memory
	if((segment_id = shmget(IPC_PRIVATE, (sizeof(sFlag)*20), S_IRUSR | S_IWUSR)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment.\n");
	return 1;
	}
	else
		printf("Shared memory segment created with ID: %i.\n", segment_id);
	// attach to newly allocated shared memory and notify status
	if((sFlag = (int *)shmat(segment_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i.\n", segment_id);
	return 1;
	}
	else
	printf("Shared memory segment %d attached at address %p.\n", segment_id, sFlag);
	
	// memory flags (IDLE, WANT_IN, IN_CS)
	// set to default state of IDLE
	for(i = 0; i < slaves; i++){
		sFlag[i] = IDLE;
	}
	
	// create some chillens
	for(i = 0; i < slaves; i++){
		child_pid = fork();
	
		// test sleep used for timer testing. 
		// sleep(rand() % 3);
		// catch forking errors
		if(child_pid == -1){
			fprintf(stderr, "Forking child %i failed.\n", i);
		exit(1);
		}
		if(child_pid == 0){
			// create child pid and child vars all char have a pad of 1 beyond expected data
			int myId = getpid();
			int currChild = i;
			// I haven't seen a segment ID longer than 9 on Hoare, so size is 9
			char arg1[10];
			sprintf(arg1, "%i", segment_id); 
			// children limited to 19 max, so size is 2
			char arg2[3];
			sprintf(arg2, "%i", currChild);
			// Limited PID to size 5 since 16 bit int only has valid range to 32,767 
			char arg3[6];
			sprintf(arg3, "%i", myId);
			// limited to size 2 for same reason as arg2
			char arg4[3];
			sprintf(arg4, "%i", slaves);
			// file names longer than 20 character seem a bit ridiculous in this scope, hence the limit
			char arg5[21];
			sprintf(arg5, "%s", fname);
			printf("Child: %i has PID: %s with master/parent: %i.\n", (atoi(arg2) + 1),  arg3, getppid());
			// execute slave process in relative path with segment_id, child number and child_pid arguments
			execl("./slave", "slave", arg1, arg2, arg3, arg4, arg5, NULL);
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
	printf("Detaching and releasing shared memory segment %d and freeing up address %p.\n", segment_id, sFlag);
	// detach/catch detach error
	if(shmdt(sFlag) == -1){
		fprintf(stderr, "Unable to detach shared memory %d @ %p.\n", segment_id, sFlag);
	}
	else	// report success
		printf("Memory successfully detached.\n");
	// remove segment
	if((shmctl(segment_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release shared memory segment.\n");
	}
	else	// report success
		printf("Shared memory segment successfully released.\n\n");
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
