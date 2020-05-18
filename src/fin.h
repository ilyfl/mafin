#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#pragma once

#define PATH_MAX 128
#define NAME_MAX 64
#define INP_MAX 256
#define DBLINE_MAX 128
#define MIN_INP_LEN 3
#define CAT_MAX 255
#define FORMAT "%02d-%02d-%02d-%02d-%02d-%d|%d,%d,%d,%d|%.2f|"

#define within(A, B, C) ((A>=B) && (A<=C))

char category[2][CAT_MAX][NAME_MAX];
char resource[CAT_MAX][NAME_MAX];
char currency[CAT_MAX][NAME_MAX];

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
void print_categories(const uint8_t typecome);
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
//void mysql_create(const char * dbname);

