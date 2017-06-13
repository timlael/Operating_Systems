/* $Author: o3-lael $
 * $Date: 2016/03/23 20:00:58 $
 * $Log: oss.c,v $
 * Revision 4.8  2016/03/23 20:00:58  o3-lael
 * fixing timer requirement
 *
 * Revision 4.6  2016/03/22 00:51:45  o3-lael
 * commenting file
 *
 * Revision 4.4  2016/03/21 21:41:59  o3-lael
 * monkeying with I/O and CPU bound demotion/promotion
 *
 * Revision 4.3  2016/03/21 21:28:25  o3-lael
 * removing unused bits
 *
 * Revision 4.2  2016/03/21 16:02:52  o3-lael
 * useless junk cleared out and arguments changed to reflect removals
 *
 * Revision 4.1  2016/03/21 15:24:44  o3-lael
 * project functional.
 * branching for final cleanup and release
 *
 * Revision 3.19  2016/03/21 13:15:24  o3-lael
 * sleep used for development shortened for release
 * re-prioritization implemented
 *
 * Revision 3.18  2016/03/20 20:26:40  o3-lael
 * working
 *
 * Revision 3.16  2016/03/20 17:30:36  o3-lael
 * fixed random timing issue that was my doing
 *
 * Revision 3.15  2016/03/20 15:20:57  o3-lael
 * almost working
 * last process in hanging and stats not running
 *
 * Revision 3.14  2016/03/19 19:40:03  o3-lael
 * *** empty log message ***
 *
 * Revision 3.13  2016/03/19 17:17:47  o3-lael
 * working on oss/up relationship
 *
 * Revision 3.12  2016/03/18 19:57:53  o3-lael
 * execl and shared memory items added
 *
 * Revision 3.11  2016/03/18 01:47:50  o3-lael
 * fixed minor scheduler turn logic error
 *
 * Revision 3.10  2016/03/18 01:41:18  o3-lael
 * *** empty log message ***
 *
 * Revision 3.9  2016/03/18 00:59:49  o3-lael
 * altered random process running time to 1-16 ms
 *
 * Revision 3.8  2016/03/18 00:39:44  o3-lael
 * bit vector implemented, but not in shared memory yet
 *
 * Revision 3.7  2016/03/18 00:28:07  o3-lael
 * scheduler completed and fully functional
 *
 * Revision 3.6  2016/03/17 20:22:39  o3-lael
 * clock now incrementing with job progression
 *
 * Revision 3.5  2016/03/17 20:13:08  o3-lael
 * context switching now functional
 *
 * Revision 3.4  2016/03/17 19:16:50  o3-lael
 * fine tuned datatypes that were causing rounding errors
 *
 * Revision 3.3  2016/03/17 17:54:17  o3-lael
 * scheduler working
 *
 * Revision 3.2  2016/03/17 02:33:31  o3-lael
 * clock functioning as expected
 * scheduling wanky
 *
 * Revision 3.1  2016/03/17 01:42:36  o3-lael
 * branched to sync file versions with addition of up.c and up.h
 *
 * Revision 2.3  2016/03/17 01:33:15  o3-lael
 * clock/increment clock finished
 *
 * Revision 2.2  2016/03/16 17:14:18  o3-lael
 * turned pcb into double linked list
 *
 * Revision 2.1  2016/03/16 15:19:08  o3-lael
 * branching to v2 and implementing forked master/slave functionality
 *
 * Revision 1.15  2016/03/16 02:14:48  o3-lael
 * working on scheduler
 *
 * Revision 1.14  2016/03/14 18:47:53  o3-lael
 * finished algorithms, running without forking... sort of
 *
 * Revision 1.13  2016/03/14 17:26:19  o3-lael
 * added SJF algorithmm and most supporting functions
 *
 * Revision 1.12  2016/03/13 20:58:32  o3-lael
 * *** empty log message ***
 *
 * Revision 1.11  2016/03/13 19:16:01  o3-lael
 * logical clock working
 *
 * Revision 1.10  2016/03/13 18:05:16  o3-lael
 * cleaned up main and made things more modular by implementing functions
 *
 * Revision 1.9  2016/03/13 16:46:29  o3-lael
 * added bit array
 *
 * Revision 1.8  2016/03/13 04:01:35  o3-lael
 * random seed added
 *
 * Revision 1.7  2016/03/12 23:21:12  o3-lael
 * added queue_typ and file seperators
 *
 * Revision 1.6  2016/03/12 22:59:49  o3-lael
 * added function placeholders for scheduling algorithms
 *
 * Revision 1.5  2016/03/12 22:05:19  o3-lael
 * cleaned apscing
 *
 * Revision 1.4  2016/03/12 21:36:27  o3-lael
 * changed Job type instances from queue to job to clarifythat they are jobs
 * I initially had them named as queue items which are jobs, but that could be confusing
 *
 * Revision 1.3  2016/03/12 21:18:36  o3-lael
 * added command line swith options
 * allocation of memory for queues and stats for each of three priotity levels added
 *
 * Revision 1.2  2016/03/12 19:26:35  o3-lael
 * addition of shared memory allocation
 * addition of Ctrl-C handler
 *
 * Revision 1.1  2016/03/12 18:23:37  o3-lael
 * Initial revision
 *
 * $State: Exp $
 * $Revision: 4.8 $
*/
#include "oss.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Main */
int main(int argc, char **argv) {
	// install signal handlers
	signal(SIGINT, ctrlCHandler);
	// initialize all memory
	setup_mem();
	*current_job = INT_MAX;
	// get PID for master for later use
	oss_pid = getpid();
	// seed random 
	srand(time(NULL));	
	
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	// parse command line for optional switches
	parseCmd(argc, argv, &up, &fname);
	if(up == 19)
		printf("oss will spawn %i user processes\n", up);
	if(strcmp(fname, "cstest") == 0)
		printf("Output will be written to cstest.\n");
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	

	printf("Startup system time is %11.9f.\n", current_time->total);
	setup_bit_array();
	create_queue();
	// create some chillens
	for(i = 0; i < up; i++){
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
			// children limited to 19 max, so size is 2
			char arg1[3];
			sprintf(arg1, "%i", currChild);
			// Limited PID to size 5 since 16 bit int only has valid range to 32,767
			char arg2[6];
			sprintf(arg2, "%i", myId);
			// shared memory segment ids
			char arg3[20];
			sprintf(arg3, "%i", segment_id);	// PCBs
			char arg4[20];
			sprintf(arg4, "%i", segment2_id);	// number_waiting 
			char arg5[20];
			sprintf(arg5, "%i", segment3_id);	// clock

			printf("User process: %i has PID: %s.\n", (atoi(arg1) + 1),  arg2);
			// execute slave process in relative path with segment_id, child number and child_pid arguments
			execl("./up", "up", arg1, arg2, arg3, arg4, arg5, NULL);
		return 0;
		}
	usleep(95000);
	}
	schedule();
	for(j = 0; j < up; j++){
		wait(NULL);
	}

	printf("\nTotal system running time is %11.9f.\n", current_time->total);
	print_stats();
	cleanup();

