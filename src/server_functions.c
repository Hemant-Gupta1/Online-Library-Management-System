#include "../header/server.h"   // Include server header file for function declarations
#include "../header/head.h"     // Include head header file for structure definitions

#define PERM 0666   // File permissions for open() function


// Function to get the ID for a new book entry
int get_id()
{
    int file_descriptor = open("../db/books.bin", O_RDONLY, PERM);  // Open the books database file in read-only mode

    int off = lseek(file_descriptor, 0, SEEK_END);  // Get the current offset (file size)

    if (off > 0)
        return (off / sizeof(struct Book) + 1);  // If file is not empty, calculate the ID based on the number of records
    else
        return 1;  // If file is empty, start ID from 1
}

// Function to display all books in the database
void get_all_books()
{
    struct Book temp;

    int file_descriptor = open("../db/books.bin", O_RDONLY, PERM);  // Open the books database file in read-only mode

    read(file_descriptor, &temp, sizeof(struct Book));  // Read the first record from the file

    printf("Title: %s\n", temp.title);  // Display book title
    printf("Author: %s\n", temp.author);  // Display book author
    printf("Copies: %d\n", temp.copies);  // Display number of copies
    printf("Valid: %d\n", temp.valid);  // Display validity status
    printf("ID: %d\n", temp.id);  // Display book ID
    printf("\n");

    while (read(file_descriptor, &temp, sizeof(struct Book)))  // Read remaining records from the file
    {
        printf("Title: %s\n", temp.title);  // Display book title
        printf("Author: %s\n", temp.author);  // Display book author
        printf("Copies: %d\n", temp.copies);  // Display number of copies
        printf("Valid: %d\n", temp.valid);  // Display validity status
        printf("ID: %d\n", temp.id);  // Display book ID
        printf("\n");
    }
}

// Function to add a new book to the database
void add_book(int nsd)
{
    int file_descriptor;
    sem_wait(&books_semaphore);  // Wait for access to the books database

    struct Book temp;

    bzero(&temp, sizeof(struct Book));

    read(nsd, &temp, sizeof(struct Book));  // Read new book details from client

    temp.id = get_id();  // Get a new ID for the book

    file_descriptor = open("../db/books.bin", O_WRONLY | O_CREAT, PERM);  // Open the books database file in write-only mode

    lseek(file_descriptor, 0, SEEK_END);  // Move the file pointer to the end of the file

    write(file_descriptor, &temp, sizeof(struct Book));  // Write the new book record to the file

    close(file_descriptor);  // Close the file

    get_all_books();  // Display all books in the database

    sem_post(&books_semaphore);  // Release access to the books database
}

// Function to send all books to the client
void send_books(int nsd)
{
    int file_descriptor,off,count;
    sem_wait(&books_semaphore);  // Wait for access to the books database

    file_descriptor = open("../db/books.bin", O_RDONLY, PERM);  // Open the books database file in read-only mode

    off = lseek(file_descriptor, 0, SEEK_END);  // Get the current offset (file size)

    count = -1;

    if (off > 0)
        count = (off / sizeof(struct Book));  // Calculate the number of books in the database
    else
        count = 0;

    printf("%d\n", count);

    write(nsd, &count, sizeof(int));  // Send the number of books to the client

    struct Book temp;

    for (int i = 0; i < count; i++)  // Loop through all books in the database
    {
        lseek(file_descriptor, i * sizeof(struct Book), SEEK_SET);  // Move the file pointer to the start of the current book record

        read(file_descriptor, &temp, sizeof(struct Book));  // Read the current book record from the file

        write(nsd, &temp, sizeof(struct Book));  // Send the current book record to the client
    }

    close(file_descriptor);  // Close the file

    sem_post(&books_semaphore);  // Release access to the books database
}

// Function to delete a book from the database
void delete_book(int nsd)
{
    int util_del, file_descriptor;
    sem_wait(&books_semaphore);  // Wait for access to the books database

    util_del = -1;

    read(nsd, &util_del, sizeof(int));  // Read the book ID to be deleted from client

    struct Book temp;

    file_descriptor = open("../db/books.bin", O_WRONLY, PERM);  // Open the books database file in write-only mode

    lseek(file_descriptor, (util_del - 1) * sizeof(struct Book), SEEK_SET);  // Move the file pointer to the start of the book record to be deleted

    read(file_descriptor, &temp, sizeof(struct Book));  // Read the book record to be deleted from the file

    temp.valid = 0;  // Mark the book record as invalid

    strcpy(temp.title, "");  // Clear the book title

    strcpy(temp.author, "");  // Clear the book author

    temp.copies = -1;  // Set the number of copies to -1

    temp.id = -1;  // Set the book ID to -1

    lseek(file_descriptor, (util_del - 1) * sizeof(struct Book), SEEK_SET);  // Move the file pointer back to the start of the book record

    write(file_descriptor, &temp, sizeof(struct Book));  // Write the modified book record back to the file

    get_all_books();  // Display all books in the database

    sem_post(&books_semaphore);  // Release access to the books database
}



