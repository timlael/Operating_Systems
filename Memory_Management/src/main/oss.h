/*$Author: o3-lael $
  $Date: 2016/04/28 02:29:59 $
  $Log: oss.h,v $
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

int detachandremove(int, void*);
int firstSlotOpen();
int initSem(int, int, int);
int numProcInSys();
pid_t r_wait(int*);
void adv_clock(double);
void checkForDead();
void checkFork();
void checkForReq();
void checkReqQueue();
void cleanup();
void doPageSwap(int);
void freeChildMemory(int);
void fork_next(int);
void init();
void init_queues();
void init_shmem();
void pageSwap(int);
void parseCmd(int argc, char **argv, int *time, int *up);
void processRequest(int);
void printResults();
void queueRequest(int);
void run();
void sendSignal(int);
void sigHandler(int);
