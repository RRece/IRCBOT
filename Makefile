CC = gcc
LIBS = include/libircclient.o 
CFLAGS = -Wall -g
LDFLAGS = -lsqlite3
NAME = ircbot

install:
	$(CC) -o $(NAME) bot.c $(LIBS) $(LDFLAGS) $(CFLAGS)
clean:
	rm *.o $(NAME)
