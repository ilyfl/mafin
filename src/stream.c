
//this file make all stream work

#include "fin.h"
#include "stream.h"
#include <math.h>


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
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

//reads config file and sets all needed global vars
void co_assign(char* var, char* value)
{
    //divide value by separator ',' and assign tokens into array
    char* token=strtok(value, ",");
    int i = 0;
    while(token!=NULL)
    {
        token=trimwhitespace(token);
        strcpy((var+NAME_MAX*(i++)),token);
        token = strtok(NULL, ",");
    }

}

conf_t* read_config()
{
    FILE *config;
    char *buffer;
    char *token;
    char *ptr;
    struct stat st;

    conf_t* options=malloc(sizeof(conf_t));
    conf_t* head=options;
    options->next=NULL;

    if(stat(cfgPath,&st))
        return NULL;
    else if((config=fopen(cfgPath, "r"))==NULL)
		return NULL;	
    else if((buffer=malloc(st.st_size))==NULL)
    {
        fclose(config);
        return NULL;
    }
    else if((size_t)(fread(buffer, 1, st.st_size, config))!=(size_t)(st.st_size))
    {
        fclose(config);
        return NULL;
    }
    fclose(config);

    ptr=buffer;

    while(*ptr!='\0')
    {
        if((*ptr=='#'))
        {
            while(*ptr!='\n' && *ptr != '\0')
                ptr++;
        }
        else
        {
            token = ptr;
            while(*ptr != '\n' && *ptr != '\0')
            {
                if(*ptr=='=')
                {
                    // left side found
                    char *tmp=ptr;
                    if(options->next!=NULL)
                        options=options->next;
                    options->key=malloc(ptr-token);
                    strncpy(options->key, token, ptr-token);
                    *(options->key+(ptr-token))='\0';
                    options->key=trimwhitespace(options->key);
                    //right side
                    token = ptr+1;
                    if((tmp=strchr(ptr, '\n'))!=NULL); 
                    else tmp=strchr(ptr,'\0');
                    options->value = malloc(tmp-token);
                    strncpy(options->value, token, tmp-token);
                    *(options->value+(tmp-token))='\0';
                    options->value=trimwhitespace(options->value);

                    options->next=malloc(sizeof(conf_t));
                }
                ptr++;
            }
        }
        ptr++;
    }
    options->next=NULL;
	return head;
    /* 

    do{
       comment=strchr(ptr++,'#');
       if(!strncmp(ptr, global[i], strlen(global[i])))
       {
           //expect "=" and values;
           ptr=strchr(ptr,'=');
           if(*ptr++=='=')
           {
               token=strtok(ptr, ",");
               for(int j=0; token; j++)
               {
                   ptr=strchr(token, '\n'); 
                   if(ptr!=NULL)
                       token=strtok(NULL, "\n");
                   if(i==0)
                   {
                       token=trimwhitespace(token); 
                       strcpy(category[0][j], token);
                   } 
                   else if(i==1)
                   {
                       token=trimwhitespace(token); 
                       strcpy(category[1][j], token);
                   } 
                   else if(i==2)
                   {
                       token=trimwhitespace(token); 
                       strcpy(resource[j], token);
                   } 
                   else if(i==3)
                   {
                       token=trimwhitespace(token); 
                       strcpy(dbpath, token);
                   } 
                   token=strtok(NULL, ",");
               }
           }
           ++i;
           ptr=buffer;
       }
       if((comment!=NULL )&& *(comment)=='#')
       {
           while(*ptr++!='\n');
           continue;
       }
    }while(i<3 && *ptr);
   */ 
    
	//if((fgets(line, sizeof(line), configfd))==NULL)
	//	return 1;
	//*pos=(size_t)ftell(configfd);
    //
    //comment=strchr(line,'#');
	//if((comment!=NULL )&& *(comment)=='#')
	//	return 1;

	//if(*pos)
	//	fseek(configfd, *pos, SEEK_SET);
    //else
	//	fseek(configfd, *pos, SEEK_SET);
	//if((fgets(line, sizeof(line), configfd))==NULL)
	//	return 1;
	//*pos=(size_t)ftell(configfd);
    //
    //comment=strchr(line,'#');
	//if((comment!=NULL )&& *(comment)=='#')
	//	return 1;
    //
	//for(size_t i=0; i<sizeof(line); ++i)
	//{
	//	if(isblank(line[i]))
	//		continue;
	//	else if(!flag && line[i]=='=')
	//		flag=1;
	//	else if(!flag && line[i]==*s++)
	//		continue;
	//	else if(flag && isprint(line[i]))
	//		var[j++]=line[i];
	//	else{
	//		var[j]='\0';
	//		return !flag;
	//	}
	//}
}
//
uint8_t readdb(FILE* dbfd, answer_t* info, long* pos)
{
	char* 	infoline=malloc(DBLINE_MAX);
    char*   ptr=infoline;
	char*   cptr=info->comment;
	int*    time_ptr = &info->time.tm_sec;

	if(*pos)
		fseek(dbfd, *pos, SEEK_SET);
			
	if((fgets(infoline, DBLINE_MAX, dbfd))==NULL)
	{
        free(infoline);
		return 1;
	}

	*pos=ftell(dbfd);
	
	//init answer_t struct's elements with zeros
	memset(info, 0, sizeof(answer_t));

	//time parser
    do{
		if(isdigit(*ptr))
			*time_ptr=10*(*time_ptr)+*ptr-'0';		
		else if(*ptr=='-')
			++time_ptr;
    }while(*ptr!='|' && *ptr++!='\n');


	int8_t flag=0;
	int8_t a=1;
	int8_t e=0;
	//info parser
    
	do{
		while(isdigit(*ptr) && !flag)
		{
			info->tcr=10*info->tcr+*ptr-'0';
			++ptr;
		}
		while(isdigit(*ptr) && flag==1)
		{
			info->payload=10*info->payload+*ptr-'0';
			++ptr;	
		}
		while((isalnum(*ptr)||isblank(*ptr)) && flag==2)
		{
			*cptr++=*ptr++;	
		}
		if(*ptr=='.')
		{
			++ptr;
			while(isdigit(*ptr))
			{
				info->payload=10*info->payload+*ptr-'0';	
				e-=1;
				++ptr;
			}
		}
		else if(*ptr=='-')
			a=-1;
		if(*ptr==',')
			flag++;
	}while(*ptr!='\n' && *ptr++!='\0');

	info->tcr*=a;
	while(e)
	{
		info->payload*=0.1;
		++e;
	}
	
    free(infoline);
	return 0;
}

