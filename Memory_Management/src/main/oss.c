/*$Author: o3-lael $
  $Date: 2016/04/28 02:29:59 $
  $Log: oss.c,v $
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

#include "oss.h"

double fork_time;
int shmemid;
int timer = 60;
int up = UP; 
RequestQueue req;
Shared *s;
TLB tlb;


int main(int argc, char **argv) {
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>\n");
	parseCmd(argc, argv, &timer, &up);
	alarm(timer);
	if(up == 12 && timer == 60)
		printf("Program will spawn %i user processes and run for a maximum of %i seconds before terminating.\n", up, timer);
	else if(up != 18 && timer == 60)
		printf("Program will run for a maximum of %i seconds before terminating.\n", timer);
	else if(up == 12 && timer != 60)
		printf("Program will spawn %i user processes.\n", up);
	printf("<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>\n");
	init();
	run();
	cleanup();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Parse command line arguments */
// function to parse command line arguments
// uses pointer to time so that arguments can set the timer variable
void parseCmd(int argc, char **argv, int *timer, int *up){
	int c;
	opterr = 0;
	char *avalue = NULL;
	char *bvalue = NULL;

	while ((c = getopt(argc, argv, "t:u:hr")) != -1){
		switch(c){
			// switch to display help/usage of args
			case 'h':
			printf("Usage:\n");
			printf("./oss	- Runs the program with default settings.\n");
			printf("./oss -r	- Displays the README file.\n");
			printf("./oss -t n	- Runs the program for a maximum time (n). 1-60 seconds. Integers only.\n");
			printf("./oss -u n	- Spawns (n) user processes. 1-18 processes. Integers only.\n");
			printf("./oss -h	- Displays help/usage information.\n\n");
		exit(0);
		break;
		// switch to cat README to terminal
		case 'r':
			execlp("/bin/cat", "-c", "README", NULL);
		exit(0);
		break;
		// max time config switch
		case 't':
			avalue = optarg;
			// set time variable to argument after converting ascii to int
			// temp variable is to coalesce into a trunc'd int
			// to handle people who cannot provide proper integer input
			int ttemp = atoi(avalue);
			*timer = ttemp;
			// validate input is in allowable range 1-60 seconds
			if(*timer > 60 || *timer < 1){
				fprintf(stderr,"Invalid time (%s) entered. Minimum time is 1. Maximum time is 60. Integers only.\n", avalue);
				exit(1);
			}
			// prevent duplicate messaging about timer value
			else if(*timer != 60)
			printf("Program will run for %i second[s] maximum before terminating and cleaning up.\n", *timer);
		break;
		// max slaves config switch
		case 'u':
			bvalue = optarg;
			// set slaves variable to argument after converting ascii to int
			// temp variable is to coalesce into a trunc'd int
			// to handle people who cannot provide proper integer input
			int uptemp = atoi(bvalue);
			*up = uptemp;
			// validate input is in allowable range 1-60 seconds
			if(*up > 18 || *up < 1){
				fprintf(stderr,"Invalid number of use processes (%s) entered. Minimum is 1. Maximum is 18. Integers only.\n", bvalue);
				exit(1);
			}
			// prevent duplicate messaging about slave numbers
			else if(*up != 12)
			printf("Program will spawn %i user process[es].\n", *up);
		break;
		// handle missing required arguments and unknown arguments
		case'?':
			if(optopt == 't' || optopt == 'u' || optopt == 'f')
				fprintf(stderr,"Switch -%c requires an additional argument.\n", optopt);
			else if(isprint(optopt)){
				fprintf(stderr,"Invalid option -%c.\n", optopt);
				fprintf(stderr,"Use ./oss -h to display usage information.\n");
			}
		exit(1);
		}
	}
}
/* End parse command line arguments */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Initialize memory container function */
void init() {
	shmemid = setSharedMemory();
	init_queues();
	init_shmem();
	setSigHandler(*sigHandler);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Fork and run processes container function */
void run() {
	fork_next(next());
	fork_time = mstons(rand() % FORK_TIME + 1);
	for(;;) {
		checkForDead();
		checkFork();
		checkForReq();
		adv_clock(150);
		checkReqQueue();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* End/Clean and print function */
void cleanup() {
	usleep(50000);
	printResults();
	int i;
	for (i = 0; i < up; i++) {
		semctl(s->semno, i, IPC_RMID);
	}
	detachandremove(shmemid, s);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Shared memory removal function from book */
int detachandremove(int shmemid, void *shmaddr) {
	int error = 0;
	if (shmdt(shmaddr) == -1)
		error = errno;
	if ((shmctl(shmemid, IPC_RMID, NULL) == -1) && !error)
		error = errno;
	if (!error)
		return 0;
	errno = error;
	perror("Failed to detach shared memory segment.");
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Fill lookaside and request with default values */
void init_queues() {
	int i;
	for (i = 0; i < TOTAL_MEM; i++) {
		tlb.frames[i] = tlb.lru[i] = tlb.dirty[i] = 0;
	}
	tlb.head = 0;
	for (i = 0; i < MAX_UP; i++) {
		req.wait[i] = 0;
		req.req[i] = -1;
	}
	req.head = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Fill shared memory structure with default values */
void init_shmem() {
	s->current_time.sec = s->current_time.ns = 0;
	s->semno = initSem(0, 62, up);
	int i;

	s->numReqs = 0;
	s->numFaults = 0;

	for (i = 0; i < MAX_UP; i++) {
		Request *req = &s->req[i];
		PageTable *tab = &s->tables[i];
		req->write = req->read = req->address = req->flag = 0;
		int j;
		for (j = 0; j < PAGE_LIMIT; j++) {
			tab->table[j] = -1;
		}
		tab->numPages = 0;
		s->pFlag[i] = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Fork user processes */
void fork_next(int index) {
	int pid;
	s->pFlag[index] = 1;
	pid = fork();
	if (pid == -1 ) {
		perror("Failed to fork.");
		sleep(1);
	}
	else if (pid == 0) {
		char arg1[3];
		sprintf(arg1, "%i", index);
		execl("up", "up", arg1, NULL); 
		perror("Failed to exec a user process.");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find slot for new user process */
int next(){
	int i;
	for (i = 0; i <= up; i++) {
		if (s->pFlag[i] == 0)
			return i;
	}
	// garbage return handled in up.c
	return 99;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Is it time to fork and is there a free spot  */
void checkFork() {
	if (fork_time == 0) {
		if (numProcInSys() < up) {
			fork_next(next());
			fork_time = mstons(rand() % FORK_TIME + 1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Are there waiting requests to fulfill  */
void checkForReq() {
	int i;
	for (i = 0; i < up; i++) {
		if (s->req[i].flag == 1) {
			processRequest(i);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Is there a process waiting for memory  */
void checkReqQueue() {
	int index = req.req[req.head];
	if (index == -1) {
		return;
	}
	if (req.wait[req.head] <= 0) {
		pageSwap(index);
		s->req[index].flag = 0; 
		semSig(s->semno, index); 
		req.req[req.head] = -1;
		req.head = ++req.head % MAX_UP;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Is there a dead child to free  */
void checkForDead() {
	int i;
	for (i = 0; i < up; i++) {
		if (s->pFlag[i] == 2) {
			r_wait(NULL); 
			s->pFlag[i] = 0; 
			freeChildMemory(i);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Reset dead child values to default  */
void freeChildMemory(int index) {
	PageTable* tab = &s->tables[index]; 
	int i;
	for (i = 0; i < tab->numPages; i++) {
		int frame = tab->table[i]; 
		tlb.frames[frame] = 0; 
		tlb.lru[frame] = 0;
		tlb.dirty[frame] = 0;
		tab->table[i] = -1; 
	}

	tab->numPages=0;
	semctl(s->semno, index, SETVAL, 0);
	s->req[index].flag = 0; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Look for valid page to swap out  */
void pageSwap(int index) {
	for (;;) {
		if (tlb.lru[tlb.head] > tlb.lru[tlb.head + 1]) {
			tlb.head = ++tlb.head % TOTAL_MEM;
		}
		else { 
			doPageSwap(index);
			tlb.head = ++tlb.head % TOTAL_MEM;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Swap out the lru page  */
void doPageSwap(int index) {
	int i;
	for (i = 0; i < up; i++) {
		PageTable* table = &s->tables[i]; 
		int j;
		for (j = 0; j < table->numPages; j++) { 
			if (table->table[j] == tlb.head) {
				table->table[j] = -1; 
				if (tlb.dirty[tlb.head] == 1){ 
					adv_clock(mstons(15)); 
				}
			}
		}
	}
	PageTable* table = &s->tables[index]; 
	Request* req = &s->req[index];
	int virtPage = req->address/1024; 
	table->table[virtPage] = tlb.head;
	tlb.frames[tlb.head] = 1;
	tlb.lru[tlb.head] = s->current_time.total;
	tlb.dirty[tlb.head] = 0;
	s->req[index].flag = 0; 

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* process the waiting memory request  */
void processRequest(int index) {
	Request* req = &s->req[index];
	int virtPage = req->address/1024; 
	PageTable *tab = &s->tables[index]; 
	int memSlot = tab->table[virtPage];
	if (memSlot != -1) {
		tlb.lru[memSlot] = s->current_time.total;
		if (req->write == 1) {
			tlb.dirty[memSlot] = 1;
		}
		adv_clock(10);
		req->flag = 0; 
		semSig(s->semno, index); 
	}
	else {
		queueRequest(index);
		req->flag = 0; 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Place the waiting memory request into a queue to fulfill */
void queueRequest(int index) {
	int i = req.head;
	int count = 0;
	for (i = req.head; count != MAX_UP; i = (i + 1) % MAX_UP){
		if (req.req[i] == -1) {
			req.req[i] = index;
			req.wait[i] = mstons(15); 
			break;
		}
		count++;
		if (count == MAX_UP) {
			printf("Queue full.\n");
			cleanup();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* obtain and init semaphores for each process to signal on */
int initSem(int semval, int ftokIndex, int numsems) {
	int semid;
	key_t key = ftok("./main/oss.c", ftokIndex);
	errno = 0;
	if ((semid = semget(key, numsems, 0700 | IPC_CREAT)) == -1) {
		perror("Semaphore acquisition failed");
		exit(0);
	}
	int i;
	for (i = 0; i <= numsems; i++)
		semctl(semid, i, SETVAL, semval);
	return semid;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* function used to limit the max number of processes in infinite create loop */
int numProcInSys() {
	int i;
	int total = 0;
	for (i = 0; i < MAX_UP; i++) {
		if (s->pFlag[i] == 1)
			total++;
	}
	return total;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* child progress wait loop from book */
pid_t r_wait(int *stat_loc) {
	int retval;
	while (((retval = wait(stat_loc)) == -1) && (errno == EINTR));
	return retval;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Signal handlers */
void sigHandler(int signo) {
	if (signo == SIGINT) {
		printf("./oss received SIGINT - Exiting.\n");
		usleep(50000);
		sendSignal(SIGUSR1);
		cleanup();
	}
	else if (signo == SIGALRM) {
		printf("./oss time limit reached. Cleaning up and terminating.\n");
		usleep(50000);
		sendSignal(signo);
	}
	else if (signo == SIGUSR1)
		return;
	cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Send signal handlers to chillren */
void sendSignal(int signo) {
	int i;
	for (i = 0; i < up; i++) {
		if (s->pFlag[i] == 1) {
			kill(s->child[i], signo);
			r_wait(NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/* STATISTICS PRINTING */
void printResults() {
	double ns = (((s->numReqs - s->numFaults) * 10) + (s->numFaults * mstons(15))) / s->numReqs;
	double ms = nstoms(ns);
	double percent = ((double)s->numFaults / (double)s->numReqs) * 100.0;
	
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("Paging Statistics\n");
	printf("<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("Total Requests Made: \t\t%d\n", s->numReqs);
	printf("Total Number of Page Faults: \t%d\n", s->numFaults);
	printf("Page Fault Percentage: \t\t%.3f%%\n", percent);
	if (ms >= 1.0)
		printf("Access Tme Average:\t\t%8.3fms\n", ms);
	else
		printf("Access Time Average:\t\t%8.0fns\n", ns);
	printf("<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>\n");
}
/* END STATISTICS PRINTING */
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* LOGICAL CLOCK IMPLEMENTATION */
////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set clock desired amount ahead */
// amount values is passed in integer nanoseconds
void adv_clock(double time){
	// increment in rollover situation
	if((s->current_time.ns + time) >= 1000000000){
		s->current_time.sec += 1;
		s->current_time.ns = ((s->current_time.ns + time) - 1000000000);
		s->current_time.total = (s->current_time.sec + (s->current_time.ns * .000000001));
	}
	else{
		s->current_time.ns = s->current_time.ns + time;
		s->current_time.total = (s->current_time.sec + (s->current_time.ns * .000000001));
	}
	
	int i;
	int p_count = 0;
	// adjust wait time to reflect clock movement
	for (i = req.head; p_count != MAX_UP; i = (i+1) % MAX_UP) {
		if (req.req[i] >= 0 ) {
			if (req.wait[i] >= time) {
				req.wait[i] -= time;
			}
			else if (req.wait[i] != 0) {
				req.wait[i] = 0;
			}
		}
		p_count++;
	}
	if (fork_time >= time)
		fork_time -= time;
	else
		fork_time = 0;
}
/* End set clock */
////////////////////////////////////////////////////////////////////////////////////////////////////
