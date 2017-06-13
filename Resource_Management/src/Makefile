# $Author: o3-lael $
# $Date: 2016/04/08 20:58:33 $
# $Log: Makefile,v $
# Revision 2.1  2016/04/08 20:58:33  o3-lael
# base project working correctly again
# branching before resource allocation implementation
#
# Revision 1.1  2016/03/27 15:35:39  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 2.1 $
FOR= for dir in
SUBDIRS= monitor main
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
	$(RM) cstest
