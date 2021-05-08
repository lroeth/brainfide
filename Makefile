FLTKCONFIG = fltk-config

cxx = $(shell $(FLTKCONFIG) --cxx )
fltk_comp_flags = $(shell $(FLTKCONFIG) --cxxflags )
fltk_link_flags = $(shell $(FLTKCONFIG) --ldflags )
fltk_static_link_flags = -static $(shell $(FLTKCONFIG) --ldstaticflags )

all : bfide clint

sall : sbfide sclint

run : bfide
	./bfide

clean :
	-rm bfide clint *.o

bfide : bfide.o idestate.o bfintbidir.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

clint : clint.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_link_flags)

sbfide : bfide.o idestate.o bfintbidir.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_static_link_flags)

sclint : clint.o bfint.o
	$(cxx)  -o $@ $^ $(fltk_static_link_flags)

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

idestate.h : bfintbidir.h
	touch $@
clint.h : bfint.h
	touch $@
bfintbidir.h : bfint.h
	touch $@
