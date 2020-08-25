#include "fin.h"
#include "misc.h"

uint8_t get_str(FILE* stream, char* str)
{
	char c=fgetc(stream);
    if(c=='\n') return 1;

	while(c!='\n' && c!='\0' && c!=EOF) {
        *str++=c;	
        c=fgetc(stream);
	} 
	*str='\0';
	return 0;
}
uint8_t get_digit(FILE* stream, uint8_t left, uint8_t right)
{
	uint8_t c;
	while((c=fgetc(stream)))
		if(isdigit(c) && within(c-'0',left,right))
			break;
		else
			return 0;
	c=c-'0';
    return c;
}

float get_float(FILE* stream){
    float number;
	char buffer[INP_MAX];
    char *endptr;
	while(get_str(stream,buffer))
		;

   	number = strtof(buffer, &endptr);
   	if ((*endptr == '\0') || isspace(*endptr))
        return number;
   	else
		return 1;
}
int get_number(FILE* stream, int left, int right)
{
    int number=0;
    int c;
	while((c=fgetc(stream))!='\n')
		number+=10*number+c;
    if(within(number,left,right))
        return number;
    else return -1;
} 


uint8_t get_name(char *name){
    uint8_t len;
	if(fgets(name, sizeof(name), stdin))
	{
		return 1;
	}
	
	len = strlen(name)-1;	
	name[len]='\0';
	return 0;
}

uint8_t get_username(char **name)
{
	*name=getenv("HOME");
	if(*name==NULL) return 1;
	return 0;
}

//TODO: no echo input
uint8_t get_password(char *password)
{
    uint8_t len;
	fgets(password, sizeof(password), stdin);

	len = strlen(password)-1;	
	password[len]='\0';

	if((strlen(password) < MIN_INP_LEN))
	{
		printf("Password has to contain up to %d and more than %d characters!\n",NAME_MAX, MIN_INP_LEN);
		return 1;
	}
	return 0;
}

double to_default_currency(info_t* info)
{
    uint8_t curr = info->currency;

    switch(curr){

        case 0:
            return info->payload;
        case 1:
            return info->payload*27;    
        case 2:
            return info->payload*30;    
        default:
            return info->payload;

    }
    
}

