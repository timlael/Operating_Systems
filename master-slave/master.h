/* $Author: o3-lael $
 * $Date: 2016/02/20 15:43:29 $
 * $Log: master.h,v $
 * Revision 4.1  2016/02/20 15:43:29  o3-lael
 * final branch ready to turn in
 *
 * Revision 3.4  2016/02/19 17:57:55  o3-lael
 * uncommented string library to account for default output location comparison
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
 * Revision 2.3  2016/02/18 18:54:04  o3-lael
 * general tune up and comments
 *
 * Revision 2.2  2016/02/18 16:23:30  o3-lael
 * adopted new timer method and implemented changes to compliment
 *
 * Revision 2.1  2016/02/18 14:57:52  o3-lael
 * Branch
 * Program is functionally complete
 * Now it is time to clean up and fine tune
 *
 * Revision 1.12  2016/02/17 21:35:09  o3-lael
 * everything is working minus time stamp
 *
 * Revision 1.11  2016/02/17 04:48:08  o3-lael
 * poking at mutex still
 *
 * Revision 1.10  2016/02/17 03:56:13  o3-lael
 * everything is in place and working except for peterson block
 * code is there, but critical section is not executing
 *
 * Revision 1.9  2016/02/16 03:07:31  o3-lael
 * implemented configuration changes
 * filename and number of slaves now controlled optionally
 * via command line switch
 *
 * Revision 1.8  2016/02/15 22:18:18  o3-lael
 * removed unused library
 *
 * Revision 1.7  2016/02/15 22:16:08  o3-lael
 * fine tuned timer command line input to handle users who attempt other than integer input
 *
 * Revision 1.6  2016/02/15 21:53:53  o3-lael
 * created a function for cleanup of memory and processes
 * implemented in normal, timeout and Ctrl-C cases
 * to reduce and stremline code
 *
 * Revision 1.5  2016/02/15 20:20:38  o3-lael
 * implemented and tested Ctrl-C signal handling
 * double stdout output at this time, but it works otherwise
 *
 * Revision 1.4  2016/02/13 22:38:14  o3-lael
 * fixed forking issues
 *
 * Revision 1.3  2016/02/13 19:11:04  o3-lael
 * *** empty log message ***
 *
 * Revision 1.1  2016/02/13 16:05:34  o3-lael
 * Initial revision
 *
 * $State: Exp $
 * $Revision: 4.1 $
*/
//#include <signal.h>
//#include <sys/ipc.h>
//#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// default output filename
char * fname = "cstest";
// default timer(configurable) and snooze status
int timer = 60;
int alarm_stop = 0;
// given number of slaves(configurable)
int slaves = 19;
// loop counter
int i;
// master pid
pid_t master_pid;
// time for random seed
time_t t;
// process id for tracking children
pid_t child_pid;
// shared memory tracking
int segment_id;
int *sFlag = 0;
// shared memory size setup (commented out after more memory friendly approace implemented) 
// const int segment_size = 4096; now (sizeof(sFlag) *20)
// flag state for processes
enum state{IDLE, WANT_IN, IN_CS};

//function prototypes
void parseCmd(int argc, char **argv, int *time, int *slaves, char **fname);
void on_alarm(int signal);
void ctrlCHandler(int signal);
void cleanup();
