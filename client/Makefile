SRCS += $(wildcard *.c) $(wildcard ../net/*.c)


OBJS = $(SRCS:.c = .o)

CC = gcc

CCFLAGS += -g -Wall -O0

INCLUDE = -I../net


client : $(OBJS)
	$(CC) $^ -o $@ $(INCLUDE)  $(LIBS)

%.o : %.c
	$(CC) -c $^ $(CCFLAGS)

clean:
	rm -rf *.o *~  client
