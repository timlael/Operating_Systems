/*$Author: o3-lael $
  $Date: 2016/03/09 20:03:28 $
  $Log: slave.h,v $
  Revision 5.3  2016/03/09 20:03:28  o3-lael
  added comments to new and/or uncommented sections

  Revision 5.2  2016/03/09 19:17:04  o3-lael
  *** empty log message ***

  Revision 5.1  2016/03/09 19:13:56  o3-lael
  folder structure and associated makefile implemented

  Revision 4.1  2016/03/09 19:06:49  o3-lael
  *** empty log message ***

  Revision 3.1  2016/03/09 18:10:47  o3-lael
  slave implementation completed

  Revision 2.2  2016/03/07 18:53:00  o3-lael
  *** empty log message ***

  Revision 2.1  2016/03/07 15:31:45  o3-lael
  Branch to rev. 2
  clean break from old strategy. Now implementing monitor from class notes
  slave unchanged at this point

  Revision 1.4  2016/03/07 14:55:40  o3-lael
  implementing Monitoe class to reflect notes

  Revision 1.3  2016/03/06 21:44:45  o3-lael
  beginnins of monitor implemenation

  Revision 1.2  2016/03/06 16:25:56  o3-lael
  cleaned up RCS headers and added RCS keywords where missing
  headers were funky on copy from project 2 since I didn't clean them up correctly before copy

  Revision 1.1  2016/03/04 15:37:19  o3-lael
  Initial revision

  $State: Exp $
  $Revision: 5.3 $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "../monitor/monitor.h"

// time for srand()
time_t t;
// loop variables
int i;
// pointer for output file and default filename
FILE *logname = NULL;
char *filename = "cstest";
// entry condition variable (shared memory)
int *entry;
// function prototypes
void ctrlCHandler(int sig);
int critical_sec(FILE *logname, int who);
void init_cs(Monitor *m, int *e_cond);
void exit_cs(Monitor *m, int *e_cond);
