#include "fin.h"
#include "stream.h"

void usage(char**argv);
int multiple_option(int argc, char**argv, info_t* info);
int single_option(int argc, char**argv);

void usage(char** argv)
{
	printf("Usage: %s [ flags ] [ expression ]\nSingle option flags:\n\t-i - interactive\n\t-s - show history\n\t-l <number> - last <number> entries\n\t-q [ [+][-] ] [ [date][typecome][category][resource][currency][payload] ] - sorts database ('+'-asc, '-'-desc)\n\t-h - displays this message\nMultiple options flags:\n\t-t <digit> - typecome (0-outcome, 1-income)\n\t-c <number> - specifies category\n\t-r <number> - specifies resource\n\t-p <float number> - specifies payload\n\t-y <number> - specifies currency\n\t-m <comment> - comment\n\t-i <number> [-t][-c][-r][-p][-y][-m] - inserts entry\n\t-g <number> [-t][-c][-r][-p][-y][-m] - changes entry\n\t-d <number> - deletes entry\n", *argv);
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
		c=getopt(argc, argv, "q:hisl:d:");
		switch(c)
		{
            case 'q':
                if(!strncmp(optarg+1,"date",4))
                    return sort_date(*optarg);
                else if(!strncmp(optarg+1,"payload",7))
                    return sort_payload(*optarg);
                else if(strlen(optarg+1)==8)
                    return sort_uint8(*optarg, optarg+1);

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
                char *token;
                token = strtok(optarg, ",");
                //fin -d 12,13,14 - deletes 12,14,16
                while(token!=NULL)
                {
                    db_rm_entry_by_num(dbpath, (uint32_t)atoi(token));    
                    token = strtok(NULL, ",");
                }
                 
                return 0;
			case 's':
				return show_history();
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
	memset(info, 0, sizeof(*info));
	char c;
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
        if(db_change_entry_by_num(info, dbpath, chNum))
            return 1;
        return 0;
    }
    if(insNum)
    {
        if(db_ins_entry_by_num(info, dbpath, insNum))
            return 1;
        return 0;
    }
	return -1;
}
