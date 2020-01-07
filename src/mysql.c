#include "fin.h"

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
