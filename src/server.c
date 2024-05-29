#include "../header/server.h" // Include server header file
#include "../header/head.h"   // Include head header file



sem_t issues_semaphore; // Semaphore for issues
sem_t books_semaphore;  // Semaphore for books
sem_t users_semaphore;  // Semaphore for users

#define PERM 0666 // Permissions for file access
#define LISTEN 50 //other macros


// Function to handle client threads
void *handleThread(void *arguments)
{
    int file_descriptor,flg,util_check;

    int util = *((int *)arguments);                                        // Get the client socket descriptor
    write(util, "Connected to server\n", sizeof("Connected to server\n")); // Notify client of connection

    struct User user_temp;                       // Temporary user structure
    read(util, &user_temp, sizeof(struct User)); // Read user information from client

    printf("Username: %s\n", user_temp.username); // Print username
    printf("Password: %s\n", user_temp.password); // Print password

    struct flock lock; // File locking structure

    lock.l_whence = SEEK_SET; // Start of file
    lock.l_type = F_RDLCK;    // Read lock
    lock.l_len = 0;        // Lock entire file
    lock.l_start = 0;      // Start from beginning
    lock.l_pid = getpid(); // Process ID of the caller


    file_descriptor = open("../db/users.bin", O_RDONLY, PERM); // Open user database file
    fcntl(file_descriptor, F_SETLKW, &lock);                       // Apply read lock to the file

    struct User u; // User structure for comparison

    flg = 0;   // Flag to indicate if user is found

    while (read(file_descriptor, &u, sizeof(u)))
    { // Read user data from file
        if (strcmp(u.username, user_temp.username) == 0)
        {            // Check if username matches
            flg = 1; // Set flag to indicate user found
            break;   // Exit loop
        }
    }

    lock.l_type = F_UNLCK;                   // Release lock

    fcntl(file_descriptor, F_SETLKW, &lock); // Release file lock

    if (strcmp(u.password, user_temp.password) || !flg)
        util_check = -1; // Check password and flag
    else
        util_check = u.type; // Set utility check to user type


    write(util, &util_check, sizeof(int)); // Send utility check to client

    if (util_check == 0)
        admin_mode(util); // If admin, enter admin mode
    else if (util_check == 1)
        user_mode(util); // If user, enter user mode


    return NULL; // Return null
}

int main()
{
    
    int sock_desc;                                      // Socket descriptor

    int util;                                    // Utility variable

    sem_init(&books_semaphore, 0, 1);  // Initialize books semaphore
    sem_init(&issues_semaphore, 0, 1); // Initialize issues semaphore
    sem_init(&users_semaphore, 0, 1);  // Initialize users semaphore


    socklen_t length;                            // Length variable for sockaddr
    struct sockaddr_in server_main, client_main; // Server and client sockaddr structures
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);        // Create socket

    bzero(&server_main, sizeof(server_main));    // Zero out server structure

    server_main.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available address
    server_main.sin_family = AF_INET;                // Use IPv4
    server_main.sin_port = htons(PORT);              // Use specified port

    bind(sock_desc, (struct sockaddr *)&server_main, sizeof(server_main)); // Bind socket to address

    listen(sock_desc, LISTEN); // Listen for incoming connections

    printf("Server listening on %d\n", PORT); // Print server listening message

    while (1)
    {                                                                // Infinite loop for accepting connections
        length = sizeof(client_main);                                // Get client length

        util = accept(sock_desc, (struct sockaddr *)&client_main, &length); // Accept incoming connection

        pthread_t tid;                                               // Thread ID
        pthread_create(&tid, NULL, handleThread, (void *)&util);     // Create new thread to handle client

    }

    
    sem_destroy(&issues_semaphore); // Destroy issues semaphore
    sem_destroy(&books_semaphore);  // Destroy books semaphore
    sem_destroy(&users_semaphore);  // Destroy users semaphore

    return 0; // Return 0
}
