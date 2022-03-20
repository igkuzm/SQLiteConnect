/**
 * File              : SQLiteConnect.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 20.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "SQLiteConnect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

//create SQLite database
int 
sqlite_connect_create_database(const char *DataBase)
{
	sqlite3 *db;
	int err = sqlite3_open_v2(DataBase, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	sqlite3_close(db);
	return err;
}

//standart callback - print in STDOUT
int sqlite_callback_print(void *data, int argc,  char **argv, char **columnName){
    int i;
	for (i=0; i< argc; i++)
		if (argv[i] != NULL){
			printf("%s,\t", argv[i]);
		}
		else {
			printf("%s,\t", "(null)");
		}
    printf("\n");
    return 0;
}

//return number of rows for SQL request
int sqlite_connect_execute_function(const char *String, const char *DataBase, void *data,  int (*callback)(void*,int*,int,char**,char**)){
	int err; //error code
	sqlite3 *db;
	sqlite3_stmt *stmt;
	
	err = sqlite3_open(DataBase, &db);
	if (err) {
		fprintf(stderr, "ERROR. Failed to open DB: %s, with error: %d\n", DataBase, err);
		return 0;
	}
	
	err = sqlite3_prepare_v2(db, String, -1, &stmt, NULL);
	if (err) {
		fprintf(stderr, "ERROR. Failed to execute SQL: %s, with error: %d\n", String, err);
		sqlite3_close(db);
		return 0;
	}	

	int num_cols = sqlite3_column_count(stmt); //number of colums
	char *columnNames[num_cols]; //names of colums
	
	//fill column titles
	int i;
	for (i = 0; i < num_cols; ++i) {
		columnNames[i] = NULL;
		if (sqlite3_column_name(stmt, i) != NULL) {
			columnNames[i] = (char *)sqlite3_column_name(stmt, i);
		}
	}	

	//fill values and exec callback for each row
	char *argv[num_cols]; //values
	int count = 0;
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		for (i = 0; i < num_cols; ++i) {
			argv[i] = NULL;
			if (sqlite3_column_text(stmt, i) != NULL) {
				argv[i] = (char *)sqlite3_column_text(stmt, i);
			}			
		}
		
		if (callback) {
			int c = callback(data, &count, num_cols, argv, columnNames); //run callback
			if (c) { //callback return non zero - stop execution
				fprintf(stderr, "SQLiteExecute interupted with code: %d", c);
				if (c == -1) {
					//stop execution with no return
					break;
				}
				else {
					//stop execute with return of callback
					count=c;				
					break;
				}
			}
		}
		count++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return count;
}

int sqlite_connect_get_string_callback(void *data, int *count, int argc, char **argv, char **columnNames){
	char *string=(char*)data;
	strcpy(string, "");		
	
	if (argv[0]!=NULL){
		char buf[BUFSIZ];		
		strncpy(buf, (const char*)argv[0], sizeof(buf) - 1); //safe string copy
		buf[sizeof(buf) - 1] = '\0';
		strcpy(string, buf);
	}	

    return -1; //do not call callback again
}

int sqlite_connect_get_row_callback(void *data, int *count, int argc, char **argv, char **columnNames){
	char **row=(char**)data;
	int i;
	for (i = 0; i < argc; ++i) { //for each column in callback

		if (argv[i]==NULL){ //no segmentation falt if arg is NULL
			argv[i]="";
		}

		char buf[BUFSIZ];		
		strncpy(buf, (const char*)argv[i], sizeof(buf) - 1); //safe copy of string (no buffer overload)
		buf[sizeof(buf) - 1] = '\0';

		char *string = calloc(BUFSIZ, sizeof(char));
		if (!string) {
			fprintf(stderr, "ERROR. Cannot allocate memory\n");		
			return 1;	
		}					
	
		row[i] = string;
	}

    return argc; //return number of rows
}


//return one string from SQL request
char *sqlite_connect_get_string(char *SQL, char *DataBase){
	char *string = calloc(BUFSIZ, sizeof(char));
	if (!string) {
		fprintf(stderr, "ERROR. Cannot allocate memory\n");		
		return NULL;	
	}					
	sqlite_connect_execute_function(SQL, DataBase, string, sqlite_connect_get_string_callback);	
	return string;
}

//return count of colums in one row from SQL request
int sqlite_connect_get_row(char *SQL, char *DataBase, char ***row){
	return sqlite_connect_execute_function(SQL, DataBase, &row, sqlite_connect_get_row_callback);	
}

//execute SQL without callback
int sqlite_connect_execute(const char *String, const char *DataBase){
	
	const char *filename = DataBase;

    sqlite3 *db;
    int rc = sqlite3_open(filename, &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQLExecute Cannot open database %s: %s\n",filename, sqlite3_errmsg(db));
        sqlite3_close(db);
		return 1;
	}

	char *sql = (char *)String;	
	char *err = NULL; 
		
	sqlite3_exec(db, sql, sqlite_callback_print, 0, &err); 
	
	if (err != NULL){
		fprintf(stderr, "ERROR SQLExecute: %s\n", err);
		return 1;
	}
    sqlite3_close(db);	
	return 0;
}



