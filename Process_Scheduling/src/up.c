/* $Author: o3-lael $
 * $Date: 2016/03/23 20:11:08 $
 * $Log: up.c,v $
 * Revision 4.5  2016/03/23 20:11:08  o3-lael
 * comment on clock increment
 *
 * Revision 4.4  2016/03/23 20:00:58  o3-lael
 * fixing timer requirement
 *
 * Revision 4.2  2016/03/21 16:02:52  o3-lael
 * useless junk cleared out and arguments changed to reflect removals
 *
 * Revision 4.1  2016/03/21 15:24:44  o3-lael
 * project functional.
 * branching for final cleanup and release
 *
 * Revision 3.10  2016/03/21 13:15:24  o3-lael
 * sleep used for development shortened for release
 * re-prioritization implemented
 *
 * Revision 3.9  2016/03/20 20:26:40  o3-lael
 * working
 *
 * Revision 3.7  2016/03/20 17:30:36  o3-lael
 * fixed random timing issue that was my doing
 *
 * Revision 3.6  2016/03/20 15:20:57  o3-lael
 * almost working
 * last process in hanging and stats not running
 *
 * Revision 3.5  2016/03/19 19:40:03  o3-lael
 * *** empty log message ***
 *
 * Revision 3.4  2016/03/19 17:17:47  o3-lael
 * working on oss/up relationship
 *
 * Revision 3.3  2016/03/18 19:57:53  o3-lael
 * passed variables and user process added
 *
 * $State: Exp $
 * $Revision: 4.5 $
*/
#include "up.h"

