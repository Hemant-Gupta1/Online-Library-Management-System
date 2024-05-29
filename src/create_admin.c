#include "../header/head.h"

#define NAME_LEN 50
#define PERM 0666   

int main()
{

    char name_util[NAME_LEN]; // Buffer to store username and password
    
    memset(name_util, 0, sizeof(name_util)); // Initialize buffer to 0

    printf("Input username: "); // Prompt user to Input username
    
    scanf("%s", name_util); // Read username from user input

    struct User user_temp; // Temporary user struct to hold username, password, and type
    
    strcpy(user_temp.username, name_util); // Copy username to user struct

    printf("Input password: "); // Prompt user to Input password
    
    memset(name_util, 0, sizeof(name_util)); // Clear buffer

    scanf("%s", name_util); // Read password from user input
    
    strcpy(user_temp.password, name_util); // Copy password to user struct

    user_temp.type = 0; // Set user type 

    // Open users.bin file in read/write mode, create if not exist, with read/write permissions
    int file_descriptor = open("../db/users.bin", O_CREAT | O_RDWR, PERM);

    lseek(file_descriptor, 0, SEEK_END); // Move file pointer to the end of file
    
    write(file_descriptor, &user_temp, sizeof(user_temp)); // Write user struct to file
    
    close(file_descriptor); // Close file

    return 0;
}
