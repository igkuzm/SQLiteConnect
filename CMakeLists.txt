project(SQLiteConnect)

add_library(SQLiteConnect STATIC SQLiteConnect.c sqlite3.c)
target_link_libraries(SQLiteConnect pthread dl)
