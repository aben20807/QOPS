all: qSim.out
CC:=gcc
INCLUDE:=.
CFLAGS:=-g -O3
OMPFLAGES:=-fopenmp

qSim.out: main.o ini.o init.o common.o gate.o gate_util.o gate_chunk.o measure.o
	$(CC) $(CFLAGS) $(OMPFLAGES) $^ -o $@ -lm
# -lm for math.h

main.o: main.c init.h common.h gate.h
	$(CC) -I$(INCLUDE) -c $(CFLAGS) $(OMPFLAGES) main.c

init.o: init.c init.h common.h gate.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) init.c

common.o: common.c common.h gate.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) common.c

gate.o: gate.c common.h gate.h gate_util.h gate_chunk.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) gate.c

gate_util.o: gate_util.c gate_util.h common.h gate.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) gate_util.c

gate_chunk.o: gate_chunk.c gate_chunk.h common.h gate_util.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) gate_chunk.c

measure.o: measure.c measure.h common.h gate.h gate_util.h gate_chunk.h
	$(CC) -c $(CFLAGS) $(OMPFLAGES) measure.c

ini.o: ini.c ini.h
	$(CC) -c $(CFLAGS) ini.c

clean:
	rm -f qSim.out main.o gate.o gate_chunk.o gate_util.o ini.o init.o common.o measure.o
