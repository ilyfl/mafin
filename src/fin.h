
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <stdint.h>

#include <string.h>

#define PATH_MAX 128
#define NAME_MAX 64
#define MIN_INP_LEN 3
#define CAT_MAX 10 
#define RES_NUM 3

//enum expense {Food, Eating, Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other};
//enum income {Salary, Wages, Random};
//enum res {Card, Cash, Credit};


void mysql_create(const char * dbname);


void get_digit(int *digit);
uint8_t get_str(char str[]);
uint8_t get_float(float *number);
uint8_t get_password(char* password);
uint8_t get_name(char *name);
uint8_t get_username(char **name);
uint8_t read_config();
void print_categories(uint8_t typecome);

//typedef struct user{
//	char username[MAX_INP_LEN];
//	char password[MAX_INP_LEN];	
//}user_t;

typedef struct answer{
	uint8_t typecome;
	uint8_t category;
	float payload;
	char comment[54];
	uint8_t resource;
} answer_t;


uint8_t prompt(answer_t* form);