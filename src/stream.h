#pragma once


char *trimwhitespace(char *str);
uint8_t db_read_info(FILE* dbfd, info_t* info, uint64_t* pos); /*reads one line of information from database file at position pos*/
uint8_t db_store_info(info_t* info, char* dbpath);/*writes new line of information to the path dbpath*/
uint8_t db_rm_entry_by_num(const char* dbpath, int64_t number);/*removes line of information from database by a number of the line*/
uint8_t db_ins_entry_by_num(info_t* info,char* dbpath, uint64_t number); /*inserts line of information to the database by number*/
uint8_t db_change_entry_by_num(info_t* info,char* dbpath, uint64_t number); /*changes line of information at the database by number*/
conf_t* config_read(); /*reads configuration file and returns pointer to the filled conf_t struct*/
void config_options_free(conf_t* options); /* internals? frees allocated by config_read memory */
uint32_t config_options_assign(char* var, char* value); /* internals? fills global variables by the information from config_read */
int db_from_sheet_convert(info_t* info, char* dbpath); /* reads from stdin in formats below, converts it and stores at dbpath 
                                                            dd.mm.yyyy hh:mm:ss category payload comment resource
                                                            dd.mm.yyyy hh:mm:ss category payload  resource */
                                                                                                               
