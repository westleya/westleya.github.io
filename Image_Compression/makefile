CFLAGS = -std=c++14 -g -O3

all: compress decompress runs display clean

# THE FOUR INDIVIDUAL PROGRAMS
compress: ch-compress/main.cpp huff.o read-files.o unhuff.o rle.o
	clang++ -o ch-compress/compress $(CFLAGS) $^

decompress: ch-decompress/main.cpp huff.o read-files.o unhuff.o rle.o unrle.o
	clang++ -o ch-decompress/decompress $(CFLAGS) $^

runs: dump-runs/main.cpp rle.o read-files.o
	clang++ -o dump-runs/runs $(CFLAGS) $^

display: ch-display/main.cpp
	clang++ -o ch-display/display ch-display/main.cpp -I/opt/X11/include -O2 -lm -lpthread -L/opt/X11/lib -lm -lpthread -lX11

# O FILES
huff.o: huff.cpp huff.h
	clang++ -c $(CFLAGS) huff.cpp

read-files.o: read-files.cpp read-files.h
	clang++ -c $(CFLAGS) read-files.cpp

unhuff.o: unhuff.cpp huff.h pbmcompress-v1.h
	clang++ -c $(CFLAGS) unhuff.cpp

rle.o: rle.cpp pbmcompress-v1.h
	clang++ -c $(CFLAGS) rle.cpp

unrle.o: unrle.cpp pbmcompress-v1.h
	clang++ -c $(CFLAGS) unrle.cpp

# CLEAN UP O FILES
clean: 
	rm *.o