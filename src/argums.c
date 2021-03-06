#include "fin.h"
#include "stream.h"

void usage(char**argv);
int multiple_option(int argc, char**argv, info_t* info);
int single_option(int argc, char**argv);

void usage(char** argv)
{
	printf("Usage: %s [ flags ] [ expression ]\n", *argv);
    printf("Single option flags:\n");
    printf("\t-i - interactive\n");
    printf("\t-s [l]-(by lines) [o]-(overall) [m]-(monthly) [tcry] - show history\n");
    printf("\t-l <number> - last <number> entries\n");
    printf("\t-q [ [+][-] ] [ [date][typecome][category][resource][currency][payload] ] - sorts database ('+'-asc, '-'-desc)\n");
    printf("\t-h - displays this message\n");
    printf("Multiple options flags:\n");
    printf("\t-t <digit> - typecome (0-outcome, 1-income)\n");
    printf("\t-c <number> - specifies category\n");
    printf("\t-r <number> - specifies resource\n");
    printf("\t-p <float number> - specifies payload\n");
    printf("\t-y <number> - specifies currency\n");
    printf("\t-m <comment> - comment\n");
    printf("\t-i <number> [-t][-c][-r][-p][-y][-m] - inserts entry\n");
    printf("\t-g <number> [-t][-c][-r][-p][-y][-m] - changes entry\n");
    printf("\t-d <[l]-(last) [a]-(all)> or <number> - deletes entry\n");

	printf("EXPENSE Categories:\n");
    print_categories(0);

	printf("INCOME Categories:\n");
    print_categories(1);

	printf("Resources:\n");
	print_resources();
	printf("Currencies:\n");
	print_currencies();
}

int single_option(int argc, char**argv)
{
		char c;
        char *token;
		c=getopt(argc, argv, "q:his:l:d:");
		switch(c)
		{
            case 'q':
                if(!strncmp(optarg+1,"date",4))
                    return sort_date(*optarg);

                else if(!strncmp(optarg+1,"payload",7))
                    return sort_payload(*optarg);

                else if(strlen(optarg+1)==8)
                    return sort_uint8(*optarg, optarg+1);
                return 1;

			case 'i':
				prompt(&info);
				return -1;

			case 'l':
				print_last(atoi(optarg));
				return 0;	

            case 'd':
                if(*optarg=='a')
                {
                    if(!db_rm_entry_by_num(dbpath, -1))
                        return 0;
                    else return 1;
                }

                else if(*optarg=='l')
                {

                    uint32_t NUM = 0;
                    FILE *dbfd;

                    if((dbfd=fopen(dbpath, "r"))==NULL)
                    {
                        fprintf(stderr, "Error occured while opening a file\n");
                        return 1;
                    }

                    while((c=fgetc(dbfd))!=EOF)
                        if(c == '\n')
                            NUM++;

                    if(!db_rm_entry_by_num(dbpath, NUM))
                        return 0;
                    else return 1;
                }

                //fin -d 12,13,14 - deletes 12,14,16
                token = strtok(optarg, ",");
                while(token!=NULL)
                {
                    db_rm_entry_by_num(dbpath, (uint32_t)atoi(token));    
                    token = strtok(NULL, ",");
                }
                 
                return 0;
			case 's':
                {
                    uint8_t flags = 0;
                    while(isalnum(*optarg))
                        switch(*optarg++)
                        {
                            case 'm':
                                flags |= HISTORY_BY_MONTH;
                                break;

                            case 'o':
                                flags |= HISTORY_OVERALL;
                                break;

                            case 't':
                                flags |= HISTORY_BY_TYPECOMES;
                                break;

                            case 'c':
                                flags |= category_len ? HISTORY_BY_CATEGORIES : 0;
                                break;

                            case 'r':
                                flags |= resource_len ? HISTORY_BY_RESOURCES : 0;
                                break;

                            case 'w':
                                flags |= HISTORY_BY_WEEK;
                                break;

                            case 'y':
                                flags |= currency_len ? HISTORY_BY_CURRENCIES : 0;
                                break;

                            case 'l':
                                flags |= HISTORY_BY_ROWS;
                                break;
                        }
				return show_history(flags);
                }

			case 'h':case '?':default:
				usage(argv);
				return 1;
		}
}

int parse_options(int argc, char** argv)
{
	if(argc<=3)
		return single_option(argc, argv);

	else if (argc >=9)
		return multiple_option(argc, argv, &info);

	else{
		usage(argv);
		return 1;
	}
	
}

int multiple_option(int argc, char** argv, info_t* info){
    size_t insNum=0;
    size_t chNum=0;
	char c;

	memset(info, 0, sizeof(*info));
	while((c=getopt(argc, argv, "i:t:c:r:y:p:m:g:"))!=-1)
	{
		switch(c)
		{

			case 't':

				if(within(atoi(optarg),0,1))
				    info->typecome=(uint8_t)atoi(optarg);	
                
                else
                    usage(argv);
				break;

			case 'c':

				if(within(atoi(optarg),0,CAT_MAX))
				    info->category=(uint8_t)atoi(optarg);	

                else
                    usage(argv);
				break;

			case 'r':
				if(within(atoi(optarg),0,CAT_MAX))
				    info->resource=(uint8_t)atoi(optarg);	

                else
                    usage(argv);
				break;

			case 'y':
				if(within(atoi(optarg),0,CAT_MAX))
				    info->currency=(uint8_t)atoi(optarg);	

                else
                    usage(argv);
				break;

			case 'p':
				info->payload=atof(optarg);
				break;

			case 'm':
                info->comment=malloc(strlen(optarg));
				strcpy(info->comment,optarg);
                *(info->comment+strlen(optarg))='\0';
				break;

			case 'i':
                insNum=atoi(optarg);
				break;

			case 'g':
                chNum=atoi(optarg);
				break;

			case '?':default:
				usage(argv);
				return 1;
		}
	}

    if(chNum)
    {
        //if(db_change_entry_by_num(info, dbpath, chNum))
            return 1;
        //return 0;
    }

    if(insNum)
    {
        if(db_ins_entry_by_num(info, dbpath, insNum))
            return 1;
        return 0;
    }

	return -1;
}
