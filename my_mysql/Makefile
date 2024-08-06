SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c, %.o, $(SRCS))
CC:=gcc
FLAGS:=-g -lmysqlclient
BIN:= db_test

ALL:$(OBJS)
	$(CC) $^ -o $(BIN) $(FLAGS)

%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS) $(BIN)

