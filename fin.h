
#pragma once

#define MAX_INP_LEN 32 
#define MIN_INP_LEN 3

enum expense {Food, Eating, Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other};
enum income {Salary, Wages, Random};
enum res {Card, Cash, Credit};

uint8_t prompt(answer_t* form);
void mysql_create(const char * dbname);
uint8_t get_password(char* password);
uint8_t get_name(char *name);

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


