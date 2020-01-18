#pragma once

uint8_t write_config(const char *left, const char* right);
uint8_t read_config(const char *left, char* right);
uint8_t readdb(char* dbpath, answer_t* info);
uint8_t storedb(answer_t* info, char* dbpath);
