level: test.o level_hashing.o hash.o my_include.o
	cc -o level test.o level_hashing.o hash.o my_include.o -lm

my_include.o: my_include/my_include.c my_include/my_include.h
	cc -c my_include/my_include.c -lm
test.o: test.c level_hashing.h
	cc -c test.c -lm
level_hashing.o : level_hashing.c level_hashing.h
	cc -c level_hashing.c -lm
hash.o : hash.c hash.h
	cc -c hash.c -lm

clean:
	rm *.o level
