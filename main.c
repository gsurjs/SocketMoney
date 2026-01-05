/*
 * SocketMoney - A simple transaction logger and alternative to RocketMoney using C and SQLite 
 *
 * Description: Entry point for the application.
 * Handles the main execution loop and user menu.
 * 
 * Compilation: gcc main.c storage.c -o socketMoney -lsqlite3 
 */

 #include <stdio.h>
 #include "storage.h" // importing custom interface for database operations

 int main() {
    sqlite3 *db;

    // init db connection, pass address of pointer so function can modify it
    initialize_db(&db);

    int choice = 0; // init choice at 0

    // init while loop to break when user exits
    while (choice != 4) {
        printf("\n--- SocketMoney Finance Tool ---\n");
        printf("1. Add New Transaction\n");
        printf("2. View All Transactions\n");
        printf("3. View Total Spending\n");
        printf("4. Exit\n");
        printf("Select an option (1-4): ");

        // input validation, check if scanf successfully read an integer
        if (scanf("%d", &choice) != 1) {
            // edge case if scanf fails, assume bad input.
            while(getchar() != '\n'); // clear input buffer
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue; // restart loop
        }
        // use switch to handle user choice
        switch(choice) {
            case 1:
                add_expense(db);
                break;
            case 2:
                view_expenses(db);
                break;
            case 3:
                view_total(db);
                break;
            case 4:
                printf("Exiting SocketMoney. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please select a number between 1 and 4.\n");
        }
    }
    // close db connection and flush any pending writes
    sqlite3_close(db);
    return 0;
 }