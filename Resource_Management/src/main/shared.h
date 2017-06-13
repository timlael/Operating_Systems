#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "../monitor/monitorlib.c"

// Time struct accessible to all through shmem
typedef struct {
	double sec;
	double ns;
	double total;
} ClockTime;

// Resource descriptor struct
typedef struct resource_desc{
	int resource_id;	// unique integer id
	int type;			// 1 - sharable; 0 - private
	int total;			// total number of resource type available
	int avail;			// currently available
	int max;
	int alloc;
	int need;
	int available;
} r_desc;

// entry condition
int *entry;
// shmem segment ids
int segment_id;
int segment2_id;
int segment3_id;
int segment4_id;
int segment5_id;
// semaphore ids
int sem1_id;
int sem2_id;
// Monitor
Monitor *monitor;
// max user processes (default 18)
// configurable for testing purposes
int up = 18;
// PID holder for oss identification
pid_t oss_pid;
// PID holder for child identification
pid_t up_pid;
// Resource descriptor block 
r_desc *rdb;
// Time pointer
ClockTime *current_time;
// pointer for output file and default filename
FILE *logname = NULL;
char *fname = "cstest";
double *time_in;	// process create/submit time
// loop counters
int i, j;
// resource matricies and deadlock avoidance tools
int **max;
int **alloc;
int **need;
int *avail;
int *mark;
int flag;
int temp;
// function prototypes
void ctrlCHandler(int signal);
void cleanup();
void parseCmd(int argc, char **argv, int *up, char **fname);
void setup_mem();
void print_stats();
void create_resources();
void claim_resources(int who);
void init_cs(Monitor *m, int *e_cond);
void exit_cs(Monitor *m, int *e_cond);
void increment_clock();
void adv_clock(double time);
void safe_check();
void calc_need();
