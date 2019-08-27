TARGET=main

CC=msp430-gcc
#CFLAGS=-DMSPGCC -DTESTANDO -DSEM_PO -Os -Wall -g -mmcu=msp430g2553 -L/usr/lib/gcc/msp430/4.5.3 -L/usr/msp430/lib
CFLAGS=-DMSPGCC -DTESTANDO -Os -Wall -g -mmcu=msp430g2553 -L/usr/lib/gcc/msp430/4.5.3 -L/usr/msp430/lib
PROGRAMER=/usr/src/msp430/mspdebug-0.19/mspdebug

OBJS=main.o \
	halactuator.o \
	SM.o \
	po.o \
	config.o


#OBJS=serial.o
#OBJS=serialTexas.o 



all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET).elf $(OBJS)

program: $(TARGET).elf
	$(PROGRAMER) rf2500 "prog $(TARGET).elf"

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET).elf $(OBJS)
