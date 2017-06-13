# $Author: o3-lael $
# $Date: 2016/03/10 14:16:45 $
# $Log: main_Makefile,v $
# Revision 5.5  2016/03/10 14:16:45  o3-lael
# sub rule for directory added
#
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
# $Revision: 5.5 $
CC= gcc
RM= rm -f
MV= mv
LIB= ../monitor/libmonitorlib.a
SRC1= master.c
SRC2= slave.c
TARGET1= master
TARGET2= slave
OBJ1= master.o
OBJ2= slave.o
UP= ../
all:  $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1)
	$(CC)  -o $(TARGET1) $(OBJ1)
	$(MV) $(TARGET1) $(UP)
$(OBJ1):$(SRC1) master.h
	$(CC) -c $(SRC1)

$(OBJ2): $(SRC2) slave.h
	$(CC) -c $(SRC2)

$(TARGET2): $(OBJ2) 
	$(CC) $(SRC2) $(LIB) -o $(TARGET2)
	$(MV) $(TARGET2) $(UP)
clean:
	$(RM) *.o
realclean:
	$(RM) *.o
	$(RM) *~

