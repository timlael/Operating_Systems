# $Author: o3-lael $
# $Date: 2016/03/09 19:17:04 $
# $Log: Makefile,v $
# Revision 5.2  2016/03/09 19:17:04  o3-lael
# *** empty log message ***
#
# Revision 5.1  2016/03/09 19:13:56  o3-lael
# folder structure and associated makefile implemented
#
# Revision 4.1  2016/03/09 19:06:49  o3-lael
# *** empty log message ***
#
# Revision 3.2  2016/03/09 18:46:27  o3-lael
# new Makefile to handle subdirs
#
# Revision 3.1  2016/03/09 18:10:47  o3-lael
# Makefile unchanged.
# However, in the next chekin, I will implement a new Makefile and
# subfolder structure with sub-Makefiles.
#
# Revision 2.2  2016/03/07 18:53:00  o3-lael
# *** empty log message ***
#
# Revision 2.1  2016/03/07 15:31:45  o3-lael
# Branch to rev. 2 
# clean break from old strategy. Now implementing monitor from class notes
# Makefile remains unchanged at this point
#
# Revision 1.5  2016/03/06 21:44:45  o3-lael
# more makefile tweaks/bugfixes
#
# Revision 1.4  2016/03/06 19:02:14  o3-lael
# modified make clean rules
#
# Revision 1.3  2016/03/06 17:25:09  o3-lael
# modifications for library usage
# may change final approach to use subdirs and multiple makefiles
#
# Revision 1.2  2016/03/06 16:25:56  o3-lael
# cleaned up RCS headers and added RCS keywords where missing
# headers were funky on copy from project 2 since I didn't clean them up correctly before copy
#
# Revision 1.1  2016/03/04 15:37:19  o3-lael
# Initial revision
#
# $State: Exp $
# $Revision: 5.2 $
FOR= for dir in
SUBDIRS= monitor main
MAKE= do make
FIN= $$dir ; done
RM= rm -f
all: subs

subs:
	$(FOR) $(SUBDIRS) ; $(MAKE) -C $(FIN)

clean:
	$(FOR) $(SUBDIRS) ; $(MAKE) clean -C $(FIN)
realclean:
	$(FOR) $(SUBDIRS) ; $(MAKE) realclean -C $(FIN)
	$(RM) ./master ./slave
	$(RM) cstest
