IRC Bot

Inhaltsverzeichnis

1. Installation
2. Bot starten
3. Konfigurationsdateien
4. Chat Befahle
5. Chat Einstellungen

1. Installation

mit ./make install

oder

gcc -o ircbot bot.c -g -Wall include/libircclient.o -lsqlite3


2. Bot starten

Der Bot wird aus der Komandokonsole gestartet. Die erfolgt mit dem Befehl:

	./ircbot SERVER NICK "#CHANNEL"

oder per Konfigurationsdatei

	./ircbot KONFIGURATIONSDATEI
	
3. Konfigurationsdatei

Die Konfigurationsdatei besteht aus einer Zeile. Sie muss wie folgt aufgebaut sein.

	IRCSERVER;BOTNAME;#CHANNEL;

4. Chat Befehle

Der Bot reagiert im Maser Channel(erste angegebene Kanal) auf folgende Befehle.

	!quit			Beendet den Bot
	!nick <neuer Name>	Bot ändert seinen Namen
	!join #CHANNEL		Bot joint einem Channel
	!part			Bot verlässt aktuellen Channel (außer den Master Channel)
	!topic <neues Topic>	Bot ändert topic
	!time	 		Bot gibt Serverzeit aus
	!set #CHANNEL <Settings>Bot legt Einstellungen im Channel fest
	!status			Bot gibt Einsellung im aktuellen Channel aus
	!geturls		Bot gibt die letzten 5 URLs aus (wenn er sie mitloggt)
	
	Der Bot reagiert auf seinen Namen. Dies dient als Ping.
	
5. Chat Einstellungen

Die Einstellungesn de Bots in einem Channel sind Bitcodiert

 1	Topic ändern
 2	Nick ändern
 4	Channel beitreten/verlassen
 8	Bot Beenden
16	Zeit ausgeben
32	In Textdatei loggen
64	URLs loggen

Beispiele:
Bot darf nur das Topic ändern
	!set #CHANNEL 1
	
Bot soll URls loggen und darf beendet werden

	!set #Channel 72
	
	(72 = 8 + 64)


