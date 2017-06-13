/* $Author: o3-lael $
 * $Date: 2016/03/23 20:00:58 $
 * $Log: up.h,v $
 * Revision 4.4  2016/03/23 20:00:58  o3-lael
 * fixing timer requirement
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
 * Revision 3.7  2016/03/21 13:15:24  o3-lael
 * sleep used for development shortened for release
 * re-prioritization implemented
 *
 * Revision 3.6  2016/03/20 20:26:40  o3-lael
 * working
 *
 * Revision 3.4  2016/03/20 15:20:57  o3-lael
 * almost working
 * last process in hanging and stats not running
 *
 * Revision 3.3  2016/03/18 19:57:53  o3-lael
 * passed variables and user process added
 *
 * $State: Exp $
 * $Revision: 4.4 $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
//#include "../monitor/monitor.h"

// Job status
enum status {IN_QUEUE, IN_PROGRESS, COMPLETE};
// number_waiting is a tagalong not directly relevant to PCB
// that is used/accessed by various algorithms/functions.
// Since these same algorithms/functions access job 
// properties also, it justifies placemnt of number_waiting.
typedef struct pcb_node{
	int pid;		// proccess id;
	int priority; 		// process priority
	int spriority; 		// process priority
	double submit_time; 	// used for calulating total time in system
	double start_time;  	// used for calculating burst time
	double end_time; 	// used for calculating burst/total time
	double time_left;	// used for shortest job first scheduling
	double burst_time;// burst time
	double time_spent;// aggregate time
	int cycles;
	int status; 		// IN_QUEUE/IN_PROGRESS/COMPLETE enum
	int quantum;
} PCB;

// Time struct accessible to all through shmem
typedef struct {
	unsigned int sec;
	unsigned int ns;
	double total;
} ClockTime;

// shmem segment ids for above items
int shmem_id;
int shmem2_id;
int shmem3_id;
// max user processes (default 19)
// configurable for testing purposes
int who;
int myId;
// PCB pointers
PCB *pcb;
ClockTime *current_time;
int *number_waiting;
// loop counter
int i;
// output file pointers (cstest is default)
FILE *logname = NULL;
char *filename = "cstest";
// clock variables
double *sec;
double *ns;
double *rand_ns;
double *total;

// function prototypes
void setup_mem();
void ctrlCHandler(int sig);
void increment_clock();
