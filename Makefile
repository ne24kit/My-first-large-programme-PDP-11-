# Makefile 6

IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)
BIN=emulator

SDIR=src
ODIR=obj

_DEPS=header.h log.h mem.h run.h test_mem.h test_reg.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = run.o log.o mem.o main.o test_reg.o test_mem.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: all clean

all: $(BIN)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(SDIR)/*~ $(IDIR)/*~






