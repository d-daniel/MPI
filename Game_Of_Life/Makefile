COMP=-c

CC=mpicc
CCFLAGS=-O3
CCLOAD= $(CCFLAGS) -lm

EXEC=life_mpi

OBJ=\
    wall_time.o \
    one_life_mpi.o \
    life_mpi.o

$(EXEC):    $(OBJ)
    $(CC) -o $(EXEC) $(CCLOAD) $(OBJ)

clean:
    rm -f *.o $(EXEC)


.SUFFIXES   :   .o .c

.c.o:
    $(CC) $(COMP) $(CCFLAGS) $*.c 
