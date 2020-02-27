#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#pragma once

#define PATH_MAX 128
#define NAME_MAX 64
#define DBLINE_MAX 128
#define MIN_INP_LEN 3
#define CAT_MAX 16 
#define RES_MAX 7
#define RES_MASK 7
#define CAT_MASK 120
#define CLSFT 3
#define TLSFT 7

#define within(A, B, C) ((A>=B) && (A<=C))

typedef struct answer{
	char comment[64];
	float payload;
    int8_t tcr;    
	struct tm time;
} answer_t;

char cfgPath[PATH_MAX];
char dbpath[PATH_MAX];
answer_t info;


uint8_t prompt(answer_t* info);
void print_categories(const uint8_t typecome);
void print_resources();
void print_last(uint32_t number);
uint8_t show_history();
extern int parse_opts(int argc, char**argv);
//void mysql_create(const char * dbname);



