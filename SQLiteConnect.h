/**
 * File              : SQLiteConnect.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 19.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef SQLITE_CONNECT
#define SQLITE_CONNECT value

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//execute SQLite SQL STRING for DATABASE
//DATA - return void in CALLBACK
int SQLiteExecuteFunction(
		const char *String, //SQL string to execute 
		const char *DataBase, //SQLite database filename
		void *user_data, //pointer to transfer throw callback  
		int (*callback)( //callback function. return non zero to stop function 
			void* user_data, //transfered pointer 
			int* count, //count of rows
			int argc, //count of arguments(columns)
			char** argv, //aray of arguments (cell values)
			char** titles //array of titles of culumns
		)
);
int SQLiteExecute(const char *String, const char *DataBase); //execute without callback

//returns one String (first row and first column) from SQL Request
char *stringWithSQLiteRequest(char *SQL, char *DataBase);

//return number of colums of sqlrequest with values in row
int rowWithSQLiteRequest(char *SQL, char *DataBase, char ***row);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif /* ifndef SQLITE_CONNECT */
