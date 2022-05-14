OBJS = cli.c srv.c

CFLAGS = -g

CC = gcc

all: $(OBJS)
	$(CC) cli.c -o cli
	$(CC) srv.c -o srv

clean: 
	rm -rf $(EXEC)
