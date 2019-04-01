GEN_EXE = server client

all : ${GEN_EXE}

clean :
	rm -f *.o ${GEN_EXE}
