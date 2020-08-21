
#if 0
this file make all stream work
#endif 
#include "fin.h"
#include "stream.h"


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
//sets global vars
uint32_t config_options_assign(char* var, char* value)
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

    return i;
}
//free config options linked list
void config_options_free(conf_t* options)
{
    conf_t* prev=options;
    while(options!=NULL)
    {
        free(options->value);
        free(options->key);
        options=options->next;
        free(prev);
        prev=options;
    }
}

//reads config file 
conf_t* config_read()
{
    FILE *config;
    char *buffer;
    char *token;
    char *ptr;
    struct stat st;

    conf_t* options=malloc(sizeof(*options));
    conf_t* head=options;
    options->next=NULL;

    if(stat(cfgPath,&st))
        return NULL;
    else if(!st.st_size)
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
        if(*ptr=='#')
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
    
                    //right side
                    token = ptr+1;
                    if((tmp=strchr(ptr, '\n'))!=NULL); 
                    else tmp=strchr(ptr,'\0');
                    options->value = malloc(tmp-token);
                    
                    strncpy(options->value, token, tmp-token);
                    *(options->value+(tmp-token))='\0';

                    options->next=malloc(sizeof(conf_t));
                }
                ptr++;
            }
        }
        ptr++;
    }
    free(options->next);
    options->next=NULL;
    free(buffer);
	return head;
}
uint8_t db_read_info_buffer(char** buffer, info_t* info) {

    char infoline[DBLINE_MAX];
    uint64_t i=0;
    char* ptr;

	int*    time_ptr = &info->time.tm_sec;
    uint8_t* info_ptr = &info->typecome;

    char *token;

    if(**buffer == '\0')
        return 1;
    while(**buffer != '\n')
    {
        infoline[i++]=**buffer;
        (*buffer)++;
    }
    (*buffer)++;
    infoline[i]='\0';
    ptr=&infoline[0];

	//time parser
    token = strtok(ptr,"-");
    while(token!=NULL)
    {
        *time_ptr++ =atoi(token);
        token=strtok(NULL,"-");
    } 
    while(*ptr++!='|');

	//info parser
    
    token=strtok(ptr,",");
    while(token!=NULL)
    {
        *info_ptr++ =atoi(token);
        token=strtok(NULL,",");
    }
    while(*ptr++!='|');
    token=ptr;
    while(*++ptr!='|');
    info->payload = strtof(token,&ptr);
    token=ptr+1; 
    while(*ptr++!='\0');
    if((ptr-(token+1))!=0)
    {
        info->comment=malloc(ptr-token+1);
        strncpy(info->comment, token, ptr-token);
        *(info->comment+(ptr-token))='\0';
    }
    else 
        info->comment=NULL;

    return 0;
}

uint8_t db_read_info(FILE* dbfd, info_t* info, uint64_t* pos)
{
	char* 	infoline=malloc(DBLINE_MAX);
    char*   ptr=infoline;
	int*    time_ptr = &info->time.tm_sec;
    uint8_t* info_ptr = &info->typecome;

    char *token;

	if(*pos)
		fseek(dbfd, *pos, SEEK_SET);
			
	if((fgets(infoline, DBLINE_MAX, dbfd))==NULL)
	{
        free(infoline);
		return 1;
	}

	*pos=ftell(dbfd);
	
	memset(info, 0, sizeof(info_t));

	//time parser
    token=strtok(ptr,"-");
    while(token!=NULL)
    {
        *time_ptr++ =atoi(token);
        token=strtok(NULL,"-");
    } 
    while(*ptr++!='|');

	//info parser
    
    token=strtok(ptr,",");
    while(token!=NULL)
    {
        *info_ptr++ =atoi(token);
        token=strtok(NULL,",");
    }
    while(*ptr++!='|');
    token=ptr;
    while(*++ptr!='|');
    info->payload=strtof(token,&ptr);
    token=ptr; 
    token++;
    while(*++ptr!='\n');
    if((ptr-token)!=0)
    {
        info->comment=malloc(ptr-token+1);
        strncpy(info->comment, token, ptr-token);
        *(info->comment+(ptr-token))='\0';
    }

    free(infoline);
	return 0;
}

uint8_t db_store_info(info_t* info, char* dbpath)
{
	FILE *db;
	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	

	fprintf(db, FORMAT ,info->time.tm_sec, info->time.tm_min, info->time.tm_hour, info->time.tm_mday, info->time.tm_mon, info->time.tm_year, info->typecome,info->category,info->resource,info->currency, info->payload);	
    if(info->comment!=NULL)
    {
        fprintf(db,"%s\n", info->comment);
        free(info->comment);
    }
    else fprintf(db,"\n");

	fclose(db);
	
	return 0;
}
#if 0
uint8_t db_change_entry_by_num(info_t* info, char* dbpath, size_t number)
{
    FILE *db;
    char *buffer;
    char *ptr;
    char insLine[2*sizeof(*info)];
    uint64_t lineSize=1;
    uint64_t remain=0;
    uint64_t i=1;  
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
    return 0;
}
#endif

