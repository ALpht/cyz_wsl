pointer00 : pointer00.o
	gcc -o pointer00 pointer00.o
pointer00.o : pointer00.c
	gcc -c pointer00.c
clean:
	rm *.o
	
