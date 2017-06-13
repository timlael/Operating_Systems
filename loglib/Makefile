# $Author: o3-lael $
# $Date: 2016/02/11 15:44:52 $
# $Log: Makefile,v $
# Revision 1.5  2016/02/11 15:44:52  o3-lael
# adding keywords
#
# $State: Exp $
# $Revision: 1.5 $
CC = gcc
CFLAGS = -g -Wall -static
TARGET = driver
OBJS = driver.o
LIBOBJS = loglib.o
LIBS = -lmyloglib
MYLIBS = libmyloglib.a
LIBPATH = .
.SUFFIXES: .c .o
$(TARGET): $(OBJS) $(MYLIBS)
	$(CC) -o $@ -L. $(OBJS) $(LIBS)
$(MYLIBS): $(LIBOBJS)
	ar -rs $@ $(LIBOBJS)
.c.o:
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o *.a *.log $(TARGET)
