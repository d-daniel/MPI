COMP=-c

CC=mpicc
CCFLAGS=-O3 -Wall
CCLOAD= $(CCFLAGS) -lm

EXEC=blowfish

OBJ=\
	walltime.o \
	blowfish.o \
	main.o

$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(CCLOAD) $(OBJ)

clean:
	rm -f *.o $(EXEC)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c
