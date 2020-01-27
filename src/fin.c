#include "fin.h"
#include "stream.h"
#include "misc.h"
#include <signal.h>
#include <getopt.h>

#define within(A, B, C) ((A>=B) && (A<=C))
//TODO:
//Sorting

char category[2][CAT_MAX][NAME_MAX]={{"Food", "Eating", "Entertainment", "Transport", "Bills", "Clothes", "Health", "Phone", "Toiletry", "Other"},{"Salary", "Wages", "Random"}};
char resource[RES_NUM][NAME_MAX]={"Cash", "Card", "Credit"};

void print_categories(const uint8_t typecome){
	printf("==============================\n");
	for(uint8_t i = 0; i < CAT_MAX; ++i)
		if(isalnum(category[typecome][i][0]))
			printf("%s(%d)\n", category[typecome][i], i);
		else break;
	printf("==============================\n");
}

void print_resources(){
	printf("==============================\n");
	for(uint8_t i=0; i<RES_NUM; ++i)
		printf("%s(%d)\n", resource[i], i);
	printf("==============================\n");
}

//prints main structure 
void print_last(uint32_t n)
{
	answer_t info;
	printf("Last %d entries:\n", n);
	for(uint32_t i=0; i < n; ++i)
	{
		if(readdb(dbpath, &info))
			break;
		printf("%d. ", i+1);
		printf("%02d/%02d/%d|%02d:%02d:%02d| ",info.time.tm_mday, info.time.tm_mon+1, info.time.tm_year+1900, info.time.tm_hour, info.time.tm_min, info.time.tm_sec);	

		if(info.tcr>=0)
			printf("Expense on ");
		else
			printf("Income by ");
		printf("%s ",category[(uint8_t)info.tcr>>7][(info.tcr&120)>>3]); 		
		printf("%.2f, res: ", info.payload);	
		printf("%s",resource[info.tcr&7]);
		if(info.comment[0]!='\0')
		{
			printf(", Comment: %s\n", info.comment);
		}
		else 
			printf("\n");
	}
	
}


uint8_t prompt(answer_t* info){
	uint8_t tmp;
	
	printf("Outcome(0)/Income(1)?: ");	
	get_digit(&tmp,0,1);
	info->tcr=tmp;

	print_categories(info->tcr);
	printf("Select one by specifying its number: ");
	if(tmp)
		get_digit(&tmp,0,2);
	else
		get_digit(&tmp,0,9);
	info->tcr <<= 4;
	info->tcr |= tmp;

	printf("Payload: ");
	get_float(&(info->payload));

	printf("Comment(optional): ");
	if(get_str(info->comment))
		memset(info->comment, 0, 54);

	print_resources();
	printf("Resource: ");
	get_digit(&tmp,0,2);
	info->tcr <<=3;
	info->tcr |=tmp;

	if(storedb(info, dbpath))
	{
		return 1;
	}
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
		fprintf(stderr,"Username is not defined!\n");
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
		fprintf(stderr,"Couldn't open config file\n");
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
		fprintf(stderr,"Database specified is not created!\n");
		exit(1);
	}
	else
	{
		fclose(tmp);
		return;
	}
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
		fprintf(stderr,"Database couldn't be opened!\n");
		exit(1);
	}
	fclose(tmp);
			
}

void test(answer_t info)
{
	//printf("%d", sizeof(answer_t));
	while(!readdb(dbpath, &info))
	{
		printf("%02d-%02d-%d/%02d:%02d:%02d|",info.time.tm_mday, info.time.tm_mon+1, info.time.tm_year+1900, info.time.tm_hour, info.time.tm_min, info.time.tm_sec);	
		printf("%d,%f,%s\n", info.tcr, info.payload, info.comment);
	}
}

void finish()
{
	printf("\n");
	exit(0);
}

int main(int argc, char** argv)
{
	(void) signal(SIGINT,finish);

	answer_t info;
	uint8_t	mode=0, typecome, category,resource;
	char c;
	float payload;
	char* comment;
	init_env();

	if(argc >= 2 && argc <= 3)
	{
		uint32_t number=10;
		c=getopt(argc, argv, "sl:");
		if(c=='l')
		{
			number=(uint32_t)atoi(optarg);
			print_last(number);
			exit(0);
		}
		else if(c=='s')
		{
			//show_history();
			exit(0);
		}
	}

	if( argc>=8 )
	{	
		uint8_t flag=0;
		while((c=getopt(argc, argv, "hxic:m:r:p:s"))!=-1)
		{
			switch(c)
			{
				case 'i':
					if(!flag)
					{
						typecome=1;
						flag=1;
					}
					else
					{
						printf("Error! Income and expense are specified at the same time!\n");
						exit(1);
					}
					break;
				case 'x':
					if(!flag)
					{
						typecome=0;
						flag=2;
					}
					else
					{
						printf("Error! Income and expense are specified at the same time!\n");
						exit(1);
					}
					break;
				case 'c':
					category=(uint8_t)atoi(optarg);
					if(flag==1 && !within(category,0,2))
					{
						printf("Error! Category for income have to be a digit within (0,1,2)!\n");
						exit(1);
					}
					else if(!within(category,0,9))
					{
						printf("Error! Category for expense have to be a digit within (0-9)!\n");
						exit(1);
					}

					break;
				case 'm':
					mode=1;
					comment=optarg;
					break;
				case 'r':
					resource=(uint8_t)atoi(optarg);
					if(!within(resource, 0,2))
					{
						printf("Error! Resource have to be a digit within (0,1,2)!\n");
						exit(1);
					}

					break;
				case 'p':
					payload=atof(optarg);
					break;
				case 'h':
				case '?':
					printf("Usage: fin [-s -l <number> ] [-i -x -c <digit> -m <comment> -r <digit> -p <float number>]\nFlags:\n\t-s - show history\n\t-l <number> - last <number> entries\n\t-i - sets income flag\n\t-x - sets expense flag\n\t-m <comment> - action comment\n\t-r <digit> - specifies resource\n\t-c <digit> - specifies category\n\t-p <float number> - specifies payload  \n");
				default:
					exit(1);

			}
		}
		if(category==9 && !mode)
		{
			printf("Comment for category \"Other\" is required\n");
			exit(1);
		}
		if(mode)
			strcpy(info.comment, comment);
		else
			memset(info.comment, 0, 64);
		info.payload = payload;
		info.tcr = typecome;
		info.tcr <<=4;
		info.tcr |=category;
		info.tcr <<=3;
		info.tcr |=resource;
		storedb(&info, dbpath);
		exit(0);
	}
	if(argc>1){
		printf("Usage: fin [-s -l <number> ] [-i -x -c <digit> -m <comment> -r <digit> -p <float number>]\nFlags:\n\t-s - show history\n\t-l <number> - last <number> entries\n\t-i - sets income flag\n\t-x - sets expense flag\n\t-m <comment> - action comment\n\t-r <digit> - specifies resource\n\t-c <digit> - specifies category\n\t-p <float number> - specifies payload  \n");
		exit(1);
	}


//	test(info);
	printf("MODES:\nInput info(0)/Show totals(1)/History(2)?: ");
	get_digit(&mode,0,2);
	if(mode==1)
	{
		fprintf(stderr,"Not ready!\n");
		return 1;
	}
	else if(mode==2)
		print_last(10);
	else
	{
		if(prompt(&info))
		{
			fprintf(stderr,"Something gone wrong, try again!\n");
			exit(1);
		}
	}



	return 0;
}

