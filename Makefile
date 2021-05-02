fltk_comp_flags = -ggdb -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_THREAD_SAFE -D_REENTRANT
fltk_link_flags = -lfltk -lXext -lX11 -lm

run : main
	./main

rtest : test
	./test

main : main.o bfide.o bfint.o
	g++ $(fltk_link_flags) -o $@ $^

test : test.o bfide.o bfint.o
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
bfide.h : bfint.h
	touch bfide.h
clint.h : bfint.h
	touch clint.h


.SECONDARY : *.o
