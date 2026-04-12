# Makefile for Atari ST / Gulam environment
# Compiler: cc68x
# Authors: Takh, Henry, Aaron
CC = cc68x
AS = gen
OBJS = tetris.o input.o renderer.o raster.o cond.o gstate.o matrix.o \
       next_box.o hold_box.o model.o synch.o tetrom.o psg.o music.o effects.o splash.o \
       setvbase.o vbl_isr.o vbl_inst.o ikbd.o ikbd_isr.o

TARGET = tetris.tos

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

tetris.o: tetris.c
	$(CC) -c tetris.c

input.o: input.c
	$(CC) -c input.c

renderer.o: renderer.c
	$(CC) -c renderer.c

raster.o: raster.c
	$(CC) -c raster.c

cond.o: cond.c
	$(CC) -c cond.c

gstate.o: gstate.c
	$(CC) -c gstate.c

matrix.o: matrix.c
	$(CC) -c matrix.c

next_box.o: next_box.c
	$(CC) -c next_box.c

hold_box.o: hold_box.c
	$(CC) -c hold_box.c

model.o: model.c
	$(CC) -c model.c

synch.o: synch.c
	$(CC) -c synch.c

tetrom.o: tetrom.c
	$(CC) -c tetrom.c

psg.o: psg.c psg.h types.h
	$(CC) -c psg.c

music.o: music.c music.h psg.h types.h
	$(CC) -c music.c

splash.o: splash.c splash.h types.h
	$(CC) -c splash.c

effects.o: effects.c
	$(CC) -c effects.c

setvbase.o: setvbase.s
	$(AS) setvbase.s -L2

vbl_isr.o: vbl_isr.s
	$(AS) vbl_isr.s -L2

vbl_inst.o: vbl_inst.c
	$(CC) -c vbl_inst.c

ikbd.o: ikbd.c ikbd.h types.h
	$(CC) -c ikbd.c

ikbd_isr.o: ikbd_isr.s
	$(AS) ikbd_isr.s -L2

clean:
	rm *.o
	del $(TARGET)