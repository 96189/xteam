CFLAGS = -pg -g -Wall 
LIBRARY = -lpthread -lm -lssl -lcrypto 
THRID_LIBS = ../third
INC = -I../include -I./ -I$(THRID_LIBS)/cJSON
MY_LIB = -lzlog

BINARY = webserver
CC = gcc
SRCS = $(wildcard *.c)
SRCS += $(wildcard $(THRID_LIBS)/cJSON/*.c)

OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(BINARY)

%.o: %.c makefile
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clear:
	rm -rf *.o
clean:
	rm -f *.o $(BINARY) $(OBJS)

$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARY)  $(MY_LIB)

