
//this file make all stream work

#include "fin.h"
#include "stream.h"


//currently writes on the first line only
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
	char	timeline[21];
	char 	infoline[sizeof(answer_t)];
	FILE* dbfd;
	static long pos;
	

	if((dbfd=fopen(dbpath, "r"))==NULL)
	{
		return 1;
	}

	if(pos)
		fseek(dbfd, pos, SEEK_SET);
			
	if((fgets(timeline, sizeof(timeline), dbfd))==NULL)
	{
		return 1;
	}
	if((fgets(infoline, sizeof(infoline), dbfd))==NULL)
	{
		return 1;
	}
	pos=ftell(dbfd);
	
	//init info->time struct's elements with zeros
	int* ptr = &info->time.tm_sec;
	memset(ptr, 0, sizeof(struct tm));

	ptr = &info->time.tm_mday;	
	for(uint8_t i=0; i<sizeof(timeline);++i)
	{
		if(isdigit(timeline[i]))
			*ptr=10*(*ptr)+timeline[i]-'0';		
		else if(timeline[i]=='-')
			ptr++;
		else if(timeline[i]=='/')
			ptr=&info->time.tm_hour;
		else if(timeline[i]==':')
			ptr--;
		else
			continue;
	}	

		
	//standardizing
	info->time.tm_mon-=1;
	info->time.tm_year-=1900;

//not working properly	
//need string to float converter 
//identifies '-' as digit
	uint8_t* uiptr = &info->typecome;
	float * fptr = &info->payload;
	*fptr=0;
	memset(uiptr, 0, 64);
	char* cptr = info->comment;
	for(uint8_t i=0; i<strlen(infoline);++i){
		if(isdigit(infoline[i]))
		{
			*uiptr=10*(*uiptr)+infoline[i]-'0';
			uiptr++;
		}
		else if(ispunct(infoline[i]))
			continue;
		else if(isalnum(infoline[i]))
		{
			*uiptr=infoline[i];
			uiptr++;	
		}


	}
	
	fclose(dbfd);
	return 0;
}

//goes to the end of file and adds new info
uint8_t storedb(answer_t* info, char* dbpath)
{
	FILE *db;
	time_t t = time(NULL);
	info->time = *localtime(&t);

	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	
	fprintf(db,"%02d-%02d-%d/%02d:%02d:%02d|",info->time.tm_mday, info->time.tm_mon+1, info->time.tm_year+1900, info->time.tm_hour, info->time.tm_min, info->time.tm_sec);	
	fprintf(db,"%d,%d,%d,%f,%s\n", info->typecome, info->category, info->resource, info->payload, info->comment);

	fclose(db);
	
	return 0;
}
