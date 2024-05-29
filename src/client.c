#include "../header/head.h"


bool handleAdmin(int sock_desc)
{
    // Display admin options
    printf("How can I help you!!\n");
    printf("\n");

    printf("Choose option:\n");
    printf("1. Add new book.\n");
    printf("2. Delete book.\n");
    printf("3. Modify book.\n");
    printf("4. Add new user.\n");
    printf("5. Show all available books\n");
    printf("6. Show all users\n");
    printf("7. Logout \n");

    
    int option; // Read admin choice
    scanf("%d", &option);

    write(sock_desc, &option, sizeof(int));

    if (option == 1)
    {   
        // Add new book
        // Read book details from user
        // Send book details to server
        struct Book booki;

        printf("Input name of Book: ");
        scanf("%s", booki.title);

        printf("Input name of Author: ");
        scanf("%s", booki.author);

        printf("Input no. of copies available: ");
        scanf("%d", &booki.copies);


        booki.valid = true;

        booki.id = -1;

        write(sock_desc, &booki, sizeof(booki));
    }
    else if (option == 2)
    {
        // Delete book
        // Read book ID from user
        // Send book ID to server
        printf("Input ID: ");

        int id = -1;
        scanf("%d", &id);

        write(sock_desc, &id, sizeof(int));
    }
    else if (option == 3)
    {
        // Modify book
        // Read book ID and new number of copies from user
        // Send book ID and new number of copies to server
        printf("Input ID: ");
        int id = -1;
        scanf("%d", &id);

        printf("Input new number of copies: ");
        int copi_num = -1;
        scanf("%d", &copi_num);

        write(sock_desc, &id, sizeof(int));
        write(sock_desc, &copi_num, sizeof(int));
    }
    else if (option == 4)
    {
        // Add new user
        // Read user details from user
        // Send user details to server
        struct User useri;

        printf("Input Username: ");
        scanf("%s", useri.username);

        printf("Input Password: ");
        scanf("%s", useri.password);

        useri.type = 1;

        write(sock_desc, &useri, sizeof(struct User));
    }
    else if (option == 5)
    {
        // Show all available books
        // Receive book details from server and print
        int count = 0;

        read(sock_desc, &count, sizeof(int));

        struct Book temp;

        for (int i = 0; i < count; i++)
        {
            read(sock_desc, &temp, sizeof(struct Book));

            if (!temp.valid)
                continue;

            printf("Title: %s\n", temp.title);
            printf("Author: %s\n", temp.author);
            printf("Copies: %d\n", temp.copies);
            printf("Valid: %d\n", temp.valid);
            printf("ID: %d\n", temp.id);

            printf("\n");
        }
    }
    else if (option == 6)
    {
        // Show all users
        // Receive user details from server and print
        int count = 0;

        read(sock_desc, &count, sizeof(int));

        printf("%d\n", count);

        struct User temp;

        for (int i = 0; i < count; i++)
        {
            read(sock_desc, &temp, sizeof(struct User));

            printf("Username: %s\n", temp.username);
            printf("Role: %s\n", temp.type ? "User" : "Admin");
        }
    }
    else{  // Logout
       
        printf("\n");
        printf("----------Please visit again.----------\n");
        printf("\n");

        return false;
    }
        
    return true;
}