uint8_t db_ins_entry_by_num(info_t* info,char* dbpath, uint64_t number) 
{
    FILE *db;
    char *buffer;
    char *ptr;
    char insLine[2*sizeof(*info)];
    uint64_t lineSize=1;
    uint64_t remain=0;
    uint64_t i=1;  
    struct stat st;
    size_t insLineSize;
    time_t t;
    size_t pos;

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
            db_store_info(info, dbpath);
            free(buffer);
            return 0;
        }
        else if(i==(number))
        {
            char *line;
            for(line = ptr; *line!='\n'; ++line)
                ++lineSize;
            break;
        }
        else if(*ptr == '\n') 
            ++i;
    }while(ptr++);

//char *insLine=malloc(2*sizeof(*info));
    t = time(NULL);
    info->time = *localtime(&t);

    sprintf(insLine, FORMAT ,info->time.tm_sec, info->time.tm_min, info->time.tm_hour, info->time.tm_mday, info->time.tm_mon, info->time.tm_year,info->typecome,info->category,info->resource,info->currency, info->payload );	

    if(info->comment != NULL)
    {
        strcat(insLine,info->comment);	
        strcat(insLine,"\n");
        free(info->comment);
    }
    else strcat(insLine,"\n");	

    insLineSize=strlen(insLine);

    pos=ptr-buffer;

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

uint8_t db_rm_entry_by_num(const char* dbpath, int64_t number) 
{
    FILE *db;
    char *buffer;
    char *ptr;
    uint64_t delete=1;
    uint64_t remain=0;
    uint64_t i=1;  
    struct stat st;

    if(number == -1)
    {
        char command[INP_MAX]="rm ";
        strcat(command, dbpath);
        system(command);
        strcpy(command, "touch ");
        strcat(command, dbpath);
        system(command);
        return 0;
    }

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

        else if(i==(uint64_t)number)
        {
           char *line;
           for(line = ptr; *line!='\n'; ++line)
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


int db_from_sheet_convert(info_t* info, char* dbpath)
{

    char c = 'a';
    uint64_t size=2*sizeof (char);
    char *buffer = malloc(size);
    char *ptr = buffer;
    uint32_t i=0;
    int j;
    char* token;

    while((c = getchar())!=EOF)
    {
        if(c == '\n')
        {
            *++ptr='\0';
            //time parser
            strptime(buffer, "%d.%m.%Y %H:%M:%S", &info->time); 

            ptr = buffer;
            for(j = 0; j < 2; ptr++)
                if(*ptr==' ')
                    j++; 

            info->typecome=0;
            info->currency=0;
            info->comment=NULL;
            if(!strncmp(ptr, "Food", 4))
                info->category=0;
            else if(!strncmp(ptr, "EatingOut", 9))
                info->category=1;
            else if(!strncmp(ptr, "Entertainment", 13))
                info->category=2;
            else if(!strncmp(ptr, "Transport", 9))
                info->category=3;
            else if(!strncmp(ptr, "Bills", 5))
                info->category=4;
            else if(!strncmp(ptr, "Clothes", 7))
                info->category=5;
            else if(!strncmp(ptr, "Health", 6))
                info->category=6;
            else if(!strncmp(ptr, "Phone", 5))
                info->category=7;
            else if(!strncmp(ptr, "Toiletry", 8))
                info->category=8;
            else if(!strncmp(ptr, "Other", 5))
                info->category=9;
            else if(!strncmp(ptr, "Dollars", 7))
                info->category=10;

            while(*ptr!=' ') ptr++;
            token=ptr;
            while(*ptr!=' ') ptr++;
            info->payload = (strtof(token, &ptr));
            ++ptr;
resource:
            if(*ptr == ' ')
            {
                token = ptr+1;
                while(*ptr!='\0') ptr++;
                if(!strncmp(token, "Cash", 4))
                    info->resource = 0;
                else if(!strncmp(token, "Pivdennyi", 9))
                    info->resource = 1;
            }
            else 
            {
                token = ptr;
                while(*ptr!=' ') ptr++;
                info->comment = malloc(ptr-token);
                strncpy(info->comment, token, ptr-token);
                *(info->comment+(ptr-token))='\0';
                goto resource;
            }
            db_store_info(info, dbpath);

            i=0;
            size = 2 * sizeof (char);
        }
        else 
        {
            buffer = realloc(buffer, ++size); 
            ptr = buffer + i++;
            *ptr = c;
        }
    }
    free(buffer);
	return 0;
}
