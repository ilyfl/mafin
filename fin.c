

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include "fin.h"




uint8_t prompt(answer_t* f){
	system("clear");
	if(f->typecome)
	{
		printf("Category: Salary(0), Wages(1), Random(2): ");
		f->category=getchar();	
		switch(f->category){
			default:
			break;
		}
	}	
	else
	{

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
		printf("Password has to contain up to %d and more than %d characters!\n",MAX_INP_LEN, MIN_INP_LEN);
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
		printf("Name should be longer than 2 symbols and less then 12\n");	
		return 1;
	}
	return 0;

}

int main(int argc, char** argv)
{
	char dbname[MAX_INP_LEN];
//      char password[MAX_INP_LEN];

	system("clear");
start:
	//TODO: ask user for path to the directory where db stored
	//othewise default path will be used
	printf("Database name: ");
	if(get_name(dbname))
	{
		goto start;	
	}
//
//	printf("Password: ");
//	if(get_password(password))
//	{
//		goto start;
//	}

//	mysql_create(dbname);

	answer_t info;
	printf("%d\n", sizeof(user_t));
	printf("Outcome(0)/Income(1)?: ");	
	while((info.typecome=getchar()))
	if(info.typecome == '1' || info.typecome == '0')
	{
		info.typecome-='0';
		prompt(&info);
	}
	else{
		printf("Incorrect input!\n");
	}
}