void modify_book(int nsd) { // Function to modify the number of copies of a book

    int u1_mod,u2_mod,file_descriptor;

    sem_wait(&books_semaphore);

    // Read the book ID and the new number of copies from the client
    u1_mod = -1;
    u2_mod = -1;

    read(nsd, &u1_mod, sizeof(int));
    read(nsd, &u2_mod, sizeof(int));

    // Open the books file
    struct Book temp;

    file_descriptor = open("../db/books.bin", O_RDWR, PERM);

    // Read the book information
    lseek(file_descriptor, (u1_mod - 1) * sizeof(struct Book), SEEK_SET);

    read(file_descriptor, &temp, sizeof(struct Book));

    
    temp.copies = u2_mod; // Update the number of copies

    // Write back the updated information
    lseek(file_descriptor, (u1_mod - 1) * sizeof(struct Book), SEEK_SET);

    write(file_descriptor, &temp, sizeof(struct Book));

    
    get_all_books(); // Notify all clients about the change

 
    close(file_descriptor);    // Close the file and release the semaphore

    sem_post(&books_semaphore);
}


void add_user(int nsd){ // Function to add a new user

    int file_descriptor;

    sem_wait(&users_semaphore);

    // Read the new user information from the client
    struct User u;

    read(nsd, &u, sizeof(struct User));

    
    file_descriptor = open("../db/users.bin", O_CREAT | O_RDWR, PERM); // Open the users file

    // Add the new user to the file
    lseek(file_descriptor, 0, SEEK_END);

    write(file_descriptor, &u, sizeof(u));

    
    close(file_descriptor); // Close the file and release the semaphore

    sem_post(&users_semaphore);
}


void send_users(int nsd) { // Function to send all users to a client

    int file_descriptor,count;

    sem_wait(&users_semaphore);

    
    file_descriptor = open("../db/users.bin", O_RDWR, PERM); // Open the users file

    
    count = lseek(file_descriptor, 0, SEEK_END) / sizeof(struct User); // Calculate the number of users in the file

    write(nsd, &count, sizeof(int)); // Send the count to the client

    
    struct User temp; // Send each user's information to the client

    for (int i = 0; i < count; i++) {

        lseek(file_descriptor, i * sizeof(struct User), SEEK_SET);

        read(file_descriptor, &temp, sizeof(struct User));

        write(nsd, &temp, sizeof(struct User));
    }

    
    close(file_descriptor); // Close the file and release the semaphore

    sem_post(&users_semaphore);
}


void get_all_issues() { // Function to display all issued books

    int file_descriptor;

    struct issue temp;

    file_descriptor = open("../db/issues.bin", O_RDONLY, PERM);

    
    read(file_descriptor, &temp, sizeof(struct issue)); // Read the first issue from the file

    printf("Bookid: %d\n", temp.bookid);
    printf("Username: %s\n", temp.username);
    printf("Valid: %d\n", temp.valid);
    printf("\n");

   
    while (read(file_descriptor, &temp, sizeof(struct issue))) {  // Read and print all remaining issues

        printf("Bookid: %d\n", temp.bookid);
        printf("Username: %s\n", temp.username);
        printf("Valid: %d\n", temp.valid);
        printf("\n");
    }

    
    close(file_descriptor); // Close the file
}


void issue_book(int nsd) { // Function to allow a user to issue a book

    int file_descriptor;

    sem_wait(&issues_semaphore);
    sem_wait(&books_semaphore);

    // Read the issue information from the client
    struct issue temp;

    read(nsd, &temp, sizeof(struct issue));

    
    struct Book newbook; // Open the books file

    file_descriptor = open("../db/books.bin", O_RDWR, PERM);

    // Read the book information
    lseek(file_descriptor, (temp.bookid - 1) * sizeof(struct Book), SEEK_SET);

    read(file_descriptor, &newbook, sizeof(struct Book));

    
    if (newbook.copies <= 0 || newbook.valid == 0) { // Check if the book is available for issue

        
        int sts = -1; // Notify the client that the issue failed

        write(nsd, &sts, sizeof(int));

        // Release the semaphores and return
        sem_post(&issues_semaphore);
        sem_post(&books_semaphore);

        return;
    }

    
    newbook.copies = newbook.copies - 1; // Update the number of copies

    // Write back the updated information
    lseek(file_descriptor, (temp.bookid - 1) * sizeof(struct Book), SEEK_SET);

    write(file_descriptor, &newbook, sizeof(struct Book));

    close(file_descriptor); // Close the books file

    // Open the issues file
    file_descriptor = open("../db/issues.bin", O_WRONLY | O_CREAT, PERM);

    // Add the new issue to the file
    lseek(file_descriptor, 0, SEEK_END);

    write(file_descriptor, &temp, sizeof(struct issue));

    close(file_descriptor);

    // Notify the client that the issue was successful
    int sts = 1;
    write(nsd, &sts, sizeof(int));

    
    get_all_issues(); // Notify all clients about the change

    // Release the semaphores
    sem_post(&issues_semaphore);
    sem_post(&books_semaphore);
}

