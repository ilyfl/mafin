#include "fin.h"

	char cfgPath[PATH_MAX]="/home/";
	char dbpath[PATH_MAX]="/home/";



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

uint8_t read_config(const char *s, char* var)
{
	FILE* config;

	if ((config=fopen(cfgPath,"r"))==NULL)
	{
		return 1;
	}
	

	char format[16]="";
	strcat(format, s);
	strcat(format, "=%s");
	if(fscanf(config, format, var)!=1)
	{
		fclose(config);
		return 10;
	}

	fclose(config);
	return 0;
}

void print_categories(const uint8_t typecome){
	char categories[2][CAT_MAX][NAME_MAX]={{"Food", "Eating", "Entertainment", "Transport", "Bills", "Clothes", "Health", "Phone", "Toiletry", "Other"},{"Salary", "Wages", "Random"}};
	printf("==============================\n");
	for(uint8_t i = 0; i < CAT_MAX; ++i)
		if(isalnum(categories[typecome][i][0]))
			printf("%s(%d)\n", categories[typecome][i], i);
		else break;
	printf("==============================\n");
}

void print_resources(){
	char resources[RES_NUM][NAME_MAX]={"Cash", "Card", "Credit"};
	printf("==============================\n");
	for(uint8_t i=0; i<RES_NUM; ++i)
		printf("%s(%d)\n", resources[i], i);
	printf("==============================\n");
}


uint8_t prompt(answer_t* info){
//	char dbname[NAME_MAX];
//      char password[NAME_MAX];


	uint8_t flag;
	if((flag=read_config("dbpath", dbpath))==1){
		printf("Couldn't open file config, try again\n");
		system("mkdir /home/$USER/.config/mafin && touch /home/$USER/.config/mafin/config");	
		return 1;
	}
	else if(flag==10){
		printf("Please input a path to the database(/home/$USER/mafin/finances): ");	
		get_str(dbpath);
		if(write_config("dbpath", dbpath))
			return 1;
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
	get_digit(&(info->typecome));

//	if(info->typecome)
//		printf("INCOME\n");
//	else
//		printf("EXPENSE\n");
		
	print_categories(info->typecome);
	printf("Select one by specifying its number: ");

	get_digit(&(info->category));

	printf("Payload: ");
	get_float(&(info->payload));

	printf("Comment(optional): ");
	if(get_str(info->comment))
		memset(info->comment, 0, 54);

	print_resources();
	printf("Resource: ");

	get_digit(&(info->resource));

	return 0;
}


int main(int argc, char** argv)
{

	strcat(cfgPath, getenv("USER"));
	strcat(cfgPath, "/.config/mafin/config");

	strcat(dbpath, getenv("USER"));
	strcat(dbpath, "/mafin/finances");

	answer_t info;

	if(prompt(&info))
	{
		printf("Something gone wrong, try again!\n");
		exit(1);
	}
	//dbpath should be created if not
	if(storedb(&info, dbpath)){
		printf("Something gone wrong, try again!\n");
		exit(1);
	}
	return 0;

//	printf("%d\n", sizeof(user_t));
}

