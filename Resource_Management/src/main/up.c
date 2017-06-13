/*$Author: o3-lael $
  $Date: 2016/04/10 16:00:39 $
  $Log: up.c,v $
  Revision 2.5  2016/04/10 16:00:39  o3-lael
  deadlock avoidance algorithm implemented
  matricies for avoidance implemented and memory allocation attempted

  Revision 2.4  2016/04/09 14:49:24  o3-lael
  changed resource claiming scheme

  Revision 2.3  2016/04/09 02:42:34  o3-lael
  adjusted inter-fork timing

  Revision 2.2  2016/04/09 00:09:32  o3-lael
  created and implemented claim resources function

  Revision 2.1  2016/04/08 20:58:33  o3-lael
  base project working correctly again
  branching before resource allocation implementation

  Revision 1.9  2016/04/08 20:50:02  o3-lael
  shared header file implemented
  error in time advance function fixed... now working

  Revision 1.8  2016/04/04 21:25:16  o3-lael
  fixed datatypes for clock which were causing errors due to mismatch

  Revision 1.7  2016/04/03 22:56:44  o3-lael
  added resource descriptors to shared memory

  Revision 1.6  2016/04/03 19:56:38  o3-lael
  trimmed out project 4 items and synchronized arguments to match

  Revision 1.5  2016/04/03 17:00:42  o3-lael
  clock/critical section implemented

  Revision 1.4  2016/04/03 16:35:13  o3-lael
  working on monitor code

  Revision 1.3  2016/03/29 15:33:53  o3-lael
  adding monitor/semaphore implementation to solve critical section issue

  Revision 1.2  2016/03/27 19:06:47  o3-lael
  cleaned up comments

  Revision 1.1  2016/03/27 15:35:39  o3-lael
  Initial revision

  $State: Exp $ 
  $Revision: 2.5 $
*/
#include "shared.h"

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr,"\n*******************************************************************************\n");
                fprintf(stderr, "*   %s is intended to be called by the oss with arguments passed from oss.    *\n", argv[0]);
                fprintf(stderr,"*******************************************************************************\n\n");
        exit(1);
	}
	srand(time(NULL) ^ (getpid()<<16));
	Monitor *monitor;
	// install signal handler
	signal(SIGINT, ctrlCHandler);
	// seed random
//////////////////// PASSED ARGUMENTS //////////////////////////////////////////
	// user process number
	int who = (atoi(argv[1]) + 1);
	// user process PID
	int myId = atoi(argv[2]);
	// shared memory ids
	int shmem_id = atoi(argv[3]);	// resource descriptors
	int shmem2_id = atoi(argv[4]);	// clock
	int shmem3_id = atoi(argv[5]);	// monitor
	int shmem4_id = atoi(argv[6]);	// entry condition
	int shmem5_id = atoi(argv[7]);	// max matrix
///////////////////// END PASSED ARGUMENTS //////////////////////////////////////	
	
	// attach to resource descriptors 
	if((rdb = (r_desc *)shmat(shmem_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to resource descriptor shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to resource descriptor shared memory segment.\n", who, myId);
	
	// attach to clock
	if((current_time = (ClockTime *)shmat(shmem2_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to clock shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to clock shared memory segment.\n", who, myId);

	// attach to monitor
	if((monitor = (Monitor *)shmat(shmem3_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to monitor shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to monitor shared memory segment.\n", who, myId);
	
	// attach to and initialize entry condition 
	if((entry = (int *)shmat(shmem4_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to entry condition shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to entry condition shared memory segment.\n", who, myId);
	
	// attach to max matrix 
	if((max = (int **)shmat(shmem4_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to max matrix shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to max matrix shared memory segment.\n", who, myId);
	
	printf("Current time is %11.9f.\n", current_time->total);
	claim_resources(who);

return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Administrative functions */
// Ctrl-C handler
void ctrlCHandler(int sig){
	// sleep added to clean up message display order
	sleep(1);
	fprintf(stderr, "\nSIGINT detected in the child process. Process %i is dying.\n", getpid());
exit(0);
}
/* End administrative functions */
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
/* END LOGICAL CLOCK IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* RESOURCE OPERATIONS */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Make initial claims */
void claim_resources(int who){
	// loop where each process will ask for 3 resources
	for(i = 1; i < 4; i++){
		// random resource number (id) to be asked for
		int p_no = (rand() % 20) + 1;
		// if resource is shared (1) claim a shared instance (no need to check availability)
		if(rdb[p_no].type == 1){
			printf("Child %i claiming shared resource %i\n", who, p_no);
			rdb[p_no].max++;
			// max[who][p_no]++;
		}
		// if resource is limited check to make sure claim does not exceed current availability
		else if(rdb[p_no].type == 0 && rdb[p_no].avail > 0){
			// random count of resource id to ask for
			int p_cnt = ((rand() % rdb[p_no].avail) + 1);
			printf("Child %i claiming %i units of resource %i\n", who, p_cnt, p_no);
			rdb[p_no].max += p_cnt;
			// max[who][p_no] += p_cnt;
			// decrease availability base upon requests
			rdb[p_no].avail -= p_cnt;
		}
		// re-enter loop and try again
		else
			i--;
	}
}
/* End make initial claims */
////////////////////////////////////////////////////////////////////////////////////////////////////
