CC   = gcc -Wall -g -O0 -c
LINK = gcc -o
LIBS =



all: test_mm



clean:
	rm -f *.o	test_mm



test_mm: test_mm.o mm_manager.o
	$(LINK) test_mm		test_mm.o mm_manager.o		$(LIBS)

test_mm.o: test_mm.c mm_manager.h
	$(CC) test_mm.c



mm_manager.o: mm_manager.c mm_manager.h
	$(CC) mm_manager.c


