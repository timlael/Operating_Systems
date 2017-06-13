/*$Author: o3-lael $
  $Date: 2016/04/28 02:29:59 $
  $Log: shared.c,v $
  Revision 5.1  2016/04/28 02:29:59  o3-lael
  previous chekin note on branch 4 didn't take
  this is a recovery branch due to Hoare issues
  Hoare was unstable 26 and 27 April and user minx files were in my vim .swp
  after an immediate shutdown on 27 APR. This project is complete and a local 
  copy that was recovered.

  Revision 4.1  2016/04/28 02:24:13  o3-lael
  see note on main makefile

  $State: Exp $
  $Revision: 5.1 $
*/
#include "shared.h"

extern Shared *s;

int setSharedMemory() {
	int id;
	key_t key = ftok("./main/oss.c", 60);
	if ((id = shmget(key, (sizeof(Shared)), IPC_CREAT | 0666)) == -1) {
		perror("Failed to create shared memory segment");
		exit(1);
	}
	if ((s = (Shared *)shmat(id, NULL, 0)) == (void *)-1) {
		perror("Failed to attach to shared memory space");
		exit(1);
	}
	return id;
}

void semWait(int semid, int index) {
	struct sembuf sembuff;
	sembuff.sem_num = index;
	sembuff.sem_op = -1;
	sembuff.sem_flg = SEM_UNDO;
	semop(semid, &sembuff, 1);}

void semSig(int semid, int index) {
	struct sembuf sembuff;
	sembuff.sem_num = index;
	sembuff.sem_op = 1;
	sembuff.sem_flg = SEM_UNDO;
	semop(semid, &sembuff, 1);}

void setSigHandler( void handler() ) {
	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGALRM, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGSTOP, &act, NULL);
	sigaction(SIGABRT, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}

// convert ns to ms
double nstoms(double ns) {
	double  ms = ns / 1000000.0;
	return ms;
}

// convert ms to ns
double mstons(double ms) {
	double ns = ms * 1000000;
	return ns;
}
