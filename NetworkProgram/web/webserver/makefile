CFLAGS = -g -Wall 
LIBRARY = -lpthread -lm -lssl -lcrypto 
THRID_LIBS = ../third
INC = -I../include -I./ -I$(THRID_LIBS)/cJSON
MY_LIB = -lzlog
LIBPATH = -L../lib

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
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARY) $(LIBPATH)/ $(MY_LIB)

#webserver: csapp.o web.o parse_conf.o  main.o
#	$(CC) $(OPTION) csapp.o parse_conf.o web.o main.o $(THRID_LIBS)/cJSON.o -o webserver \
#	$(LIBS) \
#	-L$(MY_LIB_DIR)/zlog
#main.o: main.c 
#	$(CC) $(OPTION) -c main.c $(CFLAGS) -I$(MY_INCLUDE) 
#web.o: web.c 
#	$(CC) $(OPTION) -c web.c $(CFLAGS) -I$(MY_INCLUDE) 
#parse_conf.o: parse_conf.c 
#	$(CC) $(OPTION) -c parse_conf.c $(CFLAGS) -I$(MY_INCLUDE)
#csapp.o: csapp.c 
#	$(CC) $(OPTION) -c csapp.c $(CFLAGS)
#clean:
#	rm -rf *.o webserver
