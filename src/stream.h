#pragma once

uint8_t write_config(const char *left, const char* right);
uint8_t read_config(FILE* stream, const char *left, char* right);
uint8_t readdb(FILE* dbfd, answer_t* info, long* pos);
uint8_t storedb(answer_t* info, char* dbpath);
