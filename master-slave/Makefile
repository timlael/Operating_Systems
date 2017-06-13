# $Author: o3-lael $
# $Date: 2016/02/20 15:43:29 $
# $Log: Makefile,v $
# Revision 4.1  2016/02/20 15:43:29  o3-lael
# final branch ready to turn in
#
# Revision 3.3  2016/02/19 22:35:22  o3-lael
# cleaned up spacing
#
# Revision 3.2  2016/02/19 01:17:27  o3-lael
# fixed timestamp issue in slave.c
# now ready for release
#
# Revision 3.1  2016/02/19 00:58:31  o3-lael
# Branch to release
#
# Revision 2.3  2016/02/18 22:15:55  o3-lael
# cleaned up a little spacing
#
# Revision 2.2  2016/02/18 21:49:47  o3-lael
# created more complex makefile using substitutio
# also implemented make clean
# removed simple makefile lines used in testing
#
# Revision 2.1  2016/02/18 14:57:52  o3-lael
# Branch
# Program is functionally complete
# Now it is time to clean up and fine tune
#
# Revision 1.1  2016/02/13 16:49:59  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 4.1 $
CC= gcc
RM= rm -vf
CFLAGS= -Wall -g 
SRCFILES= master.c slave.c 
OBJFILES= $(patsubst %.c, %.o, $(SRCFILES))
PROGFILES= $(patsubst %.c, %, $(SRCFILES))
.PHONY: all clean
all: $(PROGFILES)
clean:
	$(RM) cstest $(OBJFILES) $(PROGFILES) *~
