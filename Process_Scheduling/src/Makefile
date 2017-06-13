# $Author: o3-lael $
# $Date: 2016/03/21 21:28:25 $
# $Log: main_Makefile,v $
# Revision 4.2  2016/03/21 21:28:25  o3-lael
# removing unused bits
#
# Revision 4.1  2016/03/21 15:24:44  o3-lael
# project functional.
# branching for final cleanup and release
#
# Revision 3.2  2016/03/20 15:20:57  o3-lael
# almost working
# last process in hanging and stats not running
#
# Revision 3.1  2016/03/17 01:42:36  o3-lael
# branched to sync file versions with addition of up.c and up.h
#
# Revision 2.2  2016/03/16 15:35:13  o3-lael
# modified Makefiles to match current project
#
# Revision 2.1  2016/03/16 15:19:08  o3-lael
# branching to v2 and implementing forked master/slave functionality
#
# Revision 1.3  2016/03/12 22:05:19  o3-lael
# added make all rule
#
# Revision 1.2  2016/03/12 21:48:17  o3-lael
# created makefile to lessen typing during testing
#
# Revision 1.1  2016/03/12 18:23:37  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 4.2 $
CC= gcc
FLAG= -Wall -g
SRC1= oss.c
OBJ1= oss.o
TARGET1= oss
SRC2= up.c
OBJ2= up.o
TARGET2= up
RM= rm -fv
MV= mv
#LIB= ../monitor/libmonitorlib.a
UP= ../

$(TARGET1): $(OBJ1)
	$(CC) -o $(TARGET1) $(OBJ1)
$(OBJ1): $(SRC1)
	$(CC) -c $(SRC1)
$(TARGET2): $(OBJ2)
	$(CC) -o $(TARGET2) $(OBJ2)
$(OBJ2): $(SRC2)
	$(CC) -c $(SRC2)

all: $(TARGET1) $(TARGET2)
clean: 
	$(RM) *.o
realclean:
	$(RM) *.o $(TARGET1) $(TARGET2) cstest
