#include<semaphore.h>

// Function declarations
void admin_mode(int);
void *handleThread(void *);
void user_mode(int);

// External declarations of semaphores
extern sem_t issues_semaphore;
extern sem_t books_semaphore;
extern sem_t users_semaphore;
