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

//Konfigurationsdatei laden
int configfileLaden(char * filen)
{
	printf(" : - Lade aus %s \n",filen);

	configFile = fopen(filen,"r");

	if(configFile == NULL)
	{
		printf("X: - Bot konnte Configfile nicht lesen... \n");
		fclose(configFile);
		return 0;
	}
	 
	char tmp[BUFFER_SIZE];

	while(fgets(configLine, BUFFER_SIZE, configFile) != NULL)
	{
		sprintf(tmp,"%s",configLine);
	}

	fclose(configFile);

	char params[10][100];
	int i=0;

	char *ptr =strtok(tmp,";");

	while(ptr !=NULL)
	{
		sprintf(params[i],"%s",ptr);
		ptr = strtok(NULL, ";");
		i++;
	}

	server = params[0];
	ctx.nick = params[1];
	ctx.channel = params[2];

	return 1;
}

//setzt Channel Einstellungen
void setChannelSettings(const char channel[],const unsigned int settings)
{
	int i;
	for(i=0;i<MAX_CHANNELS;i++)
	{
		if(strstr(chansettings[i].channel,channel))
		{
			chansettings[i].settings = settings;
			break;
		}
	}
}

//verläst Channel
void rmChannel(const char channel[])
{
	int i;
	for(i=0;i<MAX_CHANNELS;i++)
	{
	if(strstr(chansettings[i].channel,channel))
		{
			chansettings[i].channel[0] = 'X';
			break;
		}
	}
}

//Channel hinzufügen
void addChannelSettings(const char channel[],const unsigned int settings)
{
	strcpy(chansettings[(currentChannelCount-1)].channel,channel);
	chansettings[(currentChannelCount-1)].settings = settings;
	printf(" : - %s config set = %d\n",chansettings[(currentChannelCount-1)].channel,chansettings[(currentChannelCount-1)].settings);
}

//Git Channel Einstellungen aus
unsigned int getChannelSettings(const char channel[])
{
	int i;
	for(i=0;i<MAX_CHANNELS;i++)
	{
		//printf("%c-\n",chansettings[i].channel[0] );
		if(chansettings[i].channel[0] != '#')
		{
			addChannelSettings(channel,DEFAULT_CHANNEL_SETTINGS);
			return DEFAULT_CHANNEL_SETTINGS;
		}
		// printf("%s=%s",channel,chansettings[i].channel);

		if(strcmp(channel,chansettings[i].channel) == 0)
		{
			return chansettings[i].settings;
		}	
	}
	return(0);
}

//allgemeinen Channelfunktionen
void ircCommands(irc_session_t * session,const char * origin,const char ** params,unsigned int settings)
{
	if (settings & QUIT && !strcmp (params[1], "!quit") )
	{
	irc_cmd_quit (session, "Bot wird beendet...");
	}

	if(strstr(params[1],botnick))
	{
		irc_cmd_msg(session, params[0][0] =='#' ? params[0] : origin ,"me?");
	}

	if (settings & NICK_CHANGE && strstr (params[1], "!nick") == params[1] )
	{
		sprintf(botnick,"%s",params[1] + 6);
		irc_cmd_nick (session, params[1] + 6);
	}

	if(settings & JOIN_PART)
	{	
		if(currentChannelCount < MAX_CHANNELS)
		if (strstr (params[1], "!join") == params[1] )
		{
		irc_cmd_join (session, params[1] + 6,0);
		currentChannelCount++;
		}
	
		if (strstr (params[1], "!part") == params[1] )
		{
			if(strstr(params[0],ctx.channel) == 0)
			{
				irc_cmd_part (session, params[0]);
				rmChannel(params[0]);
				currentChannelCount--;
			}
			else
			{
				irc_cmd_msg(session,params[0],"Hier geh ich net raus");
			}	
		}
	}

	if(settings & GET_TIME && strstr(params[1],"!time"))
	{	
		char tmp[60];
		time ( &rawtime );
		ptm = gmtime ( &rawtime );

		sprintf(tmp,"Es ist %2d:%02d\n", (ptm->tm_hour+MESZ)%24, ptm->tm_min);
		irc_cmd_msg(session, params[0][0] =='#' ? params[0] : origin ,tmp);
	}

	if(settings & TOPIC_CHANGE)
	{
		if ( !strcmp (params[1], "!topic") )
		{
			irc_cmd_topic (session, params[0], 0);
		}
		else if ( strstr (params[1], "!topic ") == params[1] )
		{
			irc_cmd_topic (session, params[0], params[1] + 7);
		}
	}


	if(strstr(params[1],"!status"))
	{
		char tmp[80];
		sprintf(tmp,"Channelsettings = %d",getChannelSettings(params[0]));
		irc_cmd_msg(session,params[0],tmp);
	}

	if(strstr(params[1],"!set"))
	{
		int settings;
		char line[200];
		char parts[32][32];
		int i = 0;

		strcpy(line,params[1]);

		char *ptr =strtok(line," ");

		while(ptr !=NULL)
		{
			sprintf(parts[i],"%s",ptr);
			ptr = strtok(NULL, " ");
			i++;
		}

		settings = atoi(parts[2]);

		if(strstr(parts[2],"privmsg"))
		{	
			privmsg_settings = settings;	
		}
		else
		{
			setChannelSettings(parts[1],settings);
		}
	}

	if(strstr(params[1],"!debug"))
	{	
		int i;
		for(i=0;i<MAX_CHANNELS;i++)
		{
		printf("%d=%s=%d\n",i,chansettings[i].channel,chansettings[i].settings);
		}
	}
}


int main()
{
	return 1;
}
