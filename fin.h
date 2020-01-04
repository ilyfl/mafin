
#pragma once


enum expense {Food, Eating, Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other};
enum income {Salary, Wages, Random};
enum res {Card, Cash, Credit};

typedef struct answer{
	unsigned char typecome;
	unsigned char category;
	float payload;
	char comment[54];
	unsigned char resource;
} t_answer;


