
#pragma once

#include <string.h>
#include <linux/limits.h>

#define MIN_INP_LEN 3
#define CAT_MAX 32

enum expense {Food, Eating, Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other};
enum income {Salary, Wages, Random};
enum res {Card, Cash, Credit};

char cfgPath[PATH_MAX]="/home/";

char dbpath[PATH_MAX]="/home/";


char categories[2][32][NAME_MAX]={{"Food", "Eating", "Entertainment", "Transport", "Bills", "Clothes", "Health", "Phone", "Toiletry", "Other"},{"Salary", "Wages", "Random"}};

void mysql_create(const char * dbname);
uint8_t get_password(char* password);
uint8_t get_name(char *name);
uint8_t get_username(char **name);

//typedef struct user{
//	char username[MAX_INP_LEN];
//	char password[MAX_INP_LEN];	
//}user_t;

typedef struct answer{
	unsigned char typecome;
	unsigned char category;
	float payload;
	char comment[54];
	unsigned char resource;
} answer_t;


uint8_t prompt(answer_t* form);