void return_book(int nsd)
{

    int file_descriptor;

    // Wait for access to the issues and books semaphores
    sem_wait(&issues_semaphore);
    sem_wait(&books_semaphore);

    // Read the return information from the client
    struct issue temp;
    read(nsd, &temp, sizeof(struct issue));

    temp.valid = false;

    bool flg = false;

    
    struct issue util_issue; // Open the issues file

    file_descriptor = open("../db/issues.bin", O_RDWR, PERM);
    lseek(file_descriptor, 0, SEEK_SET);

    
    while (read(file_descriptor, &util_issue, sizeof(struct issue)) > 0) // Iterate through all issues to find the one to be returned
    {
        // Print the book ID and username (for debugging)
        printf("%d", util_issue.bookid);
        printf(" %s\n", util_issue.username);

        // Check if the current issue matches the one to be returned
        if (util_issue.bookid == temp.bookid && strcmp(temp.username, util_issue.username) == 0 && util_issue.valid != 0)
        {
            
            flg = true; // Mark the issue as returned

            lseek(file_descriptor, -sizeof(struct issue), SEEK_CUR);
            write(file_descriptor, &temp, sizeof(struct issue));

            break;
        }
    }

    
    if (!flg) // If the issue was not found, notify the client and return
    {
        int sts = -1;
        write(nsd, &sts, sizeof(int));
        
        sem_post(&issues_semaphore);
        sem_post(&books_semaphore);

        return;
    }

    // Open the books file to update the number of copies
    struct Book newbook;

    file_descriptor = open("../db/books.bin", O_RDWR, PERM);
    lseek(file_descriptor, (temp.bookid - 1) * sizeof(struct Book), SEEK_SET);

    read(file_descriptor, &newbook, sizeof(struct Book));

    
    if (newbook.valid == 0) // Check if the book is valid
    {
        // Notify the client if the book is not valid
        int sts = -1;
        write(nsd, &sts, sizeof(int));
        
        sem_post(&issues_semaphore);
        sem_post(&books_semaphore);

        return;
    }

    // Increase the number of copies and write back to the file
    newbook.copies = newbook.copies + 1;

    lseek(file_descriptor, (temp.bookid - 1) * sizeof(struct Book), SEEK_SET);
    write(file_descriptor, &newbook, sizeof(struct Book));

    close(file_descriptor);

    // Notify the client that the return was successful
    int sts = 1;
    write(nsd, &sts, sizeof(int));

    // Release the semaphores
    sem_post(&issues_semaphore);
    sem_post(&books_semaphore);
    

}

void send_issues(int nsd)
{

    int file_descriptor,count;
    
    sem_wait(&issues_semaphore); // Wait for access to the issues semaphore

    // Open the issues file
    struct issue temp;
    file_descriptor = open("../db/issues.bin", O_RDWR, PERM);
    
    // Calculate the number of issues in the file
    count = lseek(file_descriptor, 0, SEEK_END) / sizeof(struct issue);
    write(nsd, &count, sizeof(int));

    // Send each issue's information to the client
    for (int i = 0; i < count; i++)
    {
        lseek(file_descriptor, i * sizeof(struct issue), SEEK_SET);
        read(file_descriptor, &temp, sizeof(struct issue));

        write(nsd, &temp, sizeof(struct issue));
    }
    close(file_descriptor);

   
    sem_post(&issues_semaphore);  // Release the issues semaphore
}

void admin_mode(int nsd)
{
   
    while (1)  // Loop for handling admin commands
    {
        
        int chc; // Read the choice from the client
        read(nsd, &chc, sizeof(chc));
        printf("Choice: %d\n", chc);

        // Process the choice
        switch (chc)
        {
        case 1:
            add_book(nsd);
            break;
        case 2:
            delete_book(nsd);
            break;
        case 3:
            modify_book(nsd);
            break;
        case 4:
            add_user(nsd);
            break;
        case 5:
            send_books(nsd);
            break;
        case 6:
            send_users(nsd);
            break;
        default:
            // Exit the loop if choice is invalid
            return;
        }
    }
}

void user_mode(int nsd)
{
    
    while (1) // Loop for handling user commands
    {
        
        int chc; // Read the choice from the client
        read(nsd, &chc, sizeof(chc));
        printf("Choice: %d\n", chc);

        // Process the choice
        switch (chc)
        {
        case 1:
            send_books(nsd);
            break;
        case 2:
            issue_book(nsd);
            break;
        case 3:
            return_book(nsd);
            break;
        case 4:
            send_issues(nsd);
            break;
        default:
            // Exit the loop if choice is invalid
            return;
        }
    }
}
