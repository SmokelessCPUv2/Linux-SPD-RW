CC=gcc
LIBS= -li2c

spd_dump: spd_dump.o
	$(CC) -o spd_dump spd_dump.o $(LIBS)

spd_write: spd_write.o
	$(CC) -o spd_write spd_write.o $(LIBS)
