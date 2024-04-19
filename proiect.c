#include "proiect.h"

char user_name[50];
char user_surname[50];

char books[100][100];
char authors[100][100];
int available_copies[100];
int num_books = 0;

char borrowed_books[10][100][100];
char borrowed_authors[10][100][100];
int borrowed_copies[10][100];
int num_borrowed = 0;

void trim(char *str);

void display_menu() {
    printf("\nMenu:\n");
    printf("1. View available books in the library\n");
    printf("2. Borrow books\n");
    printf("3. View borrowed books\n");
    printf("4. Return/Donate books\n");
    printf("5. Search books\n");
    printf("6. Exit\n");
}

void read_file(int argc, char **argv) {
    if (argc == 3) {
        strcpy(user_name, argv[1]);
        strcpy(user_surname, argv[2]);
    } else {
        printf("Enter your first and last name: ");
        scanf("%s %s", user_name, user_surname);
    }

    FILE *books_file = fopen("books.txt", "r");
    if (books_file == NULL) {
        printf("Error opening 'books.txt' file.\n");
    }

    FILE *loans_file = fopen("loans.txt", "r");
    if (loans_file == NULL) {
        printf("Error opening 'loans.txt' file.\n");
    }

    char line[256];
    while (fgets(line, sizeof(line), books_file)) {
        char *title = strtok(line, ",");
        char *author = strtok(NULL, ",");
        char *copies = strtok(NULL, ".");
        if (title != NULL && author != NULL && copies != NULL) {
            char *end;
            while (isspace(*title)) title++;
            end = title + strlen(title) - 1;
            while (end > title && isspace(*end)) end--;
            *(end + 1) = '\0';

            while (isspace(*author)) author++;
            end = author + strlen(author) - 1;
            while (end > author && isspace(*end)) end--;
            *(end + 1) = '\0';

            int copies_val = atoi(copies);

            strcpy(books[num_books], title);
            strcpy(authors[num_books], author);
            available_copies[num_books] = copies_val;

            num_books++;
        }
    }

    while (fgets(borrowed_books[num_borrowed][0], 100, loans_file) != NULL) {
        trim(borrowed_books[num_borrowed][0]);
        fgets(borrowed_authors[num_borrowed][0], 100, loans_file);
        trim(borrowed_authors[num_borrowed][0]);
        fscanf(loans_file, "%d\n", &borrowed_copies[num_borrowed][0]);
        num_borrowed++;
    }

    fclose(books_file);
    fclose(loans_file);
}

void trim(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while ((end >= start) && isspace(str[end])) end--;
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

void display_available_books() {
    system("cls");
    printf("Books available in the library:\n");
    for (int i = 0; i < num_books; i++) {
        printf("- %s by %s (%d copies)\n", books[i], authors[i], available_copies[i]);
    }
}

void return_book() {
    system("cls");
    int option;
    printf("1. Return borrowed book\n");
    printf("2. Donate a book to the library\n");
    printf("Enter your choice: ");
    scanf("%d", &option);

    switch (option) {
        case 1: {
            int loan_index;
            printf("Enter the loan index for return: ");
            scanf("%d", &loan_index);

            if (loan_index >= 0 && loan_index < num_borrowed) {
                for (int i = 0; i < num_books; i++) {
                    if (strcmp(books[i], borrowed_books[loan_index][0]) == 0 && strcmp(authors[i], borrowed_authors[loan_index][0]) == 0) {
                        available_copies[i] += borrowed_copies[loan_index][0];
                        break;
                    }
                }
                for (int i = loan_index; i < num_borrowed - 1; i++) {
                    strcpy(borrowed_books[i][0], borrowed_books[i + 1][0]);
                    strcpy(borrowed_authors[i][0], borrowed_authors[i + 1][0]);
                    borrowed_copies[i][0] = borrowed_copies[i + 1][0];
                }
                num_borrowed--;
                printf("Return successful.\n");
            } else {
                printf("Invalid loan index.\n");
            }
            break;
        }
        case 2: {
            system("cls");
            char title[100];
            char author[100];
            int num_copies;

            printf("Enter the book title: ");
            scanf(" %[^\n]", title);
            trim(title);
            printf("Enter the book author: ");
            scanf(" %[^\n]", author);
            trim(author);
            printf("Enter the number of copies donated: ");
            scanf("%d", &num_copies);

            FILE *books_file = fopen("books.txt", "a");
            if (books_file == NULL) {
                printf("Error opening 'books.txt' file for adding donated books.\n");
                return;
            }

            fprintf(books_file, "%s, %s, %d.\n", title, author, num_copies);
            fclose(books_file);

            printf("The book '%s' has been successfully donated.\n", title);
            break;
        }
        default:
            printf("Invalid option.\n");
    }
}

void display_borrowed_books() {
    system("cls");
    printf("Books borrowed by %s %s:\n", user_name, user_surname);
    for (int i = 1; i < num_borrowed; i++) {
        printf("Loan %d:\n", i);
        for (int j = 0; j < num_books; j++) {
            if (borrowed_copies[i][j] > 0) {
                printf("- %s by %s (%d copies)\n", borrowed_books[i][j], borrowed_authors[i][j], borrowed_copies[i][j]);
            }
        }
    }
}

void borrow_book() {
    system("cls");
    char title[100];
    char author[100];
    int num_copies;

    printf("Enter the book title: ");
    scanf(" %[^\n]", title);
    trim(title);
    printf("Enter the book author: ");
    scanf(" %[^\n]", author);
    trim(author);
    printf("Enter the number of copies needed: ");
    scanf("%d", &num_copies);

    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i], title) == 0 && strcmp(authors[i], author) == 0) {
            if (available_copies[i] >= num_copies) {
                strcpy(borrowed_books[num_borrowed][0], books[i]);
                strcpy(borrowed_authors[num_borrowed][0], authors[i]);
                borrowed_copies[num_borrowed][0] = num_copies;
                available_copies[i] -= num_copies;
                num_borrowed++;
                printf("The book '%s' has been borrowed successfully.\n", title);
            } else {
                printf("There are not enough available copies for '%s'.\n", title);
            }
            return;
        }
    }
    printf("The book '%s' by '%s' was not found in the library.\n", title, author);
}

void search_book() {
    system("cls");
    char search[100 + 100];
    int book_found = 0;

    printf("Enter (title) or (author) or (title,author) to search: ");
    scanf(" %[^\n]", search);

    char *title = strtok(search, ",");
    char *author = strtok(NULL, ",");

    if (author == NULL) {
        printf("Search results for '%s':\n", title);
        for (int i = 0; i < num_books; i++) {
            if (strstr(books[i], title) != NULL || strstr(authors[i], title) != NULL) {
                printf("- %s by %s (%d copies)\n", books[i], authors[i], available_copies[i]);
                book_found = 1;
            }
        }
    } else {
        printf("Search results for '%s' by '%s':\n", title, author);
        for (int i = 0; i < num_books; i++) {
            if (strstr(books[i], title) != NULL && strstr(authors[i], author) != NULL) {
                printf("- %s by %s (%d copies)\n", books[i], authors[i], available_copies[i]);
                book_found = 1;
            }
        }
    }

    if (book_found == 0) {
        printf("The searched book does not exist.\n");
    }
}
