FLTKCONFIG = fltk-config

all : bfide clint

flags.mk :
	echo cxx = $(shell $(FLTKCONFIG) --cxx) >flags.mk
	echo fltk_comp_flags = $(shell $(FLTKCONFIG) --cxxflags) >>flags.mk
	echo fltk_link_flags = $(shell $(FLTKCONFIG) --ldflags) >>flags.mk

include flags.mk

run : bfide
	./bfide

clean :
	-rm bfide clint *.o

bfide : bfide.o idestate.o bfintbidir.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

clint : clint.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

bfintbidir.o : bfintbidir.cpp bfintbidir.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
bfint.o : bfint.cpp bfint.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
clint.o : clint.cpp clint.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
idestate.o : idestate.cpp idestate.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
bfide.o : bfide.cpp idestate.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
idestate.h : bfint.h
	touch $@
clint.h : bfint.h
	touch $@
bfintbidir.h : bfint.h
	touch $@
