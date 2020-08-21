#ifndef FIN_H
#define FIN_H  1

#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <signal.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <getopt.h>


#define PATH_MAX 128
#define NAME_MAX 64
#define INP_MAX 256
#define DBLINE_MAX 128
#define MIN_INP_LEN 3
#define CAT_MAX 255
#define FORMAT "%02d-%02d-%02d-%02d-%02d-%d|%d,%d,%d,%d|%.2f|"

#define within(A, B, C) ((A>=B) && (A<=C))

#define HISTORY_BY_MONTH (1 << 0)
#define HISTORY_BY_WEEK (1 << 1)
#define HISTORY_BY_CATEGORIES (1 << 2)
#define HISTORY_BY_CURRENCIES (1 << 3)
#define HISTORY_BY_RESOURCES (1 << 4)
#define HISTORY_BY_TYPECOMES (1 << 5)
#define HISTORY_BY_ROWS (1 << 6)
#define HISTORY_OVERALL (1 << 7)

char category[2][CAT_MAX][NAME_MAX];
char resource[CAT_MAX][NAME_MAX];
char currency[CAT_MAX][NAME_MAX];

uint8_t category_len;
uint8_t resource_len;
uint8_t currency_len;
uint8_t expense_category_len;
uint8_t income_category_len;

typedef struct config{
        char *key;
        char *value;
        struct config *next;
}conf_t; 

typedef struct row{
	float payload;
    uint8_t typecome;    
    uint8_t category;
    uint8_t resource;
    uint8_t currency;
	struct tm time;
	char* comment;
} info_t;

char cfgPath[PATH_MAX];
char dbpath[PATH_MAX];
info_t info;

uint8_t prompt(info_t* info);
void print_categories(uint8_t typecome);
void print_resources();
void print_currencies();
void print_last(uint32_t number);
uint8_t print_history();
extern int parse_options(int argc, char**argv);
void usage(char**argv);
uint8_t sort_payload(uint8_t order);
uint8_t sort_uint8(uint8_t order, char* elem);
uint8_t sort_bit(uint8_t order);
uint8_t sort_date(uint8_t order);
uint8_t show_history(uint8_t flags);
//void mysql_create(const char * dbname);

#endif