return 0;
}
/* End main */
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
/* Print Stats to file */
void print_stats(){
	int k;
	double proc_time;
	logname = fopen(fname, "a");
	// make log writes immediately - no buffering (seems to have a big hit on running time)
	setbuf(logname, NULL);
	fprintf(logname, "// Process Stats\n");
	fprintf(logname, "////////////////////////////////////////////////////////////////////////////////////////////////////\n");
	for(k = 1; k <= up; k++){
	fprintf(logname, "// Process %i\n", k);
	fprintf(logname, "////////////////////////////////////////////////////////////////////////////////////////////////////\n");
		fprintf(logname, "Statistics for process %i:\n", k); 
		fprintf(logname, "Process started with a priority of %i\n", pcb[k].spriority);
		fprintf(logname, "Process finished with a priority of %i\n", pcb[k].priority);
		fprintf(logname, "Relative submission time: %11.9f seconds\n", pcb[k].submit_time);
		fprintf(logname, "Relative ending time: %11.9f seconds\n", pcb[k].end_time);
		fprintf(logname, "--------------------------------------------------\n");
		fprintf(logname, "Cumulative processing time (initial time requirement): %11.9f seconds\n", pcb[k].time_spent);
		fprintf(logname, "Cumulative wait time: %11.9f seconds\n", ((pcb[k].end_time - pcb[k].submit_time) - pcb[k].time_spent));
		fprintf(logname, "Total time in system for process %i: %11.9f seconds\n", k, (pcb[k].end_time - pcb[k].submit_time));
		fprintf(logname, "Process acquired the processor %i times.\n", (pcb[k].cycles + 1));
	fprintf(logname, "////////////////////////////////////////////////////////////////////////////////////////////////////\n");
		proc_time += pcb[k].time_spent;
	}
	fprintf(logname, "// System Stats\n");
	fprintf(logname, "////////////////////////////////////////////////////////////////////////////////////////////////////\n");
	fprintf(logname, "Total accumulated running time: %11.9f seconds\n", current_time->total);	
	fprintf(logname, "Total processor time: %11.9f seconds\n", proc_time);
	fprintf(logname, "Time processor spent idle: %11.9f seconds\n\n", (current_time->total - proc_time));
	fprintf(logname, "// End system Stats\n");
	fprintf(logname, "////////////////////////////////////////////////////////////////////////////////////////////////////\n");
}
// End print stats to file
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* BIT ARRAY IMPLEMENTATION */
void setup_bit_array(){
	// a single int is good for 0-31 (32) bits
	A[0] = 0;                    // Clear the bit array

	/* TEST CODE 
 	printf("Set bit poistions 1, 8 and 19\n");
	SetBit(A, 1);               
	SetBit(A, 8);
	SetBit(A, 19);
	// Check if SetBit() works:
	for (i = 0; i < 32; i++)
		if (TestBit(A, i))
			printf("Bit %d was set !\n", i);
	printf("\nClear bit poistion 8 \n");
	ClearBit(A, 8);
	// Check if ClearBit() works:
	for (i = 0; i < 32; i++)
		if (TestBit(A, i))
			printf("Bit %d was set !\n", i);
	*///END TEST CODE
}
/* END BIT ARRAY IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* MEMORY ALLOCATION */
void setup_mem(){
	int i;
	sec = malloc(sizeof(double));
	ns = malloc(sizeof(double));
	rand_ns = malloc(sizeof(double));
	total = malloc(sizeof(double));
	time_in = malloc(sizeof(double));
	current_job = malloc(sizeof(int));
	context = malloc(sizeof(int));
	turn_to = malloc(sizeof(int));
	number_waiting = malloc(sizeof(int));
	// alocate enough memory for all jobs	
	pcb = (PCB *)malloc(up * sizeof(PCB));
	// allocate memory for logical clock
	current_time = malloc(sizeof(ClockTime));
	
	// allocate shared memory segment for PCB
	if((segment_id = shmget(IPC_PRIVATE, (sizeof(PCB)*18), S_IRUSR | S_IWUSR)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for PCBs.\n");
	exit(1);
	}
	else
		printf("\nShared memory segment for PCBs created with ID: %i.\n", segment_id);
	
	// attach to newly allocated shared memory and notify status
	if((pcb = (PCB *)shmat(segment_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for PCBs.\n", segment_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for PCBs attached at address %p.\n\n", segment_id, pcb);
	
	// allocate shared memory segment for number_waiting
	if((segment2_id = shmget(IPC_PRIVATE, (sizeof(int)), S_IRUSR | S_IWUSR)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for number_waiting.\n");
	exit(1);
	}
	else
		printf("Shared memory segment for number_waiting created with ID: %i.\n", segment2_id);

	// attach to newly allocated shared memory and notify status
	if((number_waiting = (int *)shmat(segment2_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for number_waiting.\n", segment2_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for number_waiting attached at address %p.\n\n", segment2_id, number_waiting);
	
	// allocate shared memory segment for clock
	if((segment3_id = shmget(IPC_PRIVATE, (sizeof(ClockTime)), S_IRUSR | S_IWUSR)) ==  -1){
		fprintf(stderr, "Failed to get shared memory segment for logical clock.\n");
	exit(1);
	}
	else
		printf("Shared memory segment for logical clock created with ID: %i.\n", segment3_id);

	// attach to newly allocated shared memory and notify status
	if((current_time = (ClockTime *)shmat(segment3_id, NULL, 0)) == (void *) -1){
		fprintf(stderr,"Failed to attach to shared memory segment %i for logical clock.\n", segment3_id);
	exit(1);
	}
	else
	printf("Shared memory segment %d for logical clock attached at address %p.\n\n", segment3_id, current_time);
	
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

	// detach and release PCB shared memory
	printf("\nDetaching and releasing PCB shared memory segment %d and freeing up address %p.\n", segment_id, pcb);
	// detach/catch detach error
	if(shmdt(pcb) == -1){
		fprintf(stderr, "Unable to detach PCB shared memory %d @ %p.\n", segment_id, pcb);
	}
	else	// report success
		printf("PCB memory successfully detached.\n");
	// remove segment
	if((shmctl(segment_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release PCB shared memory segment.\n");
	}
	else	// report success
		printf("PCB shared memory segment successfully released.\n\n");

	// detach and release number_waiting shared memory
	printf("Detaching and releasing number_waiting shared memory segment %d and freeing up address %p.\n", segment2_id, number_waiting);
	// detach/catch detach error
	if(shmdt(number_waiting) == -1){
		fprintf(stderr, "Unable to detach number_waiting shared memory %d @ %p.\n", segment2_id, number_waiting);
	}
	else	// report success
		printf("number_waiting memory successfully detached.\n");
	// remove segment
	if((shmctl(segment2_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release number_waiting shared memory segment.\n");
	}
	else	// report success
		printf("number_waiting shared memory segment successfully released.\n\n");

	// detach and release ClockTime shared memory
	printf("Detaching and releasing logical clock shared memory segment %d and freeing up address %p.\n", segment3_id, current_time);
	// detach/catch detach error
	if(shmdt(current_time) == -1){
		fprintf(stderr, "Unable to detach logical clock shared memory %d @ %p.\n", segment3_id, current_time);
	}
	else	// report success
		printf("Logical clock memory successfully detached.\n");
	// remove segment
	if((shmctl(segment3_id, IPC_RMID, NULL)) == -1){
		fprintf(stderr,"Failed to release logical clock shared memory segment.\n");
	}
	else	// report success
		printf("Logical clock shared memory segment successfully released.\n\n");

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
				printf("./master	- Runs the program with default settings.\n");
				printf("./master -r	- Displays the README file.\n");
				printf("./master -f str	- Changes logging file name to the provided string (str).\n");
				printf("./master -s n	- Spawns (n) slaves. 1-19 slaves. Integers only.\n");
				printf("./master -h	- Displays help/usage information.\n");
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
				if(*up > 19 || *up < 1){
					fprintf(stderr,"Invalid number of user processes (%s) entered. Minimum is 1. Maximum is 19. Integers only.\n", bvalue);
					fprintf(stderr,"\n<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					exit(1);
				}
				// prevent duplicate messaging about slave numbers
				if(*up == 1)
					printf("oss will spawn %i user process.\n", *up);
				else if(*up != 19)
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
					fprintf(stderr,"Use ./master -h to display usage information.\n");
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
/* LOGICAL CLOCK IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Increment logical clock */
// ns is an integer representation of nanoseconds
// multiply by .000000001 to get decimal ns
// multiply by .000001 to get decimal ms
void increment_clock(){
	// increment random 0-1000 ns
	*rand_ns = rand() % 1001;
	*ns += *rand_ns;
	// increment seconds when ns roll over
	if(*ns >= 1000000000){
		*sec += 1;
		*ns = *ns - 1000000000;
	}
	// set values in time structure
	current_time->sec = *sec;
	current_time->ns = *ns;
	// implementation of total time which may prove easier to use
	*total = *sec + (*ns * .000000001);
	current_time->total = *total;
}
/* End increment logical clock */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set clock desired amount ahead */
void set_clock(double amount){
	// increment nanosecond amount if it does not cause a roll over
	if(amount + *ns < 1000000000){
		current_time->total += amount;
		*ns += amount;
	}
	// increment ns amount in roll over situation
	else{
		current_time->total += amount;
		*sec += 1;
		*ns = *ns - 1000000000;
	}
	
}	
/* End set clock */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* END LOGICAL CLOCK IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Job/Queue functions */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Create job queue */
void create_queue(){
	int i;
	// fill the queue with new Job items of random priority(1-3) and time needed
	for(i = 1; i <= up; i++){
		*time_in = current_time->total;
		// random between 10000 and 160000 to represent ns needed by process
		double time_needed = (((double) rand() / (double) RAND_MAX) * 150000) + 10000;
		// random time to represent a processes being spawned every 1-2 seconds
		double time_adv = (((double) rand() / (double) RAND_MAX) * 50000) + 50000;
		int pri = ((rand() % 3) +1); // priority 1 - 3 for queues 0 - 2
		//new with attribs{pid, priority, submit_time, start_time, end_time, time_left, burst_time, time_spent, status} 
		pcb[i].pid = i; pcb[i].priority = pri; pcb[i].submit_time = *time_in; pcb[i].start_time = 0; 
		pcb[i].end_time = 0; pcb[i].time_left = (time_needed * (double).000001);pcb[i].spriority = pri;
		pcb[i].burst_time = 0; pcb[i].time_spent = 0; pcb[i].status = IN_QUEUE;
		printf("New job: %i submitted with priority: %i requiring %11.9f seconds CPU time.\n", pcb[i].pid, pcb[i].priority, pcb[i].time_left);
		*number_waiting = (*number_waiting + 1);
		// set clock to represent time passing between spawnings
		set_clock((time_adv * .00001));
		// bit vector setting (not used)
		SetBit(A, i);
	}
	for(i = 1; i <= up; i++){
		if(TestBit(A, i))
			printf("Process %i stored in bit vector.\n", i);
	}
}
/* End create queue */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Context switch */
void switch_job(int new_job_nr){
	if(*context == 1 && (*current_job == new_job_nr)){ // switching current job for current job (nothing to do)
		printf("\nNo context switch. Job %i was picked to run again.\n", pcb[new_job_nr].pid);
		return;
	}
	else if(*context == 1){
		printf("\nContext switch taking place. Job %i is now running in place of job %i.\n", pcb[new_job_nr].pid, pcb[*current_job].pid);
		*current_job = new_job_nr;
		*context = 1;
	}
	else{
		*current_job = new_job_nr;
		*context = 1;
	}
}
/* End context switch */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find shortest job in queue 1 */
int find_shortest_q1(){
	int i;
	double shortest = DBL_MAX;
	int shortest_index = INT_MAX;
	for(i = 1; i <= up; i++){
		if(pcb[i].status == IN_QUEUE && pcb[i].time_left < shortest && pcb[i].time_left > 0 && pcb[i].priority == 1 && pcb[i].submit_time <= current_time->total){
			shortest = pcb[i].time_left;
			shortest_index = i;
		}
	}
return shortest_index;
}
/* End find shortest job in queue 1 */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find shortest job in queue 2 */
int find_shortest_q2(){
	int i;
	double shortest = DBL_MAX;
	int shortest_index = INT_MAX;
	for(i = 1; i <= up; i++){
		if(pcb[i].status == IN_QUEUE && pcb[i].time_left < shortest && pcb[i].time_left > 0 && pcb[i].priority == 2 && pcb[i].submit_time <= current_time->total){
			shortest = pcb[i].time_left;
			shortest_index = i;
		}
	}
return shortest_index;
}
/* End find shortest job in queue 2 */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find shortest job in queue 3 */
int find_rr_index(){
	int i;
	int rr_index = INT_MAX;
	for(i = up; i >= 1; i--){
		if(pcb[i].priority == 3 && pcb[i].status == IN_QUEUE && pcb[i].submit_time <= current_time->total)
		rr_index = i;
	}
return rr_index;
}
/* End find shortest job in queue 3 */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* End Job/Queue functions */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Scheduling algorithm */
void schedule(){
	//run while active processes remain
	while(*number_waiting > 0){
		// SJF algorithm for queue 1
		if(*turn_to == 0){
			int shortest_index;
			// find shortest job's index
			shortest_index = find_shortest_q1();
			// if no job was found in queue INT_MAX is returned
			if(shortest_index < INT_MAX){
				//perform context switch
				switch_job(shortest_index);
				// assign time slice
				pcb[shortest_index].quantum = 45;
				printf("\nRunning shortest index in queue 1: %i with a submit time of %11.9f and time requirement of %11.9f\n", 
					shortest_index, pcb[shortest_index].submit_time, pcb[shortest_index].time_left);
				// change status to indicate now running
				pcb[shortest_index].status = IN_PROGRESS;
				// micro sleep to synchronize terminal output for readability
				usleep(95000);
				// increment clock to reflect running time
				set_clock(pcb[shortest_index].burst_time);
				// consider partial cycles I/O and drop priority of I/O bound
				if((pcb[shortest_index].cycles == 2))
					pcb[shortest_index].priority++;
				// give turn to next queue since project requirements specified we go 1->2->3...
				*turn_to = 1;
			}
			// skip turn if INT_MAX returned indicating no more jobs with this priority
			else
				*turn_to = 1;
		}
		// SJF algorithm for queue 2
		// steps are all similar to queue 1, so comments are omitted
		if(*turn_to == 1){
			int shortest_index;
			shortest_index = find_shortest_q2();
			if(shortest_index < INT_MAX){
				switch_job(shortest_index);
				pcb[shortest_index].quantum = 35;
				printf("\nRunning shortest index in queue 2: %i with a submit time of %11.9f and time requirement of %11.9f\n", 
					shortest_index, pcb[shortest_index].submit_time, pcb[shortest_index].time_left);
				pcb[shortest_index].status = IN_PROGRESS;
				usleep(95000);
				set_clock(pcb[shortest_index].burst_time);
				// consider partial cycles I/O and drop priority of I/O bound
				if((pcb[shortest_index].cycles == 3))
					pcb[shortest_index].priority++;
				// promote CPU bound
				if(pcb[shortest_index].cycles < 3)
					pcb[shortest_index].priority--;
				*turn_to = 2;
			}
			else
				*turn_to = 2;
		}
		// Round Robin algorithm for queue 3
		if (*turn_to == 2){
			int rr_index;
			// find round robin priority job
			rr_index = find_rr_index();
			// INT_MAX returned if no RR jobs left
			if(rr_index < INT_MAX){
				// perform context switch
				switch_job(rr_index);
				// assign time slice
				pcb[rr_index].quantum = 15;
				printf("\nRunning shortest index in queue 3: %i with a submit time of %11.9f and time requirement of %11.9f\n", 
					rr_index, pcb[rr_index].submit_time, pcb[rr_index].time_left);
				// change status to indicate now running
				pcb[rr_index].status = IN_PROGRESS;
				// sleep to synchronize terminal output
				usleep(95000);
				// set clock to reflect time spent on job
				set_clock(pcb[rr_index].burst_time);
				// promote CPU bound
				if(pcb[rr_index].cycles == 1)
					pcb[rr_index].priority--;
				// give turn back to top queue
				*turn_to = 0;
			}
			else 
				*turn_to = 0;
		}
	}
}
/* End scheduling algorithm */
////////////////////////////////////////////////////////////////////////////////////////////////////
