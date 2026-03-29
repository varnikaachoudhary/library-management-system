#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define PASSWORD "admin123"

struct Book {
    int id;
    char title[50];
    char author[50];
    int quantity;
};

struct Book books[MAX];
int count = 0;

// Prototypes
void loadFromFile();
void saveToFile();
void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
int checkPassword();
void logTransaction(const char* studentID, const char* studentName, int bookID, const char* bookTitle, const char* status);

int main() {
    if (!checkPassword()) {
        printf("Access denied. Exiting...\n");
        return 0;
    }

    int choice;
    loadFromFile();

    do {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: saveToFile(); printf("Data saved. Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 7);

    return 0;
}

int checkPassword() {
    char input[50];
    printf("Enter password to access the system: ");
    fgets(input, sizeof(input), stdin);
    strtok(input, "\n");

    if(strcmp(input, PASSWORD) == 0) {
        printf("Access granted.\n");
        return 1;
    } else {
        printf("Incorrect password.\n");
        return 0;
    }
}

void loadFromFile() {
    FILE *fp = fopen("library.txt", "r");
    if(fp == NULL) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }

    while(fscanf(fp, "%d\n%[^\n]\n%[^\n]\n%d\n", &books[count].id, books[count].title, books[count].author, &books[count].quantity) == 4) {
        count++;
    }

    fclose(fp);
}

void saveToFile() {
    FILE *fp = fopen("library.txt", "w");
    if(fp == NULL) {
        printf("Error saving to file!\n");
        return;
    }

    for(int i = 0; i < count; i++) {
        fprintf(fp, "%d\n%s\n%s\n%d\n", books[i].id, books[i].title, books[i].author, books[i].quantity);
    }

    fclose(fp);
}

void addBook() {
    if(count >= MAX) {
        printf("Library full.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &books[count].id);
    getchar();

    printf("Enter Book Title: ");
    fgets(books[count].title, sizeof(books[count].title), stdin);
    strtok(books[count].title, "\n");

    printf("Enter Book Author: ");
    fgets(books[count].author, sizeof(books[count].author), stdin);
    strtok(books[count].author, "\n");

    printf("Enter Quantity: ");
    scanf("%d", &books[count].quantity);

    count++;
    saveToFile();
    printf("Book added.\n");
}

void displayBooks() {
    if(count == 0) {
        printf("No books available.\n");
        return;
    }

    for(int i = 0; i < count; i++) {
        printf("ID: %d | Title: %s | Author: %s | Quantity: %d\n", books[i].id, books[i].title, books[i].author, books[i].quantity);
    }
}

void searchBook() {
    char title[50];
    getchar();
    printf("Enter book title to search: ");
    fgets(title, sizeof(title), stdin);
    strtok(title, "\n");

    for(int i = 0; i < count; i++) {
        if(strcasecmp(books[i].title, title) == 0) {
            printf("Found - ID: %d | Author: %s | Quantity: %d\n", books[i].id, books[i].author, books[i].quantity);
            return;
        }
    }
    printf("Book not found.\n");
}

void issueBook() {
    int id;
    char studentID[20], studentName[50];
    printf("Enter Student ID: ");
    scanf("%s", studentID);
    getchar();
    printf("Enter Student Name: ");
    fgets(studentName, sizeof(studentName), stdin);
    strtok(studentName, "\n");

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    for(int i = 0; i < count; i++) {
        if(books[i].id == id) {
            if(books[i].quantity > 0) {
                books[i].quantity--;
                saveToFile();
                logTransaction(studentID, studentName, books[i].id, books[i].title, "Issued");
                printf("Book issued.\n");
            } else {
                printf("Book not available.\n");
            }
            return;
        }
    }
    printf("Book ID not found.\n");
}

void returnBook() {
    int id;
    char studentID[20], studentName[50];
    printf("Enter Student ID: ");
    scanf("%s", studentID);
    getchar();
    printf("Enter Student Name: ");
    fgets(studentName, sizeof(studentName), stdin);
    strtok(studentName, "\n");

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    for(int i = 0; i < count; i++) {
        if(books[i].id == id) {
            books[i].quantity++;
            saveToFile();
            logTransaction(studentID, studentName, books[i].id, books[i].title, "Returned");
            printf("Book returned.\n");
            return;
        }
    }
    printf("Book ID not found.\n");
}

void deleteBook() {
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < count; i++) {
        if(books[i].id == id) {
            for(int j = i; j < count - 1; j++) {
                books[j] = books[j + 1];
            }
            count--;
            saveToFile();
            printf("Book deleted.\n");
            return;
        }
    }

    printf("Book ID not found.\n");
}

void logTransaction(const char* studentID, const char* studentName, int bookID, const char* bookTitle, const char* status) {
    FILE *fp = fopen("transactions.txt", "a");
    if(fp == NULL) {
        printf("Error opening transactions.txt\n");
        return;
    }
    time_t now = time(NULL);
    char* date = ctime(&now);
    date[strlen(date)-1] = '\0';
    fprintf(fp, "%s,%s,%d,%s,%s,%s\n", studentID, studentName, bookID, bookTitle, status, date);
    fclose(fp);
}
