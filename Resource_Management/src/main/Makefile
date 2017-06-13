# $Author: o3-lael $
# $Date: 2016/04/08 20:58:33 $
# $Log: main_Makefile,v $
# Revision 2.1  2016/04/08 20:58:33  o3-lael
# base project working correctly again
# branching before resource allocation implementation
#
# Revision 1.2  2016/04/08 20:50:02  o3-lael
# changes to implement shared header file 'shared.h'
#
# Revision 1.1  2016/03/27 15:35:39  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 2.1 $
CC= gcc
RM= rm -f
MV= mv
LIB= ../monitor/libmonitorlib.a
SRC1= oss.c
SRC2= up.c
TARGET1= oss 
TARGET2= up
OBJ1= oss.o
OBJ2= up.o
UP= ../

$(TARGET1): $(OBJ1)
	$(CC)  -o $(TARGET1) $(OBJ1)
	$(MV) $(TARGET1) $(UP)
$(OBJ1):$(SRC1) 
	$(CC) -c $(SRC1)

$(OBJ2): $(SRC2) 
	$(CC) -c $(SRC2)

$(TARGET2): $(OBJ2) 
	$(CC) $(SRC2) $(LIB) -o $(TARGET2)
	$(MV) $(TARGET2) $(UP)

all:  $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)

clean:
	$(RM) *.o

realclean:
	$(RM) *.o
	$(RM) *~

