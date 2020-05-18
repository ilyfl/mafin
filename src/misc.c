#include "fin.h"
#include "misc.h"

uint8_t get_str(FILE* stream, char* str)
{
	char c=fgetc(stream);
	uint8_t i=0;
    while(c=='\n')
        return 1;
	while(c!='\n' && c!='\0' && c!=EOF) {
        str[i]=c;	
		++i;
        c=fgetc(stream);
	} 
	str[i]='\0';
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
    else return NULL;
} 


uint8_t get_name(char *name){
	if(fgets(name, sizeof(name), stdin))
	{
		return 1;
	}
	
	uint8_t len = strlen(name)-1;	
	name[len]='\0';
	return 0;
}

uint8_t get_username(char **name)
{
	*name=getenv("USER");
	if(*name==NULL) return 1;
	return 0;
}

//TODO: no echo input
uint8_t get_password(char *password)
{
	fgets(password, sizeof(password), stdin);

	uint8_t len = strlen(password)-1;	
	password[len]='\0';

	if((strlen(password) < MIN_INP_LEN))
	{
		printf("Password has to contain up to %d and more than %d characters!\n",NAME_MAX, MIN_INP_LEN);
		return 1;
	}
	return 0;
}

double to_default_currency(double value)
{
    return value;
}

