

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include "fin.h"

uint8_t read_config()
{
	FILE* config;

	if ((config=fopen(cfgPath,"r"))==NULL)
	{
		return 1;
	}
	

	uint8_t n;
	n=fscanf(config, "dbpath=%s",dbpath);
	if(n!=1)
	{
		return 10;
	}

	fclose(config);
	return 0;
}

void print_categories(uint8_t typecome){
	for(int i = 0; i < CAT_MAX; i++)
			printf("%s(%d), ", categories[typecome][i], i);
}


uint8_t prompt(answer_t* info){
//	char dbname[NAME_MAX];
//      char password[NAME_MAX];


//	system("clear");
	uint8_t flag;
start:

	if((flag=read_config())==1){
		system("mkdir /home/$USER/.config/mafin && touch /home/$USER/.config/mafin/config");	
		goto start;
	}
	else if(flag==10){
		printf("Please input a path to the database(/home/$USER/mafin/finances): ");	
		char c=getchar();
		uint8_t i=0;
		do {
			if(c=='\n')
			{
				//poka takoi kostyl'
				FILE *config;
				config=fopen(cfgPath, "w+");
				fprintf(config, "dbpath=%s\n",dbpath);
				fclose(config);
				break;
			}
			else
			{
				dbpath[i]=c;	
			}
			++i;
		} while((c=getchar())!='\n');
		dbpath[i]='\0';
		printf("%s\n", dbpath);
	}


//	printf("Database name: ");
//	if(get_name(dbname))
//	{
//		goto start;	
//	}
	
//
//	printf("Password: ");
//	if(get_password(password))
//	{
//		goto start;
//	}

//	mysql_create(dbname);

	printf("Outcome(0)/Income(1)?: ");	
	while((info->typecome=getchar()))
	if(info->typecome == '1' || info->typecome == '0')
	{
		info->typecome-='0';
		break;
	}
	else{
		printf("Incorrect input!\n");
	}

	if(info->typecome)
	{
		printf("INCOME\n");
		print_categories(info->typecome);
		info->category=getchar();	
		switch(info->category){
			default:
			break;
		}
	}	
	else
	{
		printf("EXPENSE\n");
		print_categories(info->typecome);


	}
	return 0;
}


void mysql_create(const char* db){
	MYSQL *sqlfd = mysql_init(NULL);
	if (sqlfd == NULL)
	{
		printf("%s\n", mysql_error(sqlfd));
		exit(1);
	}

	if (mysql_real_connect(sqlfd, "localhost", "root", "1",NULL, 0, NULL, 0))
	{
		printf("%s\n", mysql_error(sqlfd));
		mysql_close(sqlfd);
		exit(1);
	}

	char db_create[64]= "CREATE DATABASE ";
	strcat(db_create, db);

	if (mysql_query(sqlfd, db_create)){
		printf("%s\n", mysql_error(sqlfd));
		mysql_close(sqlfd);
		exit(1);
	}


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


uint8_t get_name(char *name){
	uint8_t i;

	fgets(name, sizeof(name), stdin);
	
	uint8_t len = strlen(name)-1;	
	name[len]='\0';

	for(i=0; i<len; i++)
	{
		if(!isalnum(name[i]))
		{
			printf("Name can include only alphanumeric characters!\n");
			return 1;
		}
	}
	if( !(i>=MIN_INP_LEN))
	{
		printf("Name should be longer than %d symbols and less then %d\n",MIN_INP_LEN, NAME_MAX);	
		return 1;
	}
	return 0;

}

uint8_t get_username(char **name)
{
	*name=getenv("USER");
	if(*name==NULL) return 1;
	return 0;
}

int main(int argc, char** argv)
{
	strcat(cfgPath, getenv("USER"));
	strcat(cfgPath, "/.config/mafin/config");

	strcat(dbpath, getenv("USER"));
	strcat(dbpath, "/mafin/finances");

	answer_t info;
	prompt(&info);
//	printf("%d\n", sizeof(user_t));
}