int main(int argc, char **argv){
	int quantum_use;
	if(argc < 2){
		fprintf(stderr,"\n*******************************************************************************\n");
                fprintf(stderr, "* %s is intended to be called by master with arguments passed from master. *\n", argv[0]);
                fprintf(stderr,"*******************************************************************************\n\n");
        exit(1);
        }
	// install signal handler
	signal(SIGINT, ctrlCHandler);
	// seed random
	srand(time(NULL));
//////////////////// PASSED ARGUMENTS //////////////////////////////////////////
	// user process number
	who = (atoi(argv[1]) + 1);
	// user process PID
	myId = atoi(argv[2]);
	// shared memory ids
	shmem_id = atoi(argv[3]);	// PCBs
	shmem2_id = atoi(argv[4]);	// number_waiting
	shmem3_id = atoi(argv[5]);	// clock
///////////////////// END PASSED ARGUMENTS //////////////////////////////////////	
	setup_mem();
	// wait your turn
wait:	while(pcb[who].status != IN_PROGRESS){
	}
	// when it's your turn
	while(pcb[who].status == IN_PROGRESS){
		// skip is a first run of loop indicator (0 indicates first run of loop)
		int skip = 0;
		// quantum_use is a random (0/1) number that indicates if the entire quantum will be used (1) or not (0).
		quantum_use = (rand() % 2);
		// if first run and use full quantum and quantum is less than process time left
		if(skip == 0 && quantum_use == 1 && ((pcb[who].quantum * .001) < pcb[who].time_left)){
			fprintf(stderr, "Job %i running full quantum %11.9f < time requirement with PID %i.\n", who, (pcb[who].quantum * .001), myId);
			// start the clock on process if it hasn't run before
			if(pcb[who].start_time == 0)
				pcb[who].start_time = current_time->total;
			// set this burst to equal quantum amount
			pcb[who].burst_time = (pcb[who].quantum * .001);
			// subtract the quantum/burst from total amount of time left
			pcb[who].time_left -= (pcb[who].quantum * .001);
			// add the quantum to the time spent on process
			pcb[who].time_spent += (pcb[who].quantum * .001);
			// place back in line
			pcb[who].status = IN_QUEUE;
			// increment number of processor cycles to facilitate promotion/demotion
			pcb[who].cycles++;
			// indicate run has taken place in this loop
			skip = 1;
			// return to waiting status
			goto wait;
		}
		// if first run and partial quantum use and quantum is less than time left
		if(skip == 0 && quantum_use == 0 && ((pcb[who].quantum * .001) < pcb[who].time_left)){
			// calculate and store random amount of quantum to use
			int partial = (rand() % pcb[who].quantum);
			fprintf(stderr, "Job %i running partial quantum %11.9f < time requirement with PID %i.\n", who, (pcb[who].quantum * .001), myId);
			// start the clock on process if it hasn't run before
			if(pcb[who].start_time == 0)
				pcb[who].start_time = current_time->total;
			// set this burst to equal partial quantum amount
			pcb[who].burst_time = (partial * .001);
			// subtract the partial quantum/burst from total amount of time left
			pcb[who].time_left -= (partial * .001);
			// add the partial quantum to the time spent on process
			pcb[who].time_spent += (partial * .001);
			// place back in line
			pcb[who].status = IN_QUEUE;
			// increment number of processor cycles to facilitate promotion/demotion
			pcb[who].cycles++;
			// indicate run has taken place in this loop
			skip = 1;
			// return to waiting status
			goto wait;
		}
		// if first run and use full quantum and quantum is greater than time left
		if(skip == 0 && quantum_use == 1 && ((pcb[who].quantum * .001) >= pcb[who].time_left)){
			fprintf(stderr, "Job %i running full quantum %11.9f >= time requirement with PID %i.\n", who, (pcb[who].quantum * .001), myId);
			// start the clock on process if it hasn't run before
			if(pcb[who].start_time == 0)
				pcb[who].start_time = current_time->total;
			// set burst time equal to remaining time since we will finish and relinquish processor control
			pcb[who].burst_time = pcb[who].time_left;
			// add time left to time spent
			pcb[who].time_spent += pcb[who].time_left;
			// set time left = 0
			pcb[who].time_left = 0;
			// mark as complete
			pcb[who].status = COMPLETE;
			// store ending time
			pcb[who].end_time = current_time->total;
			// decrement number of processes in line
			*number_waiting = (*number_waiting - 1);
			// indicate run has taken place this loop
			skip = 1;
		}
		// if first run and partial quantum use is greater than amount of time left
		if(skip == 0 && quantum_use == 0 && ((pcb[who].quantum * .001) >= pcb[who].time_left)){
			// calculate and store partial quantum amount
			int partial = (rand() % pcb[who].quantum);
			fprintf(stderr, "Job %i running partial quantum %11.9f >= time requirement with PID %i.\n", who, (pcb[who].quantum * .001), myId);
			// if partial amount end up less than time left
			if((partial * .001) < pcb[who].time_left){	
				// start the clock on process if it hasn't run before
				if(pcb[who].start_time == 0)
					pcb[who].start_time = current_time->total;
				// set burst time equal to partial amount
				pcb[who].burst_time = (partial * .001);
				// subtract partial amount from time left
				pcb[who].time_left -= (partial * .001);
				// add partial amount to total time spent
				pcb[who].time_spent += (partial * .001);
				// place back in line
				pcb[who].status = IN_QUEUE;
				// increment number of processor cycles
				pcb[who].cycles++;
				// indicate run has taken place
				skip = 1;
				// go back to waiting condition
				goto wait;
			}
			else{ // if partial amount ends up more than time left
				// start the clock on process if it hasn't run before
				if(pcb[who].start_time == 0)
					pcb[who].start_time = current_time->total;
				// set burst time equal to amount of time left
				pcb[who].burst_time = pcb[who].time_left;
				// add time left to total time spent
				pcb[who].time_spent += pcb[who].time_left;
				// set time left equal to zero
				pcb[who].time_left = 0;
				// mark as complete
				pcb[who].status = COMPLETE;
				// record ending time
				pcb[who].end_time = current_time->total;
				// decrement number of waiting processes
				*number_waiting = (*number_waiting - 1);
				// indicate run has taken place
				skip = 1;
			}
		}
		// reset skip for start of next loop
		skip = 0;
		// simulate random [0,1000] ns overhead
		increment_clock();
	}
return 0;
}

void setup_mem(){
	// attach to PCB
	if((pcb = (PCB *)shmat(shmem_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to PCB shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to PCB shared memory segment.\n", who, myId);
	

	// attach to number_waiting 
	if((number_waiting = (int *)shmat(shmem2_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to number_waiting shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to number_waiting shared memory segment.\n", who, myId);
	

	// attach to clock
	if((current_time = (ClockTime *)shmat(shmem3_id, NULL, 0)) == (void *) -1){
		perror("Failed to attach to monitor shared memory segment.\n");
	exit(1);
	}
	else
		printf("User process %i with PID %i now attached to clock shared memory segment.\n", who, myId);
}		

// Ctrl-C handler
void ctrlCHandler(int sig){
	// sleep added to clean up message display order
	sleep(1);
	fprintf(stderr, "\nSIGINT detected in the child process. Process %i is dying.\n", getpid());
exit(0);
}
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
