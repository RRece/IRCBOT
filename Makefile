# Makefile für MyApp
# ==================

# Include Dateien befinden sich hier
INCLUDE = ./include

# Compiler Optionen
CFLAGS = -g -Wall -ansi

#Welcher Compiler wird genutzt
CC = gcc

all: ircbot

ircbot: bot.o
	$(CC) -o ircbot bot.o 

bot.o:
	$(CC) -libircclient -lsqlite3 bot.c -I$(INCLUDE) $(CFLAGS)

clean:
	rm -f bot.o

install:
	cp ircbot $(INSTDIR); \
	chmod +x $(INSTDIR); \
	chmod og-w $(INSTDIR); \
	fi

 