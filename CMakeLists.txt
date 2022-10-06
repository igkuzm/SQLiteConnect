cmake_minimum_required(VERSION 3.15)
set(CMAKE_BUILD_TYPE Release)
project(SQLiteConnect)

add_library(SQLiteConnect STATIC SQLiteConnect.c sqlite3.c)

add_executable(sqlite_connect_test test.c)
target_link_libraries(sqlite_connect_test SQLiteConnect)
if (NOT ANDROID)
	target_link_libraries(sqlite_connect_test SQLiteConnect pthread dl)
endif()
