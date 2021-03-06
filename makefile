CC      = gcc
CFLAGS  = -w
RM      = rm -f


default: all

all: Hello

Hello: dist_filter.c
	mpicc -w -o filter dist_filter.c

run_1:
	mpirun -np 1 --host master,client1 ./filter scorpion.jpg

run_2:
	mpirun -np 2 --host master,client1 ./filter scorpion.jpg

clean veryclean:
	$(RM) filter
	clear


start_stats:
	sudo nethogs -t &> nethogs.log &

stats:
	./hogs -type=pretty nethogs.log
