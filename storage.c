/*
 * Description: Implementation of database logic.
 * Contains strict SQL handling using Prepared Statements to ensure
 * security and Type safety.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define DB_NAME "socketmoney.db"

void initialize_db(sqlite3 **db) {
    // sqlite3_open creates the file if it doesn't exist.
    int rc = sqlite3_open(DB_NAME, db);
    
    if (rc) {
        // stderr is used for errors so they can be separated from standard output
        fprintf(stderr, "CRITICAL: Can't open database: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    // sql schema definition, using 'IF NOT EXISTS' to avoid errors on re-initialization
    char *sql = "CREATE TABLE IF NOT EXISTS expenses ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "date TEXT NOT NULL,"
                "category TEXT NOT NULL,"
                "amount REAL NOT NULL,"
                "description TEXT);";

    char *errMsg = 0;

    // execute schema creation
    // string is hardcoded internally, so sqlite3_exec is safe
    rc = sqlite3_exec(*db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "CRITICAL: SQL error: %s\n", errMsg);
        sqlite3_free(errMsg); // free memory allocated by SQLite for the error message
    }
}

void add_expense(sqlite3 *db) {
    // input buffers with specific sizes
    char date[11]; // includes null terminator
    char category[50];
    double amount;
    char description[255];

    printf("\n --- Add New Transaction --- \n");

    // adding buffer safety, using %10s limit to prevent buffer overflows
    printf("Date (YYYY-MM-DD): ");
    scanf("%10s", date);

    printf("\nCategory (i.e. Coffee, Groceries): ");
    scanf("%49s", category);

    printf("\nTransaction Amount: ");
    scanf("%lf", &amount);

    // clean newline char from buffer before calling fgets
    while ((getchar() != '\n'));

    printf("\nDescription: ");
    fgets(description, 100, stdin);
    // remove newline char from fgets input
    description[strcspn(description, "\n")] = 0;

    // prevent injection attacks by not letting data ever get injected into the string, making sql injection attacks null
    char *sql = "INSERT INTO expenses (date, category, amount, description) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    // prepare statement by compiling sql to byte-code
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR: Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    // bind user input to prepared statement parameters
    sqlite3_bind_text(stmt, 1, date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, category, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, description, -1, SQLITE_STATIC);

    // execute the prepared statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "ERROR: Failed to execute statement: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nTransaction added successfully!\n");
    }
    sqlite3_finalize(stmt); // free resources allocated to the statement
}

void view_expenses(sqlite3 *db) {
   sqlite3_stmt *stmt;
   char *sql = "SELECT id, date, category, amount, description FROM expenses ORDER BY date DESC;";
   // prepare query
   if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
       fprintf(stderr, "ERROR: Failed to prepare statement: %s\n", sqlite3_errmsg(db));
       return;
   }

   // formatting left-align string within 5 spaces
   printf("\n%-5s %-12s %-15s %-10s %-20s\n", "ID", "Date", "Category", "Amount", "Description");
   printf("---------------------------------------------------------------\n");

   // loop through results
   // sqlite3_step returns SQLITE+ROW if there is data to read
   while (sqlite3_step(stmt) == SQLITE_ROW) {
       int id = sqlite3_column_int(stmt, 0);
       const unsigned char *date = sqlite3_column_text(stmt, 1);
       const unsigned char *category = sqlite3_column_text(stmt, 2);
       double amount = sqlite3_column_double(stmt, 3);
       const unsigned char *description = sqlite3_column_text(stmt, 4);

       printf("%-5d %-12s %-15s $%-9.2f %-20s\n", id, date, category, amount, description);
   }

    sqlite3_finalize(stmt); // free resources

}

void view_total(sqlite3 *db) {
    sqlite3_stmt *stmt;
    // aggregate fnction for letting sql engine to the sum
    char *sql = "SELECT SUM(amount) FROM expenses;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double total = sqlite3_column_double(stmt, 0);
            printf("\n>> Total Lifetime Spending: $%.2f\n", total);
        }
    }
    sqlite3_finalize(stmt); // free resources
}