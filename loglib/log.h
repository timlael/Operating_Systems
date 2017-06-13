/* $Author: o3-lael $
 * $Date: 2016/02/11 15:46:32 $
 * $Log: log.h,v $
 * Revision 1.2  2016/02/11 15:46:32  o3-lael
 * adding keywords
 *
 * $State: Exp $
 * $Revision: 1.2 $
 */

#include <time.h>

typedef struct data_struct{
	time_t time;
	char *string;
} data_t;

int addmsg(data_t data);
void clearlog(void);
char *getlog(void);
int savelog(char *filename);

