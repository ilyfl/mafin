#pragma once


uint8_t write_config(const char *left, const char* right);
conf_t* read_config();
void co_assign(char* var, char* value);
uint8_t readdb(FILE* dbfd, answer_t* info, long* pos);
uint8_t storedb(answer_t* info, char* dbpath);
uint8_t rmEntry_n(const char* dbpath, size_t number);
uint8_t insEntry_n(answer_t* info,char* dbpath, size_t number); 
uint8_t chEntry_n(answer_t* info,char* dbpath, size_t number); 
