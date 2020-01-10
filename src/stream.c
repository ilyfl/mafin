
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
	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	

	fprintf(db,"%d,%d,%f,%s,%d\n", info->typecome, info->category, info->payload, info->comment, info->resource);

	fclose(db);
	
	return 0;
}