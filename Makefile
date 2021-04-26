fltk_comp_flags = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_THREAD_SAFE -D_REENTRANT
fltk_link_flags = -lfltk -lXext -lX11 -lm

run : main
	./main

main : main.o
	g++ $(fltk_link_flags) -o main $^

%.o : %.cpp
	g++ -c $(fltk_comp_flags) -o $@ $<


.SECONDARY : *.o
