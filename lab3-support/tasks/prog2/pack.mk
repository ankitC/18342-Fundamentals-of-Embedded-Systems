PROGS_PROG2_OBJS := prog2.o
PROGS_PROG2_OBJS := $(PROGS_PROG2_OBJS:%=$(TDIR)/prog2/%)
ALL_OBJS += $(PROGS_PROG2_OBJS)

$(TDIR)/bin/prog2 : $(TSTART) $(PROGS_PROG2_OBJS) $(TLIBC)