bool handleUser(int sock_desc, char *username)
{
    // Display user options
    printf("How can I help you\n");

    printf("Choose option:\n");
    printf("1. Check all available books.\n");
    printf("2. Issue book \n");
    printf("3. Return book.\n");
    printf("4. Check all books issued by %s\n", username);
    printf("5. Logout \n");

    
    int option; // Read user choice
    scanf("%d", &option);

    write(sock_desc, &option, sizeof(int));

    if (option == 1)
    {
        // Check all available books
        // Receive book details from server and print
        printf("Quote: Books are for use!!\n");

        int count = 0;

        read(sock_desc, &count, sizeof(int));

        struct Book temp;

        for (int i = 0; i < count; i++)
        {
            read(sock_desc, &temp, sizeof(struct Book));

            if (!temp.valid || temp.copies == 0)
                continue;

            printf("Title: %s\n", temp.title);
            printf("Author: %s\n", temp.author);
            printf("Copies: %d\n", temp.copies);
            printf("Valid: %d\n", temp.valid);
            printf("ID: %d\n", temp.id);

            printf("\n");
        }
    }
    else if (option == 2)
    {
        // Issue book
        // Read book ID from user
        // Send book ID to server
        // Receive status from server and print
        
        printf("Quote: Every book has its reader!!\n");

        int id_book=-1;
        printf("Enter book id: ");
        scanf("%d",&id_book);

        struct issue is;
        is.bookid = id_book;
        is.valid = true;
        strcpy(is.username,username);
        

        write(sock_desc,&is,sizeof(struct issue));   

        int stat=0;
        read(sock_desc,&stat,sizeof(int));

        if(stat == -1) printf("Book not available\n");
        else printf("Book issued\n");  
    }
    else if (option == 3)
    {
        // Return book
        // Read book ID from user
        // Send book ID to server
        // Receive status from server and print
        int id_book=-1;

        printf("Enter book id of the book you want to return: ");
        scanf("%d",&id_book);

        struct issue issue_temp;
        issue_temp.bookid = id_book;
        issue_temp.valid = false;
        strcpy(issue_temp.username,username);
        

        write(sock_desc,&issue_temp,sizeof(struct issue)); 

        int sts=0;
        read(sock_desc,&sts,sizeof(int));

        if(sts == -1) printf("Book could not be returned\n");
        else printf("Book returned Succesfully\n");  
    }
    else if (option == 4)
    {
        // Check all books issued by user
        // Receive issue details from server and print
        int cnt = 0;
        read(sock_desc,&cnt,sizeof(int));

        struct issue issue_temp;

        for(int i=0;i<cnt;i++){
            read(sock_desc,&issue_temp,sizeof(struct issue));

            if(issue_temp.valid && !strcmp(issue_temp.username,username)){

                printf("Username of borrower: %s\n",issue_temp.username);
                printf("Id of Book issued: %d\n",issue_temp.bookid);
            }
        }
    }
    else{ // Logout
        
        printf("\n");
        printf("----------Please visit again.----------\n");
        printf("\n");

        return false;
    }
        
    return true;
}

int main()
{
    // Print welcome message
    printf("\n");
    printf("--------------------Welcome to IIIT Bangalore Library--------------------\n");

    // Create a socket
    struct sockaddr_in server;
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    
    int sts = connect(sock_desc, (struct sockaddr *)&server, sizeof(server)); // Connect to server

    if (sts < 0)
    {
        perror("Connect");
        return 0;
    }

    // Read welcome message from server and print
    char buf[BUFSIZ];
    read(sock_desc, buf, BUFSIZ);

    printf("%s\n", buf);

    // Perform authentication
    struct User temp;

    printf("Input Username: ");
    scanf("%s", temp.username);
    printf("Input Password: ");
    scanf("%s", temp.password);

    temp.type = 0;

    int stat_au = 0;

    // Send user credentials to server for authentication
    write(sock_desc, &temp, sizeof(temp));

    read(sock_desc, &stat_au, sizeof(int));

    
    if (stat_au == 1)// Handle authentication status
    {
        
        printf("Logged in as user %s\n", temp.username); // User authenticated

        
        bool md = true;// Enter user mode
        while (md)
        {
            md = handleUser(sock_desc, temp.username);
        }
    }
    else if (stat_au == 0)
    {
        
        printf("\nLogged in as Admin\n\n"); // Admin authenticated

        
        bool md = true; // Enter admin mode
        while (md)
        {
            md = handleAdmin(sock_desc);
        }
    }
    else
    {
        
        printf("Authentication Failed\n"); // Authentication failed
        return -1;
    }

    return 0;
}
