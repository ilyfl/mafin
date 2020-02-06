#include "fin.h"
#include <getopt.h>

void usage(char**argv);
int fill(int argc, char**argv, answer_t* info);
int show(int argc, char**argv);

void usage(char** argv)
{
	printf("Usage: %s [ flags ] [ expression ]\nFlags:\n\t-i - interactive\n\t-s - show history\n\t-l <number> - last <number> entries\n\t-t <digit> - typecome (0-outcome, 1-income)\n\t-m <comment> - comment\n\t-r <digit> - specifies resource\n\t-c <digit> - specifies category\n\t-p <float number> - specifies payload\n\t-d <number> - deletes entry\n\t-h - displays this message  \n", *argv);
	printf("OUTCOME Categories:\n");
	print_categories(0);
	printf("INCOME Categories:\n");
	print_categories(1);
	printf("Resources:\n");
	print_resources();
}

int show(int argc, char**argv)
{
		char c;
		c=getopt(argc, argv, "hisl:d:");
		switch(c)
		{
			case 'i':
				prompt(&info);
				return -1;
			case 'l':
				print_last(atoi(optarg));
				return 0;	
            case 'd':
                return rmEntry_n(dbpath, atoi(optarg));
			case 's':
				//show_history();
				return 0;
			case 'h':case '?':default:
				usage(argv);
				return 1;
		}
}

int parse_opts(int argc, char** argv)
{
	if(argc<=3)
		return show(argc, argv);
	else if (argc >=9)
		return fill(argc, argv, &info);
	else{
		usage(argv);
		return 1;
	}
	
}

int fill(int argc, char** argv, answer_t* info){
	uint8_t typecome,category,resource;
	memset(info, 0, sizeof(*info));
	char c;
	while((c=getopt(argc, argv, "t:c:r:p:m:"))!=-1)
	{
		switch(c)
		{
			case 't':
				typecome=(uint8_t)atoi(optarg);	
				if(within(typecome,0,1)){
					typecome<<=TLSFT;
					info->tcr|=typecome;
				}
				break;
			case 'c':
				category=(uint8_t)atoi(optarg);
				if(within(category,0,CAT_MAX)){
					category<<=CLSFT;
					info->tcr|=category;
				}
				break;
			case 'r':
				resource=(uint8_t)atoi(optarg);
				if(within(resource,0,RES_MAX))
					info->tcr|=resource;
				break;
			case 'p':
				info->payload=atof(optarg);
				break;
			case 'm':
				strcpy(info->comment,optarg);
				break;
			case '?':default:
				usage(argv);
				return 1;
		}
	}
	return -1;
}
