CC=gcc
LIBS= -li2c

spd_dump: main.o
	$(CC) -o spd_dump main.o $(LIBS)