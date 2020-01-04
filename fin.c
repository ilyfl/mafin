

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "fin.h"

void prompt(t_answer* form);
void mysql_create(const char * username,const char * password);
void get_pass_from_user(char* password);
void get_username(char *username);

void prompt(t_answer* f){
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
}


void mysql_create(const char* user, const char* pass){
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

	char db_create[32]= "CREATE DATABASE ";
	strcat(db_create, user);
	strcat(db_create, "_fin");

	if (mysql_query(sqlfd, db_create)){
		printf("%s\n" mysql_error(sqlfd));
		mysql_close(sqlfd);
		exit(1);
	}


}

void get_username(){

}

int main()
{
	char username[11];
	char password[16];
start:
       	printf("Username: ");
	fgets(username, sizeof(username), stdin);

	if(!strlen(username) > 2)
	{
		printf("Username should be longer than 2 symbols\n");	
		goto start;
	}

	for(unsigned char i=0; i<11; i++)
	{
		if(isalnum(username[i]))
			;
		else
		{
			printf("Username can include only alphanumeric characters!\n");
			goto start;
		}
	}

	printf("Password: ");
	fgets(password, sizeof(password), stdin);

	if(strlen(password)<6)
	{
		printf("Password has to contain more than 6 characters!\n";
		goto start;
	}

	mysql_create(username,password);

	t_answer info;
	printf("%d\n", sizeof(t_answer));
	printf("Outcome(0)/Income(1)?: ");	
	info.typecome=getchar();
	if(info.typecome == '1' || info.typecome == '0')
	{
		info.typecome-='0';
		prompt(&info);
	}
	else{
		printf("Incorrect input!\n");
		exit(1);
	}
}

