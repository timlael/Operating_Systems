/* $Author: o3-lael $
 * $Date: 2016/03/09 20:03:28 $
 * $Log: monitor.h,v $
 * Revision 5.3  2016/03/09 20:03:28  o3-lael
 * added comments to new and/or uncommented sections
 *
 * Revision 5.2  2016/03/09 19:17:04  o3-lael
 * *** empty log message ***
 *
 * Revision 5.1  2016/03/09 19:13:56  o3-lael
 * folder structure and associated makefile implemented
 *
 * Revision 4.1  2016/03/09 19:06:49  o3-lael
 * *** empty log message ***
 *
 * Revision 3.1  2016/03/09 18:10:47  o3-lael
 * monitor implemntation corrected to reflect memory changes
 * which were the root of all my problems
 *
 * Revision 2.3  2016/03/07 20:29:15  o3-lael
 * monitor library nearly working
 *
 * Revision 2.2  2016/03/07 18:53:00  o3-lael
 * *** empty log message ***
 *
 * Revision 2.1  2016/03/07 15:31:45  o3-lael
 * Branch to rev. 2
 * clean break from old strategy. Now implementing monitor from class notes
 * monitor.h has new monitor class and functions. Also has semaphore prototypes
 *
 * Revision 1.4  2016/03/07 14:55:40  o3-lael
 * implementing Monitoe class to reflect notes
 *
 * Revision 1.3  2016/03/06 19:02:14  o3-lael
 * passing properties struct via shmem rather than
 * passing multiple variables via multiple segments
 *
 * Revision 1.2  2016/03/06 16:25:56  o3-lael
 * cleaned up RCS headers and added RCS keywords where missing
 * headers were funky on copy from project 2 since I didn't clean them up correctly before copy
 *
 * $State: Exp $
 * $Revision: 5.3 $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

// Monitor class
typedef struct Monitor{
	int mutex;
	int num_conditions;
	int sem_id;
	int *condition_cnt;
	int shmem_id;
} Monitor;

// function prototypes
void init_monitor(Monitor*);
void enter_monitor(Monitor*);
void wait_condition(Monitor*, int *e_cond);
void signal_condition(Monitor*, int *e_cond);
