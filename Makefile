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

bfide : main.o bfide.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

clint : clint.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

bfint.o : bfint.cpp bfint.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
clint.o : clint.cpp clint.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
bfide.o : bfide.cpp bfide.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
main.o : main.cpp bfide.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
bfide.h : bfint.h
	touch bfide.h
clint.h : bfint.h
	touch clint.h
