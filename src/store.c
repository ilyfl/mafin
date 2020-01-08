
//fin.c collects the information from the user and sends it in this place
//this file gets this information and stores it into file specificated by config file

#include "fin.h"

uint8_t storedb(answer_t* info, char* dbpath)
{
	FILE *db;
	if((db=fopen(dbpath, "a"))==NULL)
	{	
		return 1;	
	}	

	fprintf(db,"%d,%d,%f,%s,%d\n", info->typecome, info->category, info->payload, info->comment, info->resource);

	fclose(db);
	
	return 0;
}
