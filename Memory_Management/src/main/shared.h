/*$Author: o3-lael $
  $Date: 2016/04/28 02:29:59 $
  $Log: shared.h,v $
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
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#ifndef MAX_UP
#define MAX_UP 18 
#endif

#ifndef UP
#define UP 12 
#endif

#ifndef TOTAL_MEM
#define TOTAL_MEM 256 
#endif

#ifndef PAGE_LIMIT
#define PAGE_LIMIT 32 
#endif

#ifndef FORK_TIME
#define FORK_TIME 20 
#endif

#ifndef SHARED_CLOCK_H
#define SHARED_CLOCK_H
typedef struct{
	double ns;
	double sec;
	double total;
}ClockTime;
#endif


#ifndef PAGETABLE_H
#define PAGETABLE_H
typedef struct {
	int numPages;
	int table[PAGE_LIMIT];
}PageTable;
#endif

#ifndef REQUEST_H
#define REQUEST_H
typedef struct {
	int address; 
	int flag; 
	int read;
	int write;
}Request;
#endif

#ifndef TLB_H
#define TLB_H
typedef struct {
	double lru[TOTAL_MEM]; 
	int dirty[TOTAL_MEM]; 
	int frames[TOTAL_MEM]; 
	int head;
}TLB;
#endif

#ifndef REQEST_QUEUE_H
#define REQEST_QUEUE_H
typedef struct {
	double wait[MAX_UP]; 
	int head;
	int req[MAX_UP];
}RequestQueue;
#endif

#ifndef SHARED_H
#define SHARED_H
typedef struct {
	int numReqs;
	int numFaults; 
	int pFlag[MAX_UP]; 
	int semno; 
	ClockTime current_time; 
	PageTable tables[MAX_UP];
	pid_t child[MAX_UP];
	Request req[MAX_UP];
}Shared;
#endif

double mstons(double ms);
double nstoms(double ns);
int setSharedMemory();
void semSig(int, int);
void semWait(int, int);
void setSigHandler( void handler() );
