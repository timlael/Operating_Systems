/* $Author: o3-lael $
 * $Date: 2016/02/11 21:06:15 $
 * $Log: loglib.c,v $
 * Revision 1.19  2016/02/11 21:06:15  o3-lael
 * fixed string malloc issue
 *
 * Revision 1.18  2016/02/10 15:40:15  o3-lael
 * final readability tune-up
 *
 * Revision 1.17  2016/02/10 02:07:09  o3-lael
 * fixed some data_t.time placement issues
 *
 * Revision 1.16  2016/02/09 20:04:28  o3-lael
 * added message when attemptin to save empty log to disk
 *
 * Revision 1.15  2016/02/09 19:36:26  o3-lael
 * one last timestamp allocation fix
 *
 * Revision 1.14  2016/02/09 16:08:38  o3-lael
 * corrected msgTime allocation in getlog()
 *
 * Revision 1.13  2016/02/09 03:17:46  o3-lael
 * altered include statements to reflect archive use
 *
 * Revision 1.10  2016/02/08 20:21:00  o3-lael
 * fixed timestamp creation bugs
 * general cleanup
 *
 * Revision 1.9  2016/02/08 16:40:02  o3-lael
 * changed my variable names on temp items. I was confucing myself.
 *
 * Revision 1.8  2016/02/03 20:16:51  o3-lael
 * minor fixes
 *
 * Revision 1.7  2016/02/03 03:43:23  o3-lael
 * fixed checkin of incorrect file in previous revision
 *
 * Revision 1.6  2016/02/03 01:33:42  o3-lael
 * bugfixes - #include <errno.h> and various datatype format errors
 *
 * Revision 1.5  2016/02/02 22:09:36  o3-lael
 * finished/added getlog()
 *
 * Revision 1.4  2016/02/02 19:34:31  o3-lael
 * savelog finished
 * clearlog was completed in previous rev, but no log entry was made upon checkin
 *
 * Revision 1.2  2016/02/02 17:45:01  o3-lael
 * addmsg function completed
 *
 * Revision 1.1  2016/02/02 17:05:24  o3-lael
 * added skeleton code
 *
 * $State: Exp $
 * $Revision: 1.19 $
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log.h"

typedef struct list_struct{
	data_t item;
	struct list_struct *next;
} log_t;
static log_t *headptr = NULL;
static log_t *tailptr = NULL;

//adds a message to the log
int addmsg(data_t data){
	//allocate memory for new log entry list item
	log_t *newMsg = malloc(sizeof(log_t));
	
	//if memory cannot be allocated print error message to stderr
	if(newMsg == NULL){
		errno = ENOMEM;
		perror("Memory allocation error\n");
	return -1;
	}

	//create timestamp and obtain current time
	time_t  current_time;
	current_time = time(NULL);
	//handle error if time is not obtained correctly
	if(current_time == ((time_t)-1)){
		perror("Failed to obtain current time");
	return -1;	
	}

	//add data text/time to newMsg and set pointer to next node to NULL
	newMsg->item = data;
	newMsg->item.time = current_time;
	newMsg->next = NULL;

	//if both head and tail pointers are null, the log is empty
	//make newMsg head and tail (first and only element)
	if(headptr == NULL && tailptr == NULL){
		headptr = newMsg;
		tailptr = newMsg;
	}
	// else place at the tail of the log by pointing current tailptr
	// to newMsg and then setting tailptr to newMsg
	else{
		tailptr->next = newMsg;
		tailptr = newMsg;
	}
return 0;
}

//clear entire log from memory
void clearlog(void){
	//check to make sure log has entries
	if(headptr != NULL){
		//create temporary copy of first element to preserve next property
		log_t *temp_log_t = headptr;
		
		while(headptr != NULL){
			temp_log_t = headptr->next;
			free(headptr);
			headptr = NULL;
			headptr = temp_log_t;
		}
		//set tailptr to null after last head element is freed
		tailptr = NULL;
		printf("The log has been cleared.\n");
	}
	else
		printf("Nothing to clear. The log is currently empty.\n");
}

char *getlog(void){
	//allocate character string
	char *entry = malloc(sizeof(char) + 1);
	
	//handle memory allocation error
	if(entry == NULL){
		errno = ENOMEM;
		perror("Memory allocation error\n");
	return NULL;
	}

	//create temporary data and log items, point temp log item to head of list
	data_t temp_data_t;
	log_t *temp_log_t = headptr;
	//create character string to hold timestamp
	char* msgTime = malloc(sizeof(time_t) + 1);
	//handle memory allocation error
	if(msgTime == NULL){
		errno =ENOMEM;
		perror("Memory allocation error\n");
	return NULL;
	}
	
	//initialize string to empty string
	strcpy(entry,"");
	//check to make sure log isn't empty
	if(temp_log_t != NULL){
	
		//iterate through each item until NULL(end of list) concat string and time together for each
		while(temp_log_t != NULL){
			//set temporary text=placeHolder text
			temp_data_t = temp_log_t->item;
			//retrieve and convert time to string
			msgTime = ctime(&temp_data_t.time);
			//reallocate memory to accomodate new size of concatenated string
			entry = realloc(entry, strlen(entry) + strlen(temp_data_t.string) + strlen(msgTime) + 4);
			//handle memory allocation error
			if(entry == NULL){
				errno = ENOMEM;
				perror("Memory allocation error\n");
			return NULL;
			}
			//concatenate values together into entry
			strcat(entry, temp_data_t.string);
			strcat(entry, " @ ");
			strcat(entry, msgTime);
			//move to next item
			temp_log_t = temp_log_t->next;
		}
	return entry;
	}
	
	else
	return NULL;
}

// save log to file
int savelog(char *filename){
	//check to make sure the log has entries
	if(headptr != NULL){
		// create a pointer to the new logfile and open with write perms
		FILE *log = NULL;
		log = fopen(filename, "a+");
		//check to make sure file was created successfully
		if(log == NULL){
			errno = EPERM;
			perror("Error creating output file.\n");
		return -1;
		}
		//create temporary data and log items point temp log item to head of list
		data_t temp_data_t;
		log_t *temp_log_t = headptr;
		//allocate character string to hold time
		char* msgTime = malloc(sizeof(time_t) + 1);
		//handle memory allocation error for messageTime
		if(msgTime == NULL){
			errno = ENOMEM;
			perror("Memory allocation error\n");
		return -1;
		}
		msgTime = "";

		//iterate through list items and write item element to file
		while(temp_log_t != NULL){
			temp_data_t = temp_log_t->item;
			msgTime = ctime(&temp_data_t.time);
			fprintf(log, "%s @ %s", temp_data_t.string, msgTime);
			temp_log_t = temp_log_t->next;
		}
		
		fclose(log);
		printf("Successfully saved log to file.\n");
	}
	else
	printf("Nothing to save to disk.\n");
return 0;
}
