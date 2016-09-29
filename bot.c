#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "./include/libircclient.h"
#include "sqlite3.h"

//libircclient 
typedef struct
{
	char 	* channel;
	char 	* nick;

} irc_ctx_t;

typedef struct
{
	char channel[32];
	unsigned int settings;
}channel_settings;

//SQLite
#define SQLITEFILE "sqlite.sqlite"

sqlite3 *sqlitedb = NULL;

//Gloabel Variablen
#define LOGFILE "log.txt"
#define MAX_CHANNELS 10

irc_ctx_t ctx;
char * server = NULL;
short unsigned int port = 6667;
char botnick[100];
unsigned int currentChannelCount=0;

//Konfigurations Variablen
#define BUFFER_SIZE 1000

char* filename = NULL;
char configLine[BUFFER_SIZE];
FILE *configFile;

//IRC Komandos
#define TOPIC_CHANGE 1
#define NICK_CHANGE 2
#define JOIN_PART 4
#define QUIT 8
#define GET_TIME 16
#define LOG_TXT 32
#define URL_SQL 64

//IRC Defaults
#define DEFAULT_CHANNEL_SETTINGS 31
#define DEFAULT_PRIVMSG_SETTINGS 127

unsigned int privmsg_settings = DEFAULT_PRIVMSG_SETTINGS;
channel_settings chansettings[MAX_CHANNELS];

//Zeit
#define MESZ (+2)
time_t rawtime;
struct tm * ptm;


//in Logfile schreiben
void log_file(const char name[],const char channel[],const char text[])
{
	time ( &rawtime );
	ptm = gmtime ( &rawtime );

	FILE * log = fopen(LOGFILE,"a");

	fprintf(log,"%02d.%02d.%4d - %02d:%02d:%02d - <%s> %s: %s\n",ptm->tm_mday,ptm->tm_mon,ptm->tm_year+1900,(ptm->tm_hour+MESZ)%24, ptm->tm_min,ptm->tm_sec,channel,name,text);
	fclose(log);

}


int main()
{
	return 1;
}
