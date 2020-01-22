

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#pragma once

#define PATH_MAX 128
#define NAME_MAX 64
#define MIN_INP_LEN 3
#define CAT_MAX 16 
#define RES_NUM 3
#define RES_MASK 7
#define CAT_MASK 120

//enum expense {Food, Eating, Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other};
//enum income {Salary, Wages, Random};
//enum res {Card, Cash, Credit};

char cfgPath[PATH_MAX];
char dbpath[PATH_MAX];

void mysql_create(const char * dbname);


//typedef struct user{
//	char username[MAX_INP_LEN];
//	char password[MAX_INP_LEN];	
//}user_t;

typedef struct answer{
	char comment[64];
	float payload;
	int8_t tcr;
	struct tm time;
} answer_t;






