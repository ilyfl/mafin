#include "fin.h"
#include "stream.h"
#include "misc.h"


void print_info(info_t* info)
{
		printf("%02d/%02d/%d|%02d:%02d:%02d| ",info->time.tm_mday, info->time.tm_mon+1, info->time.tm_year+1900, info->time.tm_hour, info->time.tm_min, info->time.tm_sec);	

		if(!info->typecome)
        {
			printf("Expense on ");
        }
		else
        {
			printf("Income by ");
        }
		printf("%s ",category[info->typecome][info->category]); 		
		printf("%.2f", info->payload);	
		printf(" %s, ",currency[info->currency]);
		printf("res: %s",resource[info->resource]);
		if(info->comment!=NULL)
		{
			printf(", Comment: %s\n", info->comment);
            free(info->comment);
		}
		else 
			printf("\n");


}

int floats_compare_asc(const void* a, const void* b)
{
    float t =(*(float*)a - *(float*) b); 
    if(t > 0)
        return 1;
    else 
        return 0;
}

int floats_compare_desc(const void* a, const void* b)
{
    float t =(*(float*)b - *(float*) a); 
    if(t > 0)
        return 1;
    else 
        return 0;
}

int ints_compare_asc(const void* a, const void* b)
{
    int t =(*(int*)a - *(int*) b); 
    if(t > 0)
        return 1;
    else 
        return 0;
}

int ints_compare_desc(const void* a, const void* b)
{
    int t =(*(int*)b - *(int*) a); 
    if(t > 0)
        return 1;
    else 
        return 0;
}

int uint8_compare_asc(const void* a, const void* b)
{
    int t =(*(uint8_t *)a - *(uint8_t*) b); 
    if(t > 0)
        return 1;
    else 
        return 0;
}
int uint8_compare_desc(const void* a, const void* b)
{
    int t =(*(uint8_t *)b - *(uint8_t*) a); 
    if(t > 0)
        return 1;
    else 
        return 0;
}



void swap(info_t* A, info_t* B)
{
    info_t temp = *A;
    *A = *B;
    *B = temp;
}

void merge_sort(uint32_t count, info_t* base, void *elem, info_t* temp, int (*cmp)(const void *, const void *))
{
    uint32_t offset = elem - (void*)base;

    if(count == 1);

    else if(count==2)
    {
        info_t *EntryA = base;
        info_t *EntryB = base + 1;
        void* sortKey0 = (void*)EntryA;
        sortKey0 += offset;
        void* sortKey1 = (void*)EntryB;
        sortKey1 += offset;

        if(cmp(sortKey0, sortKey1))
            swap(EntryA, EntryB);
    }
    else 
    {
        uint32_t half0 = count/2;
        uint32_t half1 = count-half0; 
        info_t *inHalf0 = base;
        info_t *inHalf1 = base + half0;
        void* sortKey0 = (void*)inHalf0;
        sortKey0 += offset;
        void* sortKey1 = (void*)inHalf1;
        sortKey1 += offset;

        merge_sort(half0, inHalf0, sortKey0, temp, cmp);
        merge_sort(half1, inHalf1, sortKey1, temp, cmp);

        info_t *EntryA = inHalf0;
        info_t *EntryB = inHalf1;
        info_t *end = base + count;
        for(uint32_t i = 0; i < count; i++)
        {
            void* sortKey0 = (void*)EntryA;
            sortKey0 += offset;
            void* sortKey1 = (void*)EntryB;
            sortKey1 += offset;

            if(EntryA == inHalf1) {
                temp[i] = *EntryB++;
            }
            else if(EntryB == end) {
                temp[i] = *EntryA++;
            }
            else if(!cmp(sortKey0, sortKey1)) {
                temp[i] = *EntryA++;
            }
            else {
                temp[i] = *EntryB++;
            }

        }
        for(uint32_t i = 0; i < count; i++)
            base[i]=temp[i];
    }

}

void radix_sort(uint32_t count, info_t* base, void* elem)
{
    info_t *source = base;
    uint8_t offset = elem - (void*)base;
    info_t *temp = malloc(count * sizeof *temp);
    info_t *dest = temp;

    for(uint32_t byteIndex = 0; byteIndex < 8; byteIndex+=4)
    {
        uint32_t sortKeyOffset[16]={0};
        for(uint32_t i = 0; i < count; i++)
        {
            uint8_t* radixNum = (uint8_t*)&source[i];
            radixNum += offset;
            uint8_t radixPiece = (*radixNum >> byteIndex) & 0x0F;
            ++sortKeyOffset[radixPiece];
        }
        uint32_t total = 0;
        for(uint8_t index = 0; index < 16; index++)
        {
            uint32_t num = sortKeyOffset[index];
            sortKeyOffset[index] = total;
            total += num;
        }
        for(uint32_t i = 0; i < count; i++)
        {
            uint8_t* radixNum = (uint8_t*)&source[i];
            radixNum += offset;
            uint8_t radixPiece = (*radixNum >> byteIndex) & 0x0F;
            dest[sortKeyOffset[radixPiece]++]=source[i];
        }
        info_t *swap_temp = dest;
        dest=source;
        source = swap_temp;
    }
    free(temp);
}

uint8_t sort_date(uint8_t order)
{
    FILE* dbfd;
    int NUM=0;
    uint64_t pos=0;
    char c='a';
    uint32_t i = 0;

    if(order != '+' && order != '-')
        return 1;

    if((dbfd=fopen(dbpath, "r"))==NULL)
    {
		fprintf(stderr, "Error occured while opening a file\n");
        return 1;
    }
    while((c=fgetc(dbfd))!=EOF)
        if(c == '\n')
            NUM++;

    info_t info[NUM];

    fseek(dbfd, 0 , SEEK_SET);
    if(!db_read_info(dbfd, &info[i++], &pos))
        while(!db_read_info(dbfd, &info[i++], &pos));
    else{
        fclose(dbfd);
        return 1;
    }

    info_t temp[NUM]; 

    if(order=='+') {
        merge_sort(NUM, info, &info->time.tm_sec, temp,ints_compare_asc);
        merge_sort(NUM, info, &info->time.tm_min, temp,ints_compare_asc);
        merge_sort(NUM, info, &info->time.tm_hour,temp,ints_compare_asc);
        merge_sort(NUM, info, &info->time.tm_mday,temp,ints_compare_asc);
        merge_sort(NUM, info, &info->time.tm_mon, temp,ints_compare_asc);
        merge_sort(NUM, info, &info->time.tm_year,temp,ints_compare_asc);
    }

    else 
    {
        merge_sort(NUM, info, &info->time.tm_sec, temp,ints_compare_desc);
        merge_sort(NUM, info, &info->time.tm_min, temp,ints_compare_desc);
        merge_sort(NUM, info, &info->time.tm_hour,temp,ints_compare_desc);
        merge_sort(NUM, info, &info->time.tm_mday,temp,ints_compare_desc);
        merge_sort(NUM, info, &info->time.tm_mon, temp,ints_compare_desc);
        merge_sort(NUM, info, &info->time.tm_year,temp,ints_compare_desc);
    }

    db_rm_entry_by_num(dbpath,-1);
    for(int i = 0; i < NUM; i++)
    {
        db_store_info(&info[i], dbpath);
    }

    fclose(dbfd);
    return 0;
}

uint8_t sort_uint8(uint8_t order, char* elem)
{
    FILE* dbfd;
    uint32_t NUM=0;
    uint64_t pos=0;
    char c='a';
    uint32_t i = 0;

    if(order != '+' && order != '-')
        return 1;

    if((dbfd=fopen(dbpath, "r"))==NULL)
    {
		fprintf(stderr, "Error occured while opening a file\n");
        return 1;
    }
    while((c=fgetc(dbfd))!=EOF)
        if(c == '\n')
            NUM++;

    info_t info[NUM];

    fseek(dbfd, 0 , SEEK_SET);
    while(!db_read_info(dbfd, &info[i++], &pos));

    info_t temp[NUM];

    if(!strncmp(elem, "category", 8))
    {
        if(order=='+')
            radix_sort(NUM, info, &info->category);
        else 
            merge_sort(NUM, info, &info->category, temp, uint8_compare_desc);
    }

    else if(!strncmp(elem, "resource", 8))
    {
        if(order=='+')
            radix_sort(NUM, info, &info->resource);
        else 
            merge_sort(NUM, info, &info->resource, temp, uint8_compare_desc);
    }

    else if(!strncmp(elem, "currency", 8))
    {
        if(order=='+')
            radix_sort(NUM, info, &info->currency);
        else
            merge_sort(NUM, info, &info->currency, temp, uint8_compare_desc);
    }

    else if(!strncmp(elem, "typecome", 8))
    {
        if(order=='+')
            radix_sort(NUM, info, &info->typecome);
        else
            merge_sort(NUM, info, &info->typecome, temp, uint8_compare_desc);
    }


    db_rm_entry_by_num(dbpath,-1);
    for(i = 0; i < NUM; i++)
    {
        db_store_info(&info[i], dbpath);
    }

    fclose(dbfd);
    return 0;
}

