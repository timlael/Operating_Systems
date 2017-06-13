/*$Author: o3-lael $
  $Date: 2016/03/09 20:03:28 $
  $Log: master.h,v $
  Revision 5.3  2016/03/09 20:03:28  o3-lael
  added comments to new and/or uncommented sections

  Revision 5.2  2016/03/09 19:17:04  o3-lael
  *** empty log message ***

  Revision 5.1  2016/03/09 19:13:56  o3-lael
  folder structure and associated makefile implemented

  Revision 4.1  2016/03/09 19:06:49  o3-lael
  *** empty log message ***

  Revision 3.1  2016/03/09 18:10:47  o3-lael
  master implementation complete after two days of beating head on desk
  fixed shared memory allocation/attachment error which was causing semaphore
  ids to be zeroed out. The base monitor/semaphore code was near correct all along
  it was memory issues that casused all my problems

  Revision 2.2  2016/03/07 18:53:00  o3-lael
  *** empty log message ***

  Revision 2.1  2016/03/07 15:31:45  o3-lael
  Branch to rev. 2
  clean break from old strategy. Now implementing monitor from class notes
  master remains unchanged at this point

  Revision 1.6  2016/03/07 14:55:40  o3-lael
  still working with monitor implementation
  changing everything to reflect class notes and examples

  Revision 1.5  2016/03/06 21:44:45  o3-lael
  beginnins of monitor implemenation

  Revision 1.4  2016/03/06 19:02:14  o3-lael
  passing properties struct via shmem rather than
  passing multiple variables via multiple segments

  Revision 1.3  2016/03/06 17:25:09  o3-lael
  fiddling with shared memory and status flags

  Revision 1.2  2016/03/06 16:25:56  o3-lael
  cleaned up RCS headers and added RCS keywords where missing
  headers were funky on copy from project 2 since I didn't clean them up correctly before copy

  Revision 1.1  2016/03/04 15:37:19  o3-lael
  Initial revision

  $State: Exp $
  $Revision: 5.3 $
*/
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "../monitor/monitorlib.c"

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

// shared memory/semaphore/Monitor setup	
int segment_id;
int segment2_id;
int sem1_id;
int sem2_id;
Monitor* monitor;

//function prototypes
void parseCmd(int argc, char **argv, int *time, int *slaves, char **fname);
void on_alarm(int signal);
void ctrlCHandler(int signal);
void cleanup();
