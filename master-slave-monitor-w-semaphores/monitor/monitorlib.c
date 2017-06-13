/* $Author: o3-lael $
 * $Date: 2016/03/09 20:03:28 $
 * $Log: monitorlib.c,v $
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
 * monitorlib currently has semaphore init and wait/signal functions complete
 *
 * Revision 1.6  2016/03/07 14:55:40  o3-lael
 * implementing Monitoe class to reflect notes
 *
 * Revision 1.5  2016/03/06 21:44:45  o3-lael
 * beginnins of monitor implemenation
 *
 * Revision 1.4  2016/03/06 19:02:14  o3-lael
 * passing properties struct via shmem rather than
 * passing multiple variables via multiple segments
 *
 * Revision 1.3  2016/03/06 16:25:56  o3-lael
 * cleaned up RCS headers and added RCS keywords where missing
 * headers were funky on copy from project 2 since I didn't clean them up correctly before copy
 *
 * $State: Exp $
 * $Revision: 5.3 $
*/

#include "monitor.h"
// private signaling functions
static void Wait_Sem(int, int);
static void Signal_Sem(int, int);

// monitor initialization function
void init_monitor(Monitor* m){
	//for future flexibility
	m->num_conditions = 1;
    	// get semaphore for mutex
    	m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0644);
	// get semaphore for process
	m->sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0644);
	//set up semaphores
	semctl(m->mutex, 0, SETVAL, 1);
	semctl(m->sem_id, 0, SETVAL, 0);
	// waiting list
	m->condition_cnt = (int*) (shmat(m->shmem_id, 0, 0));
	m->condition_cnt = 0;
}

// monitor entry
void enter_monitor(Monitor* m){
     Wait_Sem(m->mutex, 1);
}

// wait for entry condition
void wait_condition(Monitor* m, int *e_cond){
      m->condition_cnt++;
      Signal_Sem(m->mutex, 0);
      Wait_Sem(m->sem_id, 0);
      m->condition_cnt--;
}

// signal entry condition
void signal_condition(Monitor* m, int *e_cond){
    if(m->condition_cnt > 0) {
            Signal_Sem(m->sem_id, 0);
    }
    else {
            Signal_Sem(m->mutex, 0);
    }
}

// wait semaphore properties
void Wait_Sem(int id_sem, int var){
       struct sembuf sem_buf;
       sem_buf.sem_num = 0;
       sem_buf.sem_flg = 0;
       sem_buf.sem_op = -1;
       semop(id_sem, &sem_buf, 1);
}

// signal semaphore properties
void Signal_Sem(int id_sem, int var){
       struct sembuf sem_buf;
       sem_buf.sem_num = 0;
       sem_buf.sem_flg = 0;
       sem_buf.sem_op = 1;
       semop(id_sem, &sem_buf, 1);
}
