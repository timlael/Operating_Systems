/* $Author: o3-lael $
 * $Date: 2016/02/20 15:43:29 $
 * $Log: slave.h,v $
 * Revision 4.1  2016/02/20 15:43:29  o3-lael
 * final branch ready to turn in
 * cleaned up punctuation in messages
 *
 * Revision 3.3  2016/02/19 17:14:37  o3-lael
 * commented out libraried that were unused in the final product
 *
 * Revision 3.2  2016/02/19 01:17:27  o3-lael
 * fixed timestamp issue in slave.c
 * now ready for release
 *
 * Revision 3.1  2016/02/19 00:58:31  o3-lael
 * Branch to release
 *
 * Revision 2.2  2016/02/18 18:54:04  o3-lael
 * general tune up and comments
 * also fixed message printed to file from processes to match specified time format
 *
 * Revision 2.1  2016/02/18 14:57:52  o3-lael
 * Branch
 * Program is functionally complete
 * Now it is time to clean up and fine tune
 *
 * Revision 1.3  2016/02/17 21:35:09  o3-lael
 * everything is working minus time stamp
 *
 * Revision 1.2  2016/02/17 03:56:13  o3-lael
 * everything is in place and working except for peterson block
 * code is there, but critical section is not executing
 *
 * Revision 1.1  2016/02/16 22:04:25  o3-lael
 * Initial revision
 *
 * $State: Exp $
 * $Revision: 4.1 $
*/
//#include <ctype.h>
//#include <sys/ipc.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum state {IDLE, WANT_IN, IN_CS};
// time for srand()
time_t t;
// variable for indicating whos turn it is
int turn = 0;
// loop variables
int j = 0;
int i = 0;
// pointer for output file and default filename
FILE *logname = NULL;
char *filename = "cstest";
// Shared memory flag poointer
int *sFlag = 0;

// function prototypes
void ctrlCHandler(int sig);
int critical_sec(FILE *logname, int who);
int setup_exc(int *sFlag, int who, int slaves, int j, int turn, FILE *logname);
int handoff(int *sFlag, int who, int slaves, int j, int turn);