uint8_t sort_payload(uint8_t order)
{
    FILE* dbfd;
    uint32_t NUM=0;
    uint64_t pos=0;
    char c='a';
    uint32_t i = 0;
    if(order != '+' && order != '-')
        return 1;

    if((dbfd=fopen(dbpath, "r"))==NULL)
    {
		fprintf(stderr, "Error occured while opening a file\n");
        return 1;
    }
    while((c=fgetc(dbfd))!=EOF)
        if(c == '\n')
            NUM++;

    info_t info[NUM];

    fseek(dbfd, 0 , SEEK_SET);
    if(!db_read_info(dbfd, &info[i++], &pos))
        while(!db_read_info(dbfd, &info[i++], &pos));
    else{
        fclose(dbfd);
        return 1;
    }

    info_t temp[NUM];

    if(order=='+')
        merge_sort(NUM, info, &info->payload, temp, floats_compare_asc);
    else 
        merge_sort(NUM, info, &info->payload, temp, floats_compare_desc);

    db_rm_entry_by_num(dbpath,-1);
    for(i = 0; i < NUM; i++)
    {
        db_store_info(&info[i], dbpath);
    }

    fclose(dbfd);
    return 0;
}

uint8_t show_history(uint8_t flags){
    FILE* db;
    uint64_t pos=0;
    uint32_t NUM=0;
    char c = 'a';
    time_t t = time(NULL);
    struct tm current = *localtime(&t);
    struct tm oldest=current;

    typedef struct count
    {
        double inc;
        double exp;

        double expense_category[expense_category_len];
        double income_category[income_category_len];

        double income_currency[currency_len];
        double expense_currency[currency_len];

        double income_resource[resource_len];
        double expense_resource[resource_len];
    }count_t;

    if((db=fopen(dbpath, "r"))==NULL)
    {
		fprintf(stderr, "Error occured while opening a file\n");
		return 1;	
    }

    while((c=fgetc(db)) != EOF)
    {
        if(c=='\n')
            NUM++;
    }
    
    info_t info[NUM];

    fseek(db, 0 , SEEK_SET);

    uint32_t i=0;
    if(!db_read_info(db, &info[i++], &pos))
    {
        while((!db_read_info(db, &info[i++], &pos)))
        oldest = info[0].time;
    }

    else 
    {
        printf("Database is empty\n");
        fclose(db);
        return 1;
    }



    for(i = 0; i < NUM; i++)
    {
        if(info[i].time.tm_year < oldest.tm_year) 
            oldest=info[i].time;
        else if(info[i].time.tm_year == oldest.tm_year)
        {
            if(info[i].time.tm_mon < oldest.tm_mon) 
                oldest=info[i].time;
            else if(info[i].time.tm_mon == oldest.tm_mon)
            {
                if(info[i].time.tm_mday < oldest.tm_mday)
                    oldest=info[i].time;
            }
        }
    }

    uint32_t nbuckets = 12 * (current.tm_year - oldest.tm_year) + (current.tm_mon - oldest.tm_mon) + 1;

    count_t ov;
    count_t mon[nbuckets];

    //data initialization
    ov.inc = 0;
    ov.exp = 0;

    for(i = 0; i < expense_category_len; i++)
        ov.expense_category[i] = 0;

    for(i = 0; i < income_category_len; i++)
        ov.income_category[i] = 0;

    for(i = 0; i < currency_len; i++)
    {
            ov.income_currency[i] = 0;
            ov.expense_currency[i] = 0;
    }

    for(i = 0; i < currency_len; i++)
    {
            ov.income_resource[i] = 0;
            ov.expense_resource[i] = 0;
    }

    for(i = 0; i < nbuckets; i++)
    {
        mon[i].inc=0;
        mon[i].exp=0;

        for(uint32_t j = 0; j < expense_category_len; j++)
            mon[i].expense_category[j] = 0;

        for(uint32_t j = 0; j < income_category_len; j++)
            mon[i].income_category[j] = 0;

        for(uint32_t j = 0; j < currency_len; j++)
        {
            mon[i].income_currency[j] = 0;
            mon[i].expense_currency[j] = 0;
        }

        for(uint32_t j = 0; j < resource_len; j++)
        {
            mon[i].income_resource[j] = 0;
            mon[i].expense_resource[j] = 0;
        }
    }
    
    //counting
    for(i = 0; i < NUM; i++)
    {
        uint32_t bucket = 12 * (info[i].time.tm_year - oldest.tm_year) + (info[i].time.tm_mon - oldest.tm_mon);

        if(info[i].typecome)
        {
            mon[bucket].inc += info[i].payload;
            mon[bucket].income_category[info[i].category] +=  info[i].payload;
            mon[bucket].income_currency[info[i].currency] +=  info[i].payload;
            mon[bucket].income_resource[info[i].resource] +=  info[i].payload;
            ov.inc += to_default_currency(info[i].payload);
            ov.income_category[info[i].category] +=  info[i].payload;
            ov.income_currency[info[i].currency] +=  info[i].payload;
            ov.income_resource[info[i].resource] +=  info[i].payload;
        }

        else
        {
            mon[bucket].exp += info[i].payload;
            mon[bucket].expense_category[info[i].category] +=  info[i].payload;
            mon[bucket].expense_currency[info[i].currency] +=  info[i].payload;
            mon[bucket].expense_resource[info[i].resource] +=  info[i].payload;
            ov.exp += to_default_currency(info[i].payload);
            ov.expense_category[info[i].category] +=  info[i].payload;
            ov.expense_currency[info[i].currency] +=  info[i].payload;
            ov.expense_resource[info[i].resource] +=  info[i].payload;
        }
    }

    //printing
    if(flags & HISTORY_BY_ROWS)
    {
        for(i = 0; i < NUM; i++)
        {
            printf("%d. ", i+1);
            print_info(&info[i]);
        }
    }

    if(flags & HISTORY_BY_MONTH)
    {
        uint8_t month = oldest.tm_mon + 1;
        uint64_t year = oldest.tm_year + 1900;
        for(i = 0; i < nbuckets; i++)
        {
            printf("01.%02d.%ld:\n", month, year);

            if(flags & HISTORY_BY_CATEGORIES)
            {
                printf("Categories:\n------\n");    
                
                printf("|Incomes by\n");    
                for(uint32_t j = 0; j < income_category_len; j++)
                {
                    printf("\t%s: ", category[1][j]);    
                    printf("%.02f\n", mon[i].income_category[j]);    
                }

                printf("|Expenses on\n");    
                for(uint32_t j = 0; j < expense_category_len; j++)
                {
                    printf("\t%s: ", category[0][j]);    
                    printf("%.02f\n", mon[i].expense_category[j]);    
                }

                printf("------\n");
            }

            if(flags & HISTORY_BY_RESOURCES)
            {

                printf("Resources:\n------\n");    
                
                printf("|Incomes via\n");    
                for(uint32_t j = 0; j < currency_len; j++)
                {
                    printf("\t%s: ", resource[j]);    
                    printf("%.02f\n", mon[i].income_resource[j]);    
                }

                printf("|Expenses via\n");    
                for(uint32_t j = 0; j < currency_len; j++)
                {
                    printf("\t%s: ", resource[j]);    
                    printf("%.02f\n", mon[i].expense_resource[j]);    
                }
                printf("------\n");
            }
            
            if(flags & HISTORY_BY_CURRENCIES)
            {

                printf("Currencies:\n------\n");    
                
                printf("|Incomes in\n");    
                for(uint32_t j = 0; j < currency_len; j++)
                {
                    printf("\t%s: ", currency[j]);    
                    printf("%.02f\n", mon[i].income_currency[j]);    
                }

                printf("|Expenses in\n");    
                for(uint32_t j = 0; j < currency_len; j++)
                {
                    printf("\t%s: ", currency[j]);    
                    printf("%.02f\n", mon[i].expense_currency[j]);    
                }
                printf("------\n");
            }

            if(flags & HISTORY_BY_TYPECOMES)
            {
                printf("Typecomes in (%s):\n------\n", currency[0]);    
                printf("|Incomes: %.02f\n|Expenses: %.02f\n------\n", mon[i].inc, mon[i].exp);
            }


            if(month==12)
            {
                month=1;
                year++;
            }
            else 
                month++;
        }

    }
    
    if(flags & HISTORY_OVERALL)
    {
        printf("------\nOverall:\n");

        if(flags & HISTORY_BY_CATEGORIES)
        {
            printf("Categories:\n------\n");    

            printf("|Incomes by\n");    
            for(i = 0; i < income_category_len; i++)
            {
                printf("\t%s: ", category[1][i]);    
                printf("%.02f\n", ov.income_category[i]);    
            }

            printf("|Expenses on\n");    
            for(i = 0; i < expense_category_len; i++)
            {
                printf("\t%s: ", category[0][i]);    
                printf("%.02f\n", ov.expense_category[i]);    
            }

            printf("------\n");
        }

        if(flags & HISTORY_BY_RESOURCES)
        {

            printf("Resources:\n------\n");    
            
            printf("|Incomes via\n");    
            for(uint32_t j = 0; j < currency_len; j++)
            {
                printf("\t%s: ", resource[j]);    
                printf("%.02f\n", ov.income_resource[j]);    
            }

            printf("|Expenses via\n");    
            for(uint32_t j = 0; j < currency_len; j++)
            {
                printf("\t%s: ", resource[j]);    
                printf("%.02f\n", ov.expense_resource[j]);    
            }
            printf("------\n");
        }

        if(flags & HISTORY_BY_CURRENCIES)
        {

            printf("Currencies:\n------\n");    
            
            printf("|Incomes in\n");    
            for(uint32_t j = 0; j < currency_len; j++)
            {
                printf("\t%s: ", currency[j]);    
                printf("%.02f\n", ov.income_currency[j]);    
            }

            printf("|Expenses in\n");    
            for(uint32_t j = 0; j < currency_len; j++)
            {
                printf("\t%s: ", currency[j]);    
                printf("%.02f\n", ov.expense_currency[j]);    
            }
            printf("------\n");
        }

        if(flags & HISTORY_BY_TYPECOMES)
        {
            printf("Typecomes in (%s):\n------\n", currency[0]);    
            printf("|Incomes: %.2f\n|Expenses: %.2f\n", ov.inc, ov.exp);
        }
    }

    fclose(db);
    return 0;
}

