#include "fin.h"
#include "stream.h"
#include "misc.h"
#include <signal.h>

//TODO:
//Sorting


char category[2][CAT_MAX][NAME_MAX]={{"Food", "Eating", "Entertainment", "Transport", "Bills", "Clothes", "Health", "Phone", "Toiletry", "Other"},{"Salary", "Wages", "Random"}};
char resource[RES_MAX][NAME_MAX]={"Cash", "Card", "Credit"};

uint8_t sort(FILE* dbfd, uint8_t type)
{
    return 0;
}

uint8_t show_history(){
    FILE* dbfd;
    answer_t info;
    size_t i=1;
    size_t pos=0;

    if((dbfd=fopen(dbpath, "r"))==NULL)
    {
		fprintf(stderr, "Error occured while opening a file\n");
		return 1;	
    }
    


    while(!readdb(dbfd, &info, &pos)){
		printf("%d. ", i++);
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

    return 0;
}

void print_categories(const uint8_t typecome){
	for(uint8_t i = 0; i < CAT_MAX; ++i)
		if(isalnum(category[typecome][i][0]))
			printf("\t%s(%d)\n", category[typecome][i], i);
		else break;
}

void print_resources(){
	for(uint8_t i=0; i<RES_MAX; ++i)
		if(isalnum(resource[i][0]))
			printf("\t%s(%d)\n", resource[i], i);
		else break;
}

//prints main structure 
void print_last(uint32_t n)
{
	answer_t info;
	FILE* dbfd;
	long pos=0;
	if((dbfd=fopen(dbpath, "r"))==NULL)
	{
		fprintf(stderr, "Error occured while opening a file\n");
		exit(1);
	}
	printf("Last %d entries:\n", n);
	for(uint32_t i=0; i < n; ++i)
	{
		if(readdb(dbfd, &info, &pos))
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
	fclose(dbfd);
	
}


uint8_t prompt(answer_t* info){
	uint8_t tmp;
	
	printf("Outcome(0)/Income(1)?: ");	
	get_digit(stdin,&tmp,0,1);

	info->tcr|=tmp<<TLSFT;

	print_categories(tmp);
	printf("Select one by specifying its number: ");
	if(tmp)
		get_digit(stdin,&tmp,0,2);
	else
		get_digit(stdin,&tmp,0,9);
	info->tcr |= tmp<<CLSFT;

	printf("Payload: ");
	get_float(stdin,&info->payload);

	printf("Comment(optional): ");
	if(get_str(stdin,info->comment))
		memset(info->comment, 0, sizeof(info->comment));

	print_resources();
	printf("Resource: ");
	get_digit(stdin,&tmp,0,2);
	info->tcr |=tmp;

	return 0;
}

void init_env()
{
	char* 	username[NAME_MAX];
	char 	command[PATH_MAX];
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

	//strcpy(dbpath,"/home/");	
	//strcat(dbpath, *username);
	//strcat(dbpath, "/mafin/finances");

//reading config file
	if((tmp=fopen(cfgPath, "r"))==NULL)	
	{
		fprintf(stderr,"Couldn't open config file\n");
		exit(1);
	}
    
    char temp1[RES_MAX*NAME_MAX];   
    char temp2[CAT_MAX*NAME_MAX]; 
    size_t pos=0; 
	while(read_config(tmp, "dbpath", dbpath, &pos));
    pos=0;
	while(read_config(tmp, "resources", temp1,&pos));
    pos=0;
	while(read_config(tmp, "categories", temp2,&pos));
    pos=0;
	fclose(tmp);
    printf("res: %s\ncat:%s\n", temp1,temp2);
	if((tmp=fopen(dbpath, "r"))==NULL)
	{
		fprintf(stderr,"Database specified is not created!\n");
		exit(1);
	}

	fclose(tmp);
	return;

//dbpath is read so create it
//bug! creates folder not a file
	strcpy(command,"mkdir -p -m 775 ");
	strcat(command, dbpath);
	system(command);

	strcpy(command,"touch ");
	strcat(command, dbpath);
	system(command);
	if((tmp=fopen(dbpath, "r"))==NULL)
	{
		fprintf(stderr,"Database couldn't be opened!\n");
		exit(1);
	}
	fclose(tmp);
			
}

void finish()
{
	printf("\n");
	exit(0);
}

int main(int argc, char** argv)
{
	int8_t flag=0;

	(void) signal(SIGINT,finish);

    clock_t t;
    t=clock();

	init_env();
	flag=parse_opts(argc,argv);
	if(flag>0)
		return 1;
	else if(flag<0)
    {
        time_t t = time(NULL);
        info.time = *localtime(&t);
		if(storedb(&info, dbpath))
		{
			return 1;
		}
    }

    t=clock()-t;
    printf("%lli\n", (long long)t);

	return 0;
}

