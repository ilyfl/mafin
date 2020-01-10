#include "fin.h"

uint8_t get_str(char str[])
{
	char c=getchar();
	uint8_t i=0;
	do {
		if(c=='\n')
		{
			return 1;
		}
		else
		{
			str[i]=c;	
		}
		++i;
	} while((c=getchar())!='\n');
	str[i]='\0';
	return 0;
}

uint8_t get_float(float *number){
	char buffer[64];
    	char *endptr;
	while(get_str(buffer))
		;
	
   	//if (fgets(buffer, sizeof(buffer), stdin) == NULL)
   	//    	return 1; /* Unexpected error */
   	*number = strtof(buffer, &endptr);
   	if ((*endptr == '\0') || (isspace(*endptr) != 0))
   	    	return 0;
   	else
		return 1;
}

void get_digit(uint8_t* digit)
{
	unsigned char c;
	while(c=getchar())
		if(isdigit(c))
			break;
		else
			continue;
	*digit=c-'0';
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