void print_categories(uint8_t typecome){
    int count = typecome ? income_category_len : expense_category_len ;
	for(uint8_t i = 0; i < count; ++i)
			printf("\t%s(%d)\n", category[typecome][i], i);
}

void print_resources(){
	for(uint8_t i=0; i < resource_len; ++i)
			printf("\t%s(%d)\n", resource[i], i);
}
void print_currencies(){
	for(uint8_t i=0; i < currency_len; ++i)
			printf("\t%s(%d)\n", currency[i], i);
}

//prints main structure 
void print_last(uint32_t n) {
	info_t info;
	FILE* dbfd;
	uint64_t pos=0;
	if((dbfd=fopen(dbpath, "r"))==NULL)
	{
		fprintf(stderr, "Error occured while opening a file\n");
		exit(1);
	}
	printf("Last %d entries:\n", n);
	for(uint32_t i=0; i < n; ++i)
	{
		if(db_read_info(dbfd, &info, &pos))
			break;
		printf("%d. ", i+1);
        print_info(&info);
	}
	fclose(dbfd);
	
}
//uint8_t prompt(){
//    uint8_t typecome;
//    int category;
//    float payload;
//    char comment[64];
//    int resource;
//    char command[INP_MAX];
//	printf("Outcome(0)/Income(1)?: ");	
//	typecome = get_digit(stdin,0,1);
//
//	print_categories(tmp);
//	printf("Select one by specifying its number: ");
//    category = get_number(stdin, 0, CAT_MAX);
//
//	printf("Payload: ");
//    payload = get_float(stdin);
//     
//	printf("Comment(optional): ");
//	if(get_str(stdin,comment))
//		memset(comment, 0, sizeof(comment));
//
//	print_resources();
//	printf("Resource: ");
//	resource = get_digit(stdin,0,RES_MAX);
//
//    strcpy(command, argv[0]);
//    strcat(command, "-t");
//    strcat(command, );
//    strcat(command, "-c");
//    strcat(command, );
//    strcat(command, "-r");
//    strcat(command, );
//    strcat(command, "-m");
//    strcat(command, );
//
//}


