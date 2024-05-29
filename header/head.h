#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/poll.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/ipc.h>
#include<stdbool.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<pthread.h>
#include<sched.h>
#include<netinet/in.h>
#include<stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

#define PORT 6000
#define LENGTH 50


// Structure to hold user information
struct User{
    
    char password [LENGTH];
    char username [LENGTH];
    int type;

};


// Structure to hold book information
struct Book{
    
    char author[LENGTH];
    char title[LENGTH];
    int copies;
    bool valid;
    int id;

};



// Structure to hold book issuing information
struct issue{
    
    int bookid;
    char username[LENGTH];
    bool valid;

};