#pragma once
void get_digit(FILE* stream,uint8_t *digit, uint8_t left, uint8_t right);
uint8_t get_str(FILE* stream, char str[]);
uint8_t get_float(FILE* stream, float *number);
uint8_t get_password(char* password);
uint8_t get_name(char *name);
uint8_t get_username(char **name);
