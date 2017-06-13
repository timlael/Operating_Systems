# $Author: o3-lael $
# $Date: 2016/04/28 02:29:59 $
# $Log: main_Makefile,v $
# Revision 5.1  2016/04/28 02:29:59  o3-lael
# previous chekin note on branch 4 didn't take
# this is a recovery branch due to Hoare issues
# Hoare was unstable 26 and 27 April and user minx files were in my vim .swp
# after an immediate shutdown on 27 APR. This project is complete and a local 
# copy that was recovered.
#
# Revision 4.1  2016/04/28 02:24:13  o3-lael
# see note on main makefile
#
# $State: Exp $
# $Revision: 5.1 $
CC= gcc
FLAGS= -g
RM= rm -f
MV= mv
SRC1= oss.c
SRC2= up.c
TARGET1= oss
TARGET2= up
OBJ1= oss.o $(SHR)
OBJ2= up.o $(SHR)
SHR= shared.o
UP= ../

.SUFFIXES: .c .o

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1)
	$(CC)  -o $@ $(OBJ1)
	$(MV) $(TARGET1) $(UP)

$(TARGET2): $(OBJ2)
	$(CC)  -o $@ $(OBJ2)
	$(MV) $(TARGET2) $(UP)

.c.o:
	$(CC) -c -o $@ $<

clean:
	$(RM) *.o

realclean:
	$(RM) *.o
	$(RM) *~
