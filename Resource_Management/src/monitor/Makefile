# $Author: o3-lael $
# $Date: 2016/04/08 20:58:33 $
# $Log: monitor_Makefile,v $
# Revision 2.1  2016/04/08 20:58:33  o3-lael
# base project working correctly again
# branching before resource allocation implementation
#
# Revision 1.3  2016/04/08 20:50:02  o3-lael
# changes to implement shared header file 'shared.h'
#
# Revision 1.2  2016/04/03 16:35:13  o3-lael
# copied over from p3
#
# $State: Exp $
# $Revision: 2.1 $
AR= ar -rs
CC= gcc
RM= rm -f
LIB= libmonitorlib.a
OBJ= monitorlib.o
SRC= monitorlib.c

$(OBJ): $(SRC) 
	$(CC) -c $(SRC)
	$(AR) $(LIB) $(OBJ)

all: $(OBJ)
clean:
	$(RM) *.o *.a
realclean:
	$(RM) *.o *.a
	$(RM) *~

