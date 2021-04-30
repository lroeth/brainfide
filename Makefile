fltk_comp_flags = -ggdb -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_THREAD_SAFE -D_REENTRANT
fltk_link_flags = -lfltk -lXext -lX11 -lm

run : main
	./main

rtest : test
	./test

main : main.o bfide.o bfint.o
	g++ $(fltk_link_flags) -o $@ $^

test : test.o
	g++ $(fltk_link_flags) -o $@ $^

%.o : %.cpp *.h
	g++ -c $(fltk_comp_flags) -o $@ $<

clint : clint.o bfint.o
	g++ $(fltk_link_flags) -o $@ $^

.SECONDARY : *.o
