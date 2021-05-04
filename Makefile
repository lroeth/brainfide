FLTKCONFIG = fltk-config

cxx = $(shell $(FLTKCONFIG) --cxx)
fltk_comp_flags = $(shell $(FLTKCONFIG) --cxxflags)
fltk_link_flags = $(shell $(FLTKCONFIG) --ldflags)

all : bfide clint

run : bfide
	./bfide

clean :
	-rm bfide clint *.o

bfide : main.o bfide.o bfint.o roeditor.o
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
roeditor.o : roeditor.cpp roeditor.h
	$(cxx) -c $(fltk_comp_flags) -o $@ $<
bfide.h : bfint.h roeditor.h
	touch bfide.h
clint.h : bfint.h
	touch clint.h
