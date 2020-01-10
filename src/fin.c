#include "fin.h"


//currently writes on the first line only

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
	uint8_t	mode;
	printf("MODES:\nInput info(0)/Show totals(1)/History(2)?: ");
	get_digit(&mode);
	if(mode)
	{
		printf("Not ready!\n");
		return 1;
	}
	
	printf("Outcome(0)/Income(1)?: ");	
	get_digit(&(info->typecome));

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

void init_env()
{
	char* 	username[NAME_MAX];
	char 	command[PATH_MAX];
	uint8_t	flag;
	FILE* tmp;

	if(get_username(username))
	{
		printf("Username is not defined!\n");
		exit(1);
	}
	
//initializing globals cfgPath and dbpath
	strcpy(cfgPath,"/home/");	
	strcat(cfgPath, *username);
	strcat(cfgPath, "/.config/mafin/config");

	strcpy(dbpath,"/home/");	
	strcat(dbpath, *username);
	strcat(dbpath, "/mafin/finances");

//reading config file
	if((flag=read_config("dbpath", dbpath))==1){
		printf("Couldn't open config file\n");
		system("mkdir -p --mode=775 /home/$USER/.config/mafin && touch /home/$USER/.config/mafin/config");	
		exit(1);
	}
	else if(flag==10){
		printf("Please input a path to the database file(/home/$USER/mafin/finances): ");	
		get_str(dbpath);
		if(write_config("dbpath", dbpath))
			exit(1);
	}

	if((tmp=fopen(dbpath, "r"))==NULL)
	{
		flag=2;	
		printf("Database specified is not created!\n");
		exit(1);
	}
	else
		return;

//dbpath is read so create it
//bug! creates folder not a file
	if(flag){
		strcpy(command,"mkdir -p -m 775 ");
		strcat(command, dbpath);
		system(command);

		strcpy(command,"touch ");
		strcat(command, dbpath);
		system(command);
	}
	if((tmp=fopen(dbpath, "r"))==NULL)
	{
		printf("Database couldn't be opened!\n");
		exit(1);
	}
	fclose(tmp);
			
}

int main(int argc, char** argv)
{
	if(argc>1)
	{	
		printf("Wow you have specified an arguments, congrats!\n");
	}

	answer_t	info;

	init_env();

	if(prompt(&info))
	{
		printf("Something gone wrong, try again!\n");
		exit(1);
	}

	if(storedb(&info, dbpath))
	{
		exit(1);
	}
	
	return 0;
}

