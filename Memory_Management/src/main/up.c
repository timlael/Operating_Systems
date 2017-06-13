/*$Author: o3-lael $
  $Date: 2016/04/28 02:29:59 $
  $Log: up.c,v $
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

#include "up.h"


int myIndex;
int numFaults;
int numReqs;
int maxAddr;
ClockTime age;
PageTable* myTable;
Shared *s;

int main(int argc, char **argv) {
	signal(SIGINT, SIG_IGN);
	if(argc < 2){
		fprintf(stderr,"\n");
		fprintf(stderr,"*******************************************************************************\n");
		fprintf(stderr,"*  %s is intended to be called by the oss with arguments passed from oss.   *\n", argv[0]);
		fprintf(stderr,"*******************************************************************************\n");
		fprintf(stderr,"\n");
				exit(1);
	}
	setSharedMemory();
	setSigHandler(*sigHandler);
	myIndex = atoi(argv[1]);
	myTable = &s->tables[myIndex];
	if(myIndex != 99)
		printf("Generated new process: %i.\n", (myIndex + 1));

	srand(time(NULL) ^ (getpid() << 16));
	myTable->numPages = rand() % PAGE_LIMIT + 1;
	maxAddr = myTable->numPages * 1024;

	age.total = s->current_time.total;


	s->pFlag[myIndex] = 1;
	s->child[myIndex] = getpid();

	run();

	return 0;
}

// container function to run user process tasks
void run() {
	int numReqs = rand() % 100 + 1000;
	int i = 0;
	for (i = 0; i < numReqs; i++) {
		genRequest();
		semWait(s->semno, myIndex); 
		if (i == numReqs - 1) {
			if (rand() % 3 != 1) {
				i = 0;
				numReqs = rand() % 100 + 1000;
			}
		}
	}
	cleanup();
}

// generate memory requests with random read/write attribute
void genRequest() {
	int rand_typ = (rand() % 2);
	if (rand_typ == 1){
		s->req[myIndex].write = 1;
		s->req[myIndex].read = 0;
	}
	else{
		s->req[myIndex].read = 1;
		s->req[myIndex].write = 0;
	}


	int address = rand() % (maxAddr + 1);
	s->req[myIndex].address = address;
	s->req[myIndex].flag = 1;

	int page = address / 1024;
	if (myTable->table[page] == -1) {
		numFaults++; 
		s->numFaults++; 
	}
	numReqs++; 
	s->numReqs++; 
}

// user process graceful/normal cleanup function
void cleanup() {
	s->pFlag[myIndex] = 2;
	printf("User Process %i gracefully exiting.\n", (myIndex + 1));
	exit(0);
}

// user process forced exit cleanup function
void fcleanup() {
	s->pFlag[myIndex] = 2;
	printf("User Process %i forcefully exiting.\n", (myIndex + 1));
	exit(0);
}

// user process signal handler
void sigHandler(int signo) {
	if (signo == SIGINT) {
		return;
	}
	else if (signo == SIGALRM) {
		printf("User Process %i Killed.\n", (myIndex + 1));
		fcleanup();
	}
	else if (signo == SIGUSR1) {
		printf("User Process %i Killed.\n", (myIndex + 1));
		fcleanup();
	}
}