uint8_t prompt(info_t* info){
	
	printf("Outcome(0)/Income(1)?: ");	
	info->typecome = get_digit(stdin,0,1);

	print_categories(info->typecome);
	printf("Select one by specifying its number: ");
    fscanf(stdin, "%hhu", &info->category);
    //info->category = get_number(stdin, 0, CAT_MAX);

	print_currencies();
	printf("Select one by specifying its number: ");
    fscanf(stdin, "%hhu", &info->currency);

	printf("Payload: ");
	info->payload = get_float(stdin);

	printf("Comment(optional): ");
    info->comment = malloc(INP_MAX);
	if(get_str(stdin, info->comment))
    {
        free(info->comment);
        info->comment=NULL;
    }

	print_resources();
	printf("Resource: ");
    fscanf(stdin, "%hhu" , &info->resource);
    //info->resource = get_digit(stdin,0,2);

	return 0;
}

void init_env()
{
    category_len = 0;
    resource_len = 0;
    currency_len = 0;
    expense_category_len = 0;
    income_category_len = 0;

	char* 	username[NAME_MAX];
	char 	command[PATH_MAX];
	FILE*   tmp;
    conf_t* options;
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
	strcat(dbpath, "/mafin/finances.db");

//reading config file and assigning globals from it

	options = config_read();
    if(options==NULL)
    {
		fprintf(stderr,"Config file is not created or empty!\n");
        exit(1);
    }

    conf_t* head = options;
    options = head;
    while(options!=NULL)
    {
        if(!strcmp("resources", trimwhitespace(options->key)))
            resource_len = config_options_assign(resource,options->value);

        else if(!strcmp("currencies", trimwhitespace(options->key)))
            currency_len = config_options_assign(currency,options->value);

        else if(!strcmp("expenses", trimwhitespace(options->key)))
            expense_category_len = config_options_assign(category[0],options->value);

        else if(!strcmp("incomes", trimwhitespace(options->key)))
            income_category_len = config_options_assign(category[1],options->value);

        else if(!strcmp("dbpath", trimwhitespace(options->key)))
            strcpy(dbpath,trimwhitespace(options->value));
        options=options->next;
    }
    config_options_free(head);

    category_len = income_category_len + expense_category_len;
    
	if((tmp=fopen(dbpath, "r"))==NULL)
	{
		fprintf(stderr,"Database specified is not created!\n");
		fprintf(stderr,"Creating...\n");
        strcpy(command,"touch ");
        strcat(command, dbpath);
        system(command);
		exit(1);
	}
    
	fclose(tmp);

    return;
}

void finish()
{
	printf("\n");
	exit(0);
}




int main(int argc, char** argv)
{
	(void) signal(SIGINT,finish);
    uint8_t status=0;

//    clock_t t;
//    t=clock();

	init_env();

	if(argc==1)
    {
        usage(argv);
        status=1;
    }

	else if(parse_options(argc,argv) < 0)
    {
        time_t t = time(NULL);
        info.time = *localtime(&t);
		if(db_store_info(&info, dbpath))
		{
            status=1;
		}
    }

//    t=clock()-t;
//    printf("%lli cycles, %f microseconds\n",(long long)t,(float)t/2500);

	return status;
}

