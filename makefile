GEN_EXE = server client

linklist.o : linklist.c
	cc -c $<
server : server.o linklist.o
	gcc -o $@ $^

all : ${GEN_EXE}

clean :
	rm -f *.o ${GEN_EXE}
