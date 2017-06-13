# $Author: o3-lael $
# $Date: 2016/04/28 02:29:59 $
# $Log: Makefile,v $
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
# Revision 3.1  2016/04/27 04:26:19  o3-lael
# final working before comments
#
# Revision 2.1  2016/04/22 14:27:40  o3-lael
# branching to revision 2 
# major implementation change moving all oss child spawn and management to functions
#
# Revision 1.1  2016/04/14 19:17:48  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 5.1 $
FOR= for dir in
SUBDIRS= main
MAKE= do make all
CLEAN= do make clean
REALCLEAN= do make realclean
FIN= $$dir ; done
RM= rm -f
all: subs

subs:
	$(FOR) $(SUBDIRS) ; $(MAKE) -C $(FIN)

clean:
	$(FOR) $(SUBDIRS) ; $(CLEAN) -C $(FIN)
realclean:
	$(FOR) $(SUBDIRS) ; $(REALCLEAN) -C $(FIN)
	$(RM) ./oss ./up
