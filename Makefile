fltk_comp_flags = $(shell fltk-config --cxxflags)
fltk_link_flags = $(shell fltk-config --ldflags)

run : main
	./main

rtest : test
	./test

main : main.o bfide.o bfint.o roeditor.o
	g++ $(fltk_link_flags) -o $@ $^

test : test.o bfide.o bfint.o roeditor.o
	g++ $(fltk_link_flags) -o $@ $^

clint : clint.o bfint.o
	g++ $(fltk_link_flags) -o $@ $^

%.o : %.cpp
	g++ -c $(fltk_comp_flags) -o $@ $<
bfint.o : bfint.cpp bfint.h
	g++ -c $(fltk_comp_flags) -o $@ $<
clint.o : clint.cpp clint.h
	g++ -c $(fltk_comp_flags) -o $@ $<
bfide.o : bfide.cpp bfide.h
	g++ -c $(fltk_comp_flags) -o $@ $<
main.o : main.cpp bfide.h
	g++ -c $(fltk_comp_flags) -o $@ $<
test.o : test.cpp bfide.h
	g++ -c $(fltk_comp_flags) -o $@ $<
roeditor.o : roeditor.cpp roeditor.h
	g++ -c $(fltk_comp_flags) -o $@ $<
bfide.h : bfint.h roeditor.h
	touch bfide.h
clint.h : bfint.h
	touch clint.h


.SECONDARY : *.o
