
//this file make all stream work

#include "fin.h"


uint8_t write_config(const char *s, const char* var)
{
	FILE* config;
	if ((config=fopen(cfgPath,"a"))==NULL)
	{
		return 1;
	}

	fprintf(config,"%s=%s\n", s, var);
	fclose(config);
	return 0;
}

uint8_t read_config(const char *s, char* var)
{
	FILE* config;

	if ((config=fopen(cfgPath,"r"))==NULL)
	{
		return 1;
	}
	

	char format[16]="";
	strcat(format, s);
	strcat(format, "=%s");
	if(fscanf(config, format, var)!=1)
	{
		fclose(config);
		return 10;
	}

	fclose(config);
	return 0;
}

uint8_t readdb(char* dbpath, answer_t* info)
{
	FILE *db;
	if((db=fopen(dbpath, "r"))==NULL)
	{	
		return 1;	
	}	
	
	fclose(db);
	return 0;
}
uint8_t storedb(answer_t* info, char* dbpath)
{
	FILE *db;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	
	fprintf(db,"%02d-%02d-%d/%02d:%02d:%02d|",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);	
	fprintf(db,"%d,%d,%f,%s,%d\n", info->typecome, info->category, info->payload, info->comment, info->resource);

	fclose(db);
	
	return 0;
}
