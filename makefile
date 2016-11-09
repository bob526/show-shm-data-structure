all:	show_shm_ds.c
	gcc -Wall -Wextra -g -o show_shm_ds show_shm_ds.c

run:
	./show_shm_ds
