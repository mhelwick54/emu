TARGET=emu
CFLAGS=-Wall -Wno-unused-variable -Wno-unused-value -ffreestanding
LDFLAGS=-Iinclude

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	ifeq ($(shell which gcc-8), /usr/local/bin/gcc-8)
		CC=gcc-8#MacOS GNU gcc, not clang
	else
		CC=clang
	endif
else
	CC=gcc#GNU gcc
endif

IDIR=include
SDIR=src
BDIR=bin
LDIR=lib

_DEPS=funcs.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_SRC=main.c
SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

_OBJ=main.o
OBJ=$(patsubst %,$(BDIR)/%,$(_OBJ))

_LIB=
LIB=$(patsubst %,-l%,$(_LIB))

all: $(BDIR)/$(TARGET)

pre:
	$(CC) $(LDFLAGS) -E $(SRC) -o pre.c

info:
	$(info DEPS is $(DEPS))
	$(info SRC is $(SRC))
	$(info OBJ is $(OBJ))
	$(info LIB is $(LIB))

$(BDIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -MMD -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(BDIR)/*
