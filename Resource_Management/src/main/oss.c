/*$Author: o3-lael $
  $Date: 2016/04/10 16:00:39 $
  $Log: oss.c,v $
  Revision 2.7  2016/04/10 16:00:39  o3-lael
  deadlock avoidance algorithm implemented
  matricies for avoidance implemented and memory allocation attempted

  Revision 2.6  2016/04/09 14:49:24  o3-lael
  changing rdb struct and initialization

  Revision 2.5  2016/04/09 13:41:21  o3-lael
  added forking/timinng message
  added forking/parent message

  Revision 2.4  2016/04/09 02:42:34  o3-lael
  adjusted inter-fork timing

  Revision 2.3  2016/04/09 00:09:32  o3-lael
  re-enabled and fixed create resources function

  Revision 2.2  2016/04/08 21:13:42  o3-lael
  fixed command line parse function to reflect correct program name 'oss' and options

  Revision 2.1  2016/04/08 20:58:33  o3-lael
  base project working correctly again
  branching before resource allocation implementation

  Revision 1.13  2016/04/08 20:57:15  o3-lael
  fixed memory setup/command line parse placement error

  Revision 1.12  2016/04/08 20:50:02  o3-lael
  changes to implement shared header file 'shared.h'

  Revision 1.11  2016/04/04 21:25:16  o3-lael
  normalized shared memory allocation for entry condition

  Revision 1.10  2016/04/04 15:16:17  o3-lael
  1-500 millisecond delay between child forking implemented

  Revision 1.9  2016/04/03 22:56:44  o3-lael
  added resource descriptors to shared memory

  Revision 1.8  2016/04/03 20:02:21  o3-lael
  fixed user process command line argument to reflect new limit of 18

  Revision 1.7  2016/04/03 19:56:38  o3-lael
  added resource population

  Revision 1.6  2016/04/03 17:00:42  o3-lael
  clock/critical section implemented

  Revision 1.5  2016/04/03 16:35:13  o3-lael
  working on monitor code

  Revision 1.4  2016/04/03 14:57:53  o3-lael
  added semaphore cleanup

  Revision 1.3  2016/04/03 14:49:54  o3-lael
  implemented base of monitor and semaphores

  Revision 1.2  2016/03/29 15:33:53  o3-lael
  adding monitor/semaphore implementation to solve critical section issue

  Revision 1.1  2016/03/27 15:35:39  o3-lael
  Initial revision

  $State: Exp $ 
  $Revision: 2.7 $
*/
#include "shared.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Main */
int main(int argc, char **argv) {
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	// parse command line for optional switches
	parseCmd(argc, argv, &up, &fname);
	if(up == 18)
		printf("oss will spawn %i user processes\n", up);
	if(strcmp(fname, "cstest") == 0)
		printf("Output will be written to cstest.\n");
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	
	// install signal handlers
	signal(SIGINT, ctrlCHandler);
	// initialize all memory
	setup_mem();
	init_monitor(monitor);
	// keep track of sems and shared memory
	monitor->shmem_id = segment3_id;
	sem1_id = monitor->mutex;
	sem2_id = monitor->sem_id;
	// get PID for master for later use
	oss_pid = getpid();
	// seed random 
	srand(time(NULL));	
	
	// print startup time (0.000000000) then create system resources	
	printf("Startup system time is %11.9f.\n", current_time->total);
	create_resources();

	// create some chillens
	for(i = 0; i < up; i++){
		// random number to simulate 1-500 miliseconds between children
		double rand_ms = (((rand() % 500) + 1) * 1000000);
		init_cs(monitor, entry);
		adv_clock(rand_ms);
		printf("Clock incremented %3.0fms before process %i was forked.\n", (rand_ms / 1000000), (i + 1));
		exit_cs(monitor, entry);
		
		up_pid = fork();

		// catch forking errors
		if(up_pid == -1){
			fprintf(stderr, "Forking user process %i failed.\n", i);
		exit(1);
		}
		if(up_pid == 0){
			// create child pid and child vars all char have a pad of 1 beyond expected data
			int myId = getpid();
			int currChild = i;
///////////////////////// Set up arguments to be passed to child //////////////////////////	
			// children limited to 18 max, so size is 2
			char arg1[3];
			sprintf(arg1, "%i", currChild);
			// Limited PID to size 5 since 16 bit int only has valid range to 32,767
			char arg2[6];
			sprintf(arg2, "%i", myId);
			// shared memory segment ids
			char arg3[20];
			sprintf(arg3, "%i", segment_id);	// resource descriptors
			char arg4[20];
			sprintf(arg4, "%i", segment2_id);	// clock
			char arg5[20];
			sprintf(arg5, "%i", segment3_id);	// monitor
			char arg6[20];
			sprintf(arg6, "%i", segment4_id);	// entry condition
			char arg7[20];
			sprintf(arg7, "%i", segment5_id);	//max matrix 
///////////////////////// End arguments to be passed to child //////////////////////////	

			printf("User process: %i has PID: %s and parent: %i.\n", (atoi(arg1) + 1),  arg2, oss_pid);
			// execute slave process in relative path with segment_id, child number and child_pid arguments
			execl("./up", "up", arg1, arg2, arg3, arg4, arg5, arg6, arg7, NULL);
		}
	usleep(50000);
	}	
	for(j = 0; j < up; j++){
		wait(NULL);
	}
	printf("\nTotal system running time is %11.9f.\n", current_time->total);
	cleanup();

return 0;
}
/* End main */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Administrative functions */
////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctrl-C handler
void ctrlCHandler(int signal){
	if(getpid() == oss_pid){
		fprintf(stderr, "\nCtrl-C detected in master. Cleaning up and stopping.\n");
		fprintf(stderr, "Killing children.\n");
		// call cleanup to terminate and clean up
		cleanup();
	}
exit(signal);
}
// End Ctrl-C handler
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* MEMORY ALLOCATION */
void setup_mem(){
	rdb	= (r_desc *)malloc(20 * sizeof(r_desc));
	current_time = (ClockTime *)malloc(sizeof(ClockTime));
	monitor = (Monitor *)malloc(sizeof(Monitor));
	entry = (int *)malloc(sizeof(int));
	time_in = (double *)malloc(sizeof(double));
	avail = (int *)malloc(sizeof(int) * 20);
	mark = (int *)malloc(sizeof(int) * 20);
	int **max = (int **) malloc(sizeof (int *) * up);
	for (i = 0; i < up; ++i) {
	    max[i] = (int *) malloc(sizeof (int) * 20);
	}
	int **alloc = (int **) malloc(sizeof (int *) * up);
	for (i = 0; i < up; ++i) {
	    alloc[i] = (int *) malloc(sizeof (int) * 20);
	}
	int **need = (int **) malloc(sizeof (int *) * up);
	for (i = 0; i < up; ++i) {
	    need[i] = (int *) malloc(sizeof (int) * 20);
	}
	
	// allocate shared memory segment for resource descriptors
	if((segment_id = shmget(IPC_PRIVATE, (sizeof(r_desc)), IPC_CREAT | 0644)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for resource descriptors.\n");
	exit(1);
	}
	else
		printf("Shared memory segment for resource descriptors created with ID: %i.\n", segment_id);

	// attach to newly allocated shared memory and notify status
	if((rdb = (r_desc *)shmat(segment_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for resource descriptors.\n", segment_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for resource descriptors attached at address %p.\n\n", segment_id, rdb);
	
	// allocate shared memory segment for clock
	if((segment2_id = shmget(IPC_PRIVATE, (sizeof(ClockTime)), IPC_CREAT | 0644)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for logical clock.\n");
	exit(1);
	}
	else
		printf("Shared memory segment for logical clock created with ID: %i.\n", segment2_id);

	// attach to newly allocated shared memory and notify status
	if((current_time = (ClockTime *)shmat(segment2_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for logical clock.\n", segment2_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for logical clock attached at address %p.\n\n", segment2_id, current_time);
	
	// allocate shared memory segment for Monitor 
	if((segment3_id = shmget(IPC_PRIVATE, (sizeof(Monitor)), IPC_CREAT | 0644)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for monitor.\n");
	exit(1);
	}
	else
		printf("Shared memory segment for monitor created with ID: %i.\n", segment3_id);

	// attach to newly allocated shared memory and notify status
	if((monitor = (Monitor *)shmat(segment3_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for monitor.\n", segment3_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for monitor attached at address %p.\n\n", segment3_id, monitor);
 	
	// allocate shared memory segment for entry condition 
	if((segment4_id = shmget(IPC_PRIVATE, (sizeof(int)), IPC_CREAT | 0644)) == -1){
		perror("Failed to allocate shared memory segment for entry condition.\n");
		exit(1);
	}
	else
		printf("Shared memory segment for entry condition created with ID: %i.\n", segment4_id);
	
	// attach to newly allocated shared memory and notify status
	if((entry = (int *)shmat(segment4_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for entry condition.\n", segment4_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for entry condition attached at address %p.\n\n", segment4_id, entry);
 	*entry = 0;	
 	
	// allocate shared memory segment for max matrix 
	if((segment5_id = shmget(IPC_PRIVATE, (sizeof(int *)), IPC_CREAT | 0644)) == -1){
		perror("Failed to allocate shared memory segment for max matrix.\n");
		exit(1);
	}
	else
		printf("Shared memory segment for max matrix created with ID: %i.\n", segment5_id);
	
	// attach to newly allocated shared memory and notify status
	if((max = (int **)shmat(segment5_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for max matrix.\n", segment5_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for entry condition attached at address %p.\n\n", segment5_id, max);
}
/* END SHARED MEMORY ALLOCATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Cleanup function */
void cleanup(){
	// kill processes
	signal(SIGINT, SIG_IGN);
	// Get rid of the kids
	kill(-oss_pid, SIGINT);
	// sleep allows clean stdout/stderr display
	usleep(50000);

	// detach and release resource descriptor shared memory
	printf("Detaching and releasing resource descriptor shared memory segment %d and freeing up address %p.\n", segment_id, rdb);
	// detach/catch detach error
	if(shmdt(rdb) == -1){
		fprintf(stderr, "Unable to detach resource descriptor shared memory %d @ %p.\n", segment_id, rdb);
	}
	else	// report success
		printf("Resource descriptor memory successfully detached.\n");
	// remove segment
	if((shmctl(segment_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release resource descriptor shared memory segment.\n");
	}
	else	// report success
		printf("Resource descriptor shared memory segment successfully released.\n\n");

	// detach and release ClockTime shared memory
	printf("Detaching and releasing logical clock shared memory segment %d and freeing up address %p.\n", segment2_id, current_time);
	// detach/catch detach error
	if(shmdt(current_time) == -1){
		fprintf(stderr, "Unable to detach logical clock shared memory %d @ %p.\n", segment2_id, current_time);
	}
	else	// report success
		printf("Logical clock memory successfully detached.\n");
	// remove segment
	if((shmctl(segment2_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release logical clock shared memory segment.\n");
	}
	else	// report success
		printf("Logical clock shared memory segment successfully released.\n\n");

	// detach and release Monitor shared memory
	printf("Detaching and releasing monitor shared memory segment %d and freeing up address %p.\n", segment3_id, monitor);
	// detach/catch detach error
	if(shmdt(monitor) == -1){
		fprintf(stderr, "Unable to detach monitor shared memory %d @ %p.\n", segment3_id, monitor);
	}
	else	// report success
		printf("Monitor memory successfully detached.\n");
	// remove segment
	if((shmctl(segment3_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release monitor shared memory segment.\n");
	}
	else	// report success
		printf("Monitor shared memory segment successfully released.\n\n");

	// release entry condition shared memory
	// remove segment
	if((shmctl(segment4_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release entry condition shared memory segment.\n");
	}
	else	// report success
		printf("Entry condition shared memory segment successfully released.\n\n");

	// release max matrix shared memory
	// remove segment
	if((shmctl(segment5_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release max matrix shared memory segment.\n");
	}
	else	// report success
		printf("Max matrix shared memory segment successfully released.\n\n");
    
	// remove semaphores
	if((semctl(sem1_id, 0, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "Failed to rerlease semaphore.\n");
	}
	else    // report success
		printf("Semaphore successfully released.\n");
	if((semctl(sem2_id, 0, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "Failed to rerlease semaphore.\n");
	}
	else    // report success
		printf("Semaphore successfully released.\n");
	free(time_in);
	free(avail);
	free(mark);
exit(0);
}
/* End cleanup function */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Parse command line arguments */
void parseCmd(int argc, char **argv, int *up, char **fname){
	int c;
	opterr = 0;
	char *bvalue = NULL;
	char *cvalue = NULL;

	while ((c = getopt(argc, argv, "s:f:hr")) != -1){
		switch(c){
			// switch to display help/usage of args
			case 'h':
				printf("Usage:\n");
				printf("./oss		- Runs the program with default settings.\n");
				printf("./oss -r	- Displays the README file.\n");
				printf("./oss -f str	- Changes logging file name to the provided string (str).\n");
				printf("./oss -s n	- Spawns (n) user processes. 1-18 processes. Integers only.\n");
				printf("./oss -h	- Displays help/usage information.\n");
			exit(0);
			break;
			// switch to cat README to terminal
			case 'r':
				execlp("/bin/cat", "-c", "README", NULL);
			exit(0);
			break;
			// max user processes config switch
			case 's':
				bvalue = optarg;
				// set up variable to argument after converting ascii to int
				// temp variable is to coalesce into a trunc'd int
				// to handle people who cannot provide proper integer input
				int stemp = atoi(bvalue);
				*up = stemp;
				// validate input is in allowable range 1-60 seconds
				if(*up > 18 || *up < 1){
					fprintf(stderr,"Invalid number of user processes (%s) entered. Minimum is 1. Maximum is 18. Integers only.\n", bvalue);
					fprintf(stderr,"\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					exit(1);
				}
				// prevent duplicate messaging about slave numbers
				if(*up == 1)
					printf("oss will spawn %i user process.\n", *up);
				else if(*up != 18)
					printf("oss will spawn %i user processes.\n", *up);
			break;
			// filename config switch
			case 'f':
				cvalue = optarg;
				// validate file name provided
				if(cvalue == NULL){
					fprintf(stderr,"No file name provided using default of %s.\n", *fname);
				}
				else{
					// set filename variable to -f argument
					*fname = cvalue;
					printf("Output will be written to %s.\n", *fname);
				}
			break;		
			// handle missing required arguments and unknown arguments
			case'?':
				if(optopt == 's' || optopt == 'f'){
					fprintf(stderr,"Switch -%c requires an additional argument.\n", optopt);
					fprintf(stderr,"\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				}
				else if(isprint(optopt)){
					fprintf(stderr,"Invalid option -%c.\n", optopt);
					fprintf(stderr,"Use ./oss -h to display usage information.\n");
					fprintf(stderr,"\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				}
			exit(1);
		}
	}
}
/* End parse command line arguments */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* End administrative functions */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Resource functions */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Create resources */
void create_resources(){
	double total;
	double shared;
	int type;
	// loop through and create 20 processes
	for(i = 1; i <= 20; i++){
		*time_in = current_time->total;
		// resource type 0-static/1-sharable
		// conditions to ensure 20% +/- 5% sharable resources
		if(total == 0){
			type = 0;
			total++;
		}
		else if((shared / total < .20) && (shared  / total < .25)){
			type = 1;
			shared++;
			total++;
		}
		else{
			type = 0;
			total++;
		}
		// populate descriptor block fields
		rdb[i].resource_id = i; rdb[i].type = type;
		// if shared, only 1 instance
		if(rdb[i].type == 1)
		rdb[i].total = 1;
		// otherwise rand up to 10 instances
		else
		rdb[i].total = ((rand() % 10) + 1);
		// set initial available to total amount
		rdb[i].avail = rdb[i].total;
		// printf creation results
		if(rdb[i].type == 1 && rdb[i].total > 1)
		printf("Resource %i created as a sharable resource with %i instances\n", i, rdb[i].total);
		else if(rdb[i].type == 1 && rdb[i].total == 1)
		printf("Resource %i created as a sharable resource with %i instance\n", i, rdb[i].total);
		else if(rdb[i].type == 0 && rdb[i].total > 1)
		printf("Resource %i created as a static resource with %i instances\n", i, rdb[i].total);
		else
		printf("Resource %i created as a static resource with %i instance\n", i, rdb[i].total);
		// available matrix entry/creation
		avail[i] = rdb[i].total;
	}
	printf("Availavble matrix:\n");
	for(i=1; i<=20; i++){
		printf("%i\t", avail[i]);
	}
	printf("\n");
}
/* End create resources */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* End resource functions */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Critical section/monitor code */
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
/* End critical section/monitor code */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* LOGICAL CLOCK IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set clock desired amount ahead */
// amount values is passed in integer nanoseconds
void adv_clock(double time){
	// increment in rollover situation
	if((current_time->ns + time) >= 1000000000){
		current_time->sec +=1;	
		current_time->ns = ((current_time->ns + time) - 1000000000);
		current_time->total = (current_time->sec + (current_time->ns * .000000001));
	}
	else{
		current_time->ns = current_time->ns + time;
		current_time->total = (current_time->sec + (current_time->ns * .000000001));
	}
}	
/* End set clock */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* COLLISION AVOIDANCE IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// available matrix created/populated during resource creation ///////////////////
/* Compute need matrix */
void calc_need(){
	for(i = 0;i < up; i++){
		for(j = 0;j < 20; j++){
			need[i][j] = max[i][j] - alloc[i][j];
		}
	}
}
/* End compute need matrix */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Check for safe/unsafe condition using banker's algorithm */
void safe_check(){
	int in_need = up;

	while(in_need != 0){
		temp = in_need;
		for(i = 0; i < up; i++){
			if(mark[i] == 0){
				flag = 1;
				for(j = 0; j < 20; j++){
					if((avail[j] - need[i][j]) < 0){
						flag = 0;
						break;
					}
				}
				if(flag == 1){
					printf("Needs met for process %i.\n", i + 1);
					mark[i] = 1;
					in_need--;
					for(j = 0; j < 20; j++){
						avail[j] += alloc[i][j];
					}
				}
			}
		}
		if(in_need == temp){
			printf("DEADLOCK!!! System may reach an unsafe state.\n");
			break;
		}
	}
}
/* End check for safe/unsafe condition using banker's algorithm */
////////////////////////////////////////////////////////////////////////////////////////////////////