//goes to the end of file and adds new info
uint8_t storedb(answer_t* info, char* dbpath)
{
	FILE *db;
	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	

	fprintf(db,"%02d-%02d-%02d-%02d-%02d-%d|",info->time.tm_sec, info->time.tm_min, info->time.tm_hour, info->time.tm_mday, info->time.tm_mon, info->time.tm_year);	
	fprintf(db,"%d,%.2f,%s\n",info->tcr, info->payload, info->comment);

	fclose(db);
	
	return 0;
}

uint8_t chEntry_n(answer_t* info, char* dbpath, size_t number)
{
    if(rmEntry_n(dbpath, number))
        return 1;
    if(insEntry_n(info,dbpath,number))
        return 1;
    return 0;
}

uint8_t insEntry_n(answer_t* info,char* dbpath, size_t number) 
{
    FILE *db;
    char *buffer;
    char *ptr;
    char insLine[2*sizeof(*info)];
    size_t lineSize=1;
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

    ptr=buffer;
    do{
        if(ptr==buffer+st.st_size)
        {
            time_t t = time(NULL);
            info->time = *localtime(&t);
            storedb(info, dbpath);
            free(buffer);
            return 0;
        }
        else if(i==(number))
        {
            for(char *line = ptr; *line!='\n'; ++line)
                ++lineSize;
            break;
        }
        else if(*ptr == '\n') 
            ++i;
    }while(ptr++);

//char *insLine=malloc(2*sizeof(*info));
    size_t insLineSize;
    time_t t = time(NULL);
    info->time = *localtime(&t);

	sprintf(insLine,"%02d-%02d-%02d-%02d-%02d-%d|%d,%.2f,%s\n",info->time.tm_sec, info->time.tm_min, info->time.tm_hour, info->time.tm_mday, info->time.tm_mon, info->time.tm_year,info->tcr, info->payload, info->comment);	

    insLineSize=strlen(insLine);
    size_t pos=ptr-buffer;

    st.st_size+= insLineSize;
    remain=(buffer+st.st_size) - (ptr+lineSize);
    buffer = realloc(buffer, st.st_size);
    ptr=buffer+pos;
    memmove(ptr+lineSize, ptr, remain); 

    memcpy(ptr, insLine, insLineSize);

    if((db=fopen(dbpath, "w"))==NULL)
		return 1;	

    fwrite(buffer, 1, st.st_size , db);
    free(buffer);
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

    ptr=buffer;
    do{
        if(ptr==buffer+st.st_size)
        {
            free(buffer);
            return 1;
        }
        else if(i==number)
        {
           for(char *line = ptr; *line!='\n'; ++line)
                ++delete;
           break;
        }
        else if(*ptr == '\n') 
            ++i;
    }while(ptr++);
    remain=(buffer+st.st_size) - (ptr+delete);
    
    memmove(ptr, ptr+delete, remain); 

    if((db=fopen(dbpath, "w"))==NULL)
		return 1;	

    fwrite(buffer, 1, st.st_size - delete, db);
    free(buffer);
	fclose(db);
    return 0;
}

