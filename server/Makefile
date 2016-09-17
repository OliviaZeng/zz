SRCS += $(wildcard *.c)
SRCS += $(wildcard ../net/*.c)


OBJS = $(SRCS:.c = .o)

CC = gcc

CCFLAGS += -g -Wall -O0

INCLUDE = -I../net


server : $(OBJS)
	$(CC) $^ -o $@ $(INCLUDE)  $(LIBS)

%.o : %.c
	$(CC) -c $^ $(CCFLAGS)

clean:
	rm -rf *.o *~  server
