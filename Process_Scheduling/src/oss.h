/* $Author: o3-lael $
 * $Date: 2016/03/21 21:28:25 $
 * $Log: oss.h,v $
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
 * Revision 3.15  2016/03/21 13:15:24  o3-lael
 * sleep used for development shortened for release
 * re-prioritization implemented
 *
 * Revision 3.14  2016/03/20 20:26:40  o3-lael
 * working
 *
 * Revision 3.12  2016/03/19 19:40:03  o3-lael
 * *** empty log message ***
 *
 * Revision 3.11  2016/03/19 17:17:47  o3-lael
 * working on oss/up relationship
 *
 * Revision 3.10  2016/03/18 19:57:53  o3-lael
 * execl and shared memory items added
 *
 * Revision 3.9  2016/03/18 01:41:18  o3-lael
 * added signal.h include
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
 * Revision 2.4  2016/03/17 01:33:15  o3-lael
 * clock/increment clock finished
 *
 * Revision 2.3  2016/03/16 17:14:18  o3-lael
 * turned pcb into double linked list
 *
 * Revision 2.2  2016/03/16 15:35:13  o3-lael
 * modified header to match current file structure reflecting monitor location
 *
 * Revision 2.1  2016/03/16 15:19:08  o3-lael
 * branching to v2 and implementing forked master/slave functionality
 *
 * Revision 1.16  2016/03/16 02:14:48  o3-lael
 * working on scheduler
 *
 * Revision 1.15  2016/03/14 18:47:53  o3-lael
 * finished algorithms, running without forking... sort of
 *
 * Revision 1.14  2016/03/14 17:26:19  o3-lael
 * added SJF algorithmm and most supporting functions
 *
 * Revision 1.13  2016/03/13 19:16:01  o3-lael
 * logical clock working
 *
 * Revision 1.12  2016/03/13 18:05:16  o3-lael
 * cleaned up main and made things more modular by implementing functions
 *
 * Revision 1.11  2016/03/13 16:49:18  o3-lael
 * fixed spacing
 *
 * Revision 1.10  2016/03/13 16:46:29  o3-lael
 * added bit array macros for set, clear and test
 *
 * Revision 1.9  2016/03/13 04:01:35  o3-lael
 * *** empty log message ***
 *
 * Revision 1.8  2016/03/13 03:46:46  o3-lael
 * fixed ; error
 *
 * Revision 1.7  2016/03/12 22:59:49  o3-lael
 * added scheduling function prototypes
 * added time_left in Job type for SJF implementation
 *
 * Revision 1.6  2016/03/12 22:05:19  o3-lael
 * cleaned apscing
 *
 * Revision 1.5  2016/03/12 21:36:27  o3-lael
 * changed Job type instances from queue to job to clarifythat they are jobs
 * I initially had them named as queue items which are jobs, but that could be confusing
 *
 * Revision 1.4  2016/03/12 21:27:11  o3-lael
 * cleaned up spacing
 *
 * Revision 1.3  2016/03/12 21:18:36  o3-lael
 * added variables and function prototypes to support functionality added in oss.c
 *
 * Revision 1.2  2016/03/12 19:26:35  o3-lael
 * addition of shared memory segment ids
 * addition of handler prototype
 * initial structs defined
 *
 * Revision 1.1  2016/03/12 18:23:37  o3-lael
 * Initial revision
 *
 * $State: Exp $
 * $Revision: 4.3 $
*/

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
//#include "../monitor/monitorlib.c"
#define SetBit(A,k)     (A[(k/32)] |= (1 << (k%32)))
#define ClearBit(A,k)   (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)    (A[(k/32)] & (1 << (k%32)))

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
	double burst_time;	// burst time
	double time_spent;	// aggregate time
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
int segment_id;
int segment2_id;
int segment3_id;
// max user processes (default 19)
// configurable for testing purposes
int up = 19;
// PID holder for oss identification
pid_t oss_pid;
// PID holder for child identification
pid_t up_pid;
// PCB pointers
PCB *pcb;
// Time
ClockTime *current_time;
// pointer for output file and default filename
FILE *logname = NULL;
char *fname = "cstest";
// job priority
int priority;
// context switch tracking (0 - No, 1 - Yes)
int *context = 0;
// clock variables
double *sec;
double *ns;
double *rand_ns;
double *total;
double *time_in;
// scheduler variables
int *number_waiting;
int *current_job;
int *turn_to;
// Bit Vector
int A[1];
// loop counters
int i, j;
// function prototypes
void ctrlCHandler(int signal);
void cleanup();
void parseCmd(int argc, char **argv, int *up, char **fname);
void setup_mem();
void setup_bit_array();
void increment_clock();
void create_queue();
void switch_job(int new_job_nr);
void set_clock(double amount);
void schedule();
int find_shortest_q1();
int find_shortest_q2();
int find_rr_index();
void print_stats();
