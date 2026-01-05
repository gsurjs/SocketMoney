/*
 * Header file defining the interface for database operations.
 * This allows the main application to interact with data without
 * needing to know the underlying SQL implementation details.
 */

 // preprocessor directives to prevent file from being included multiple times in single compilation, which would cause errors
#ifndef STORAGE_H
#define STORAGE_H
#include <sqlite3.h>

// funct prototypes that tell compiler that functions exist, even if code isn't defined in storage.c

void initialize_db(sqlite3 **db); // connects SQLite db and ensures schema exists, @param db: pointer to db connection pointer

void add_expense(sqlite3 *db); // prompts user for input and securely inserts a new expense, @param db: active db connection

void view_expenses(sqlite3 *db); // retrieves and displays all expenses from db, @param db: active db connection

void view_total(sqlite3 *db); // calculates and displays total expenses, @param db: active db connection

#endif // STORAGE_H