**SQLiteConnect**

Plain C library to manage SQLite. Uses libsqlite3.

Simple example:

```
#include "SQLiteConnect.h"
#include <stdio.h>

int callback(void *data, int *count, int argc, char **argv, char **titles){
	for (int i = 0; i < argc; ++i) {
		printf("%s: %s\t", titles[i], argv[i]);
	}
	printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
	printf("Create test database\n");
	int err = sqlite_connect_create_database("test.db"); 
	if (err) {
		printf("%s: %d\n", "Error to create database", err);
		return 1;
	}
	printf("OK\n");

	sqlite_connect_execute("CREATE TABLE IF NOT EXISTS test (col_number INT, col_text TEXT)", "test.db");
	sqlite_connect_execute("INSERT INTO test (col_number, col_text) VALUES(1, 'This is test!')", "test.db");

	printf("PRINT TEST TEBLE:\n");
	sqlite_connect_execute_function("SELECT * FROM test", "test.db", NULL, callback);

	getchar();

	remove("test.db");
	
	return 0;
}
```
