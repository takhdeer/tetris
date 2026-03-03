# Makefile for Atari ST / Gulam environment
# Compiler: cc68x
# Author: Henry

CC = cc68x

OBJS = tst_hr.o renderer.o raster.o hold_box.o tetrom.o matrix.o \
       next_box.o gstate.o cond.o model.o synch.o

TARGET = tst_hr.prg

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

tst_hr.o: tst_hr.c
	$(CC) -c tst_hr.c

renderer.o: renderer.c
	$(CC) -c renderer.c

raster.o: raster.c
	$(CC) -c raster.c

hold_box.o: hold_box.c
	$(CC) -c hold_box.c

tetrom.o: tetrom.c
	$(CC) -c tetrom.c

matrix.o: matrix.c
	$(CC) -c matrix.c

next_box.o: next_box.c
	$(CC) -c next_box.c

gstate.o: gstate.c
	$(CC) -c gstate.c

cond.o: cond.c
	$(CC) -c cond.c

model.o: model.c
	$(CC) -c model.c

synch.o: synch.c
	$(CC) -c synch.c

clean:
	del *.o
	del $(TARGET)
