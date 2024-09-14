#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book structure
typedef struct Book {
    int id;
    char title[100];
    int available; // 1 for available, 0 for borrowed
    struct Book* next;
} Book;

// User structure (Queue node)
typedef struct User {
    char name[100];
    struct User* next;
} User;

// Stack for borrowing history
typedef struct History {
    char userName[100];
    char bookTitle[100];
    struct History* next;
} History;

// Function prototypes
void addBook(int id, char title[]);
void displayBooks();
void borrowBook(char userName[], int bookId);
void returnBook(char userName[], int bookId);
void addToWaitingList(int bookId, char userName[]);
void displayWaitingList(int bookId);
void addHistory(char userName[], char bookTitle[]);
void displayBorrowHistory();

// Global variables
Book* library = NULL; // Head of the book linked list
History* historyStack = NULL; // Stack for history

// Functions for managing the library system

// Add a book to the library
void addBook(int id, char title[]) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    newBook->available = 1;
    newBook->next = library;
    library = newBook;
    printf("Book added: %s (ID: %d)\n", title, id);
}

// Display available books
void displayBooks() {
    Book* temp = library;
    printf("\nAvailable Books:\n");
    while (temp != NULL) {
        if (temp->available)
            printf("ID: %d, Title: %s\n", temp->id, temp->title);
        temp = temp->next;
    }
}

// Borrow a book
void borrowBook(char userName[], int bookId) {
    Book* temp = library;
    while (temp != NULL) {
        if (temp->id == bookId) {
            if (temp->available) {
                temp->available = 0;
                printf("Book borrowed: %s by %s\n", temp->title, userName);
                addHistory(userName, temp->title);
                return;
            } else {
                printf("Book is not available. Adding %s to the waiting list.\n", userName);
                addToWaitingList(bookId, userName);
                return;
            }
        }
        temp = temp->next;
    }
    printf("Book not found.\n");
}

// Return a book
void returnBook(char userName[], int bookId) {
    Book* temp = library;
    while (temp != NULL) {
        if (temp->id == bookId) {
            if (!temp->available) {
                temp->available = 1;
                printf("Book returned: %s by %s\n", temp->title, userName);
                displayWaitingList(bookId);
                return;
            } else {
                printf("This book was not borrowed.\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("Book not found.\n");
}

// Waiting list queue node
typedef struct WaitingList {
    char userName[100];
    struct WaitingList* next;
} WaitingList;

// Waiting list management
WaitingList* waitingLists[100] = { NULL }; // Array of waiting lists for books

void addToWaitingList(int bookId, char userName[]) {
    WaitingList* newUser = (WaitingList*)malloc(sizeof(WaitingList));
    strcpy(newUser->userName, userName);
    newUser->next = NULL;

    if (waitingLists[bookId] == NULL) {
        waitingLists[bookId] = newUser;
    } else {
        WaitingList* temp = waitingLists[bookId];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newUser;
    }
}

void displayWaitingList(int bookId) {
    if (waitingLists[bookId] != NULL) {
        printf("Waiting list for book ID %d:\n", bookId);
        WaitingList* temp = waitingLists[bookId];
        while (temp != NULL) {
            printf("%s\n", temp->userName);
            temp = temp->next;
        }
    } else {
        printf("No waiting list for this book.\n");
    }
}

// Add to borrow history (stack)
void addHistory(char userName[], char bookTitle[]) {
    History* newHistory = (History*)malloc(sizeof(History));
    strcpy(newHistory->userName, userName);
    strcpy(newHistory->bookTitle, bookTitle);
    newHistory->next = historyStack;
    historyStack = newHistory;
}

void displayBorrowHistory() {
    printf("\nBorrow History:\n");
    History* temp = historyStack;
    while (temp != NULL) {
        printf("%s borrowed %s\n", temp->userName, temp->bookTitle);
        temp = temp->next;
    }
}

// Main function
int main() {
    int choice, bookId;
    char userName[100], bookTitle[100];

    while (1) {
        printf("\n--- Library Management System ---\n");
        printf("1. Add a book\n");
        printf("2. Display available books\n");
        printf("3. Borrow a book\n");
        printf("4. Return a book\n");
        printf("5. Display borrow history\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer

        switch (choice) {
            case 1:
                printf("Enter book ID: ");
                scanf("%d", &bookId);
                getchar(); // Clear buffer
                printf("Enter book title: ");
                fgets(bookTitle, 100, stdin);
                bookTitle[strcspn(bookTitle, "\n")] = '\0'; // Remove newline character
                addBook(bookId, bookTitle);
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                printf("Enter your name: ");
                fgets(userName, 100, stdin);
                userName[strcspn(userName, "\n")] = '\0'; // Remove newline character
                printf("Enter book ID to borrow: ");
                scanf("%d", &bookId);
                borrowBook(userName, bookId);
                break;
            case 4:
                printf("Enter your name: ");
                fgets(userName, 100, stdin);
                userName[strcspn(userName, "\n")] = '\0'; // Remove newline character
                printf("Enter book ID to return: ");
                scanf("%d", &bookId);
                returnBook(userName, bookId);
                break;
            case 5:
                displayBorrowHistory();
                break;
            case 6:
                printf("Exiting the system...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
