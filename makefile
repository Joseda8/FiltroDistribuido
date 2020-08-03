CC      = gcc
CFLAGS  = -w
RM      = rm -f


default: all

all: Hello

Hello: dist_filter.c
	mpicc -w -o filter dist_filter.c

run:
	mpirun -np 2 --host master,client1 ./filter scorpion.jpg

clean veryclean:
	$(RM) filter
