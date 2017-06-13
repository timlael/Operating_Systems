/* $Author: o3-lael $
 * $Date: 2016/04/08 20:58:33 $
 * $Log: monitorlib.c,v $
 * Revision 2.1  2016/04/08 20:58:33  o3-lael
 * base project working correctly again
 * branching before resource allocation implementation
 *
 * Revision 1.4  2016/04/08 20:50:02  o3-lael
 * changes to implement shared header file 'shared.h'
 *
 * Revision 1.3  2016/04/03 16:35:13  o3-lael
 * copied over from p3
 *
 * $State: Exp $
 * $Revision: 2.1 $
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

// private signaling functions
static void Signal_Sem(int, int);
static void Wait_Sem(int, int);

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
