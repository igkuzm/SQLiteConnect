/**
 * File              : SQLiteConnect.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 09.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//execute SQLite SQL STRING for DATABASE
//DATA - return void in CALLBACK
extern int SQLiteExecuteFunction(const char *String, const char *DataBase, void *data,  int (*callback)(void*,int*,int,char**,char**));
extern int SQLiteExecute(const char *String, const char *DataBase); //execute without callback

//standart callback - print in STDOUT
extern int SQLiteCallbackFunctionPrint(void *data, int argc, char **argv, char **columnName);

//returns one String (first row and first column) from SQL Request
extern char *stringWithSQLiteRequest(char *SQL, char *DataBase);
//return number of colums of sqlrequest with values in row
extern int rowWithSQLiteRequest(char *SQL, char *DataBase, char ***row);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

