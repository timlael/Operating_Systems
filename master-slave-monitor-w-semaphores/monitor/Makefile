# $Author: o3-lael $
# $Date: 2016/03/10 02:34:47 $
# $Log: monitor_Makefile,v $
# Revision 5.4  2016/03/10 02:34:47  o3-lael
# added substitution rules to the max
#
# Revision 5.3  2016/03/09 19:44:42  o3-lael
# substitution rules added
#
# Revision 5.2  2016/03/09 19:17:04  o3-lael
# *** empty log message ***
#
# Revision 5.1  2016/03/09 19:13:56  o3-lael
# folder structure and associated makefile implemented
#
# $State: Exp $
# $Revision: 5.4 $
AR= ar -rs
CC= gcc
RM= rm -f
LIB= libmonitorlib.a
OBJ= monitorlib.o
SRC= monitorlib.c

$(OBJ): $(SRC) monitor.h
	$(CC) -c $(SRC)
	$(AR) $(LIB) $(OBJ)

all: $(OBJ)
clean:
	$(RM) *.o *.a
realclean:
	$(RM) *.o *.a
	$(RM) *~

