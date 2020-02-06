
//this file make all stream work

#include "fin.h"
#include "stream.h"
#include "math.h"


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

uint8_t read_config(FILE* configfd, const char *s, char* var)
{
	char line[PATH_MAX];
	static long pos;
	uint8_t flag=0;
	size_t j=0;

	if(pos)
		fseek(configfd, pos, SEEK_SET);
	if((fgets(line, sizeof(line), configfd))==NULL)
		return 1;
	pos=ftell(configfd);
	if(line[0]=='#')
		return 1;
    
	for(size_t i=0; i<sizeof(line); ++i)
	{
		if(isblank(line[i]))
			continue;
		else if(!flag && line[i]=='=')
			flag=1;
		else if(!flag && line[i]==*s++)
			continue;
		else if(flag && isprint(line[i]))
			var[j++]=line[i];
		else{
			var[j]='\0';
			return !flag;
		}
	}
	return 0;
}

//2 or 3 times faster 
uint8_t readdb(FILE* dbfd, answer_t* info, long* pos)
{
	char	timeline[20];
	char 	infoline[sizeof(answer_t)];

	if(*pos)
		fseek(dbfd, *pos, SEEK_SET);
			
	if((fgets(timeline, sizeof(timeline), dbfd))==NULL)
	{
		return 1;
	}
	if((fgets(infoline, sizeof(infoline), dbfd))==NULL)
	{
		return 1;
	}
	*pos=ftell(dbfd);
	
	//init info struct's elements with zeros
	memset(info, 0, sizeof(*info));

	int* ptr = &info->time.tm_sec;
	//time parser
	for(uint8_t i=0; i<sizeof(timeline);++i)
	{
		if(isdigit(timeline[i]))
			*ptr=10*(*ptr)+timeline[i]-'0';		
		else if(ispunct(timeline[i]))
			++ptr;
		else
			continue;
	}	

	char* cptr=&info->comment;

	int8_t flag=0;
	int8_t a=1;
	int8_t e=0;
	//info parser
	for(uint8_t i=0; i<strlen(infoline);++i){
		while(isdigit(infoline[i]) && !flag)
		{
			info->tcr=10*info->tcr+infoline[i]-'0';
			++i;
		}
		while(isdigit(infoline[i]) && flag==1)
		{
			info->payload=10*info->payload+infoline[i]-'0';
			++i;	
		}
		while((isalnum(infoline[i])||isblank(infoline[i])) && flag==2)
		{
			*cptr++=infoline[i++];	
		}
		if(infoline[i]=='.')
		{
			++i;
			while(isdigit(infoline[i]))
			{
				info->payload=10*info->payload+infoline[i]-'0';	
				e-=1;
				++i;
			}
		}
		else if(infoline[i]=='-')
			a=-1;
		if(infoline[i]==',')
			flag++;
	}

	info->tcr*=a;
	while(e)
	{
		info->payload*=0.1;
		++e;
	}
	
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
	fprintf(db,"%02d-%02d-%02d-%02d-%02d-%d|",info->time.tm_sec, info->time.tm_min, info->time.tm_hour, info->time.tm_mday, info->time.tm_mon, info->time.tm_year);	
	fprintf(db,"%d,%.2f,%s\n",info->tcr, info->payload, info->comment);

	fclose(db);
	
	return 0;
}


uint8_t rmEntry_n(const char* dbpath, size_t number) 
{
    FILE *db;
    char *buffer;
    char *ptr;
    size_t delete=1;
    size_t remain=0;
    size_t i=1;  
    struct stat st;


    if(stat(dbpath,&st))
        return 1;
    else if((db=fopen(dbpath, "r"))==NULL)
		return 1;	
    else if((buffer=malloc(st.st_size))==NULL)
    {
        fclose(db);
        return 1;
    }
    else if((size_t)(fread(buffer, 1, st.st_size, db))!=(size_t)(st.st_size))
    {
        fclose(db);
        return 1;
    }
    fclose(db);
    

    for(ptr=buffer; *ptr!=EOF; ++ptr)
    {
        if(i==number)
        {
           for(char *line = ptr; *line!='\n'; ++line)
                ++delete;
           break;
        }
        else if(*ptr == '\n') 
            ++i;
    }
    remain=(buffer+st.st_size) - (ptr+delete);
    
    memmove(ptr, ptr+delete, remain); 

    if((db=fopen(dbpath, "w"))==NULL)
		return 1;	

    fwrite(buffer, 1, st.st_size - delete, db);
    free(buffer);
	fclose(db);
    return 0;
}

