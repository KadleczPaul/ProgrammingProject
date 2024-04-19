/*Each person can borrow multiple books from a library. Each person is uniquely identified by their
NAME and SURNAME. Every book contains information regarding its:
- Title
- Author
- Copies available
For each loan, a document containing relevant information will be generated:
- Which person borrowed the books
- The books borrowed:
o Title + Author
o Copies borrowed

Any person can see all the loans that they made. Furthermore, ANY person can search the library for a
specific book (by its title or author). Any person can return the books that they borrowed, or they can
donate new books to the library.
*/

#include "proiect.h"

int main(int argc, char **argv) {
    // Read file based on command line arguments
    read_file(argc, argv);

    int option;
    do {
        // Display menu
        display_menu();
        printf("\nEnter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                display_available_books();
                break;
            case 2:
                borrow_book();
                break;
            case 3:
                display_borrowed_books();
                break;
            case 4:
                return_book();
                break;
            case 5:
                search_book();
                break;
            case 6:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (option != 6);

    return 0;
}


