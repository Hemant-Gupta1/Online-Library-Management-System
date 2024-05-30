# Online-Library-Management-System (OLMS)

# GitHub Link
https://github.com/Hemant-Gupta1/Online-Library-Management-System

# Description

In the digital age, efficient management systems are crucial for the smooth functioning of libraries. My project aims to design and develop an Online Library Management System (OLMS) that provides essential library functionalities while ensuring data security and concurrency control. The system leverages socket programming to enable multiple clients to access the library database concurrently and utilizes system calls for process management, file handling, file locking, multithreading, and interprocess communication to optimize performance and resource utilization.

# Objectives
Implement secure user authentication to ensure safe access to member accounts.
Provide password-protected administrative access to prevent unauthorized usage.
Develop functionalities for adding, deleting, modifying and searching member details.
Implement file-locking mechanisms using system calls to ensure data consistency and concurrency control.
Utilize socket programming to allow concurrent access to the library database by multiple clients.

# Functionalities
User Authentication: Members must log in to access their accounts, ensuring data privacy and security.
Administrative Access: Password-protected access for librarians to manage book transactions and member information.
Book Management: Administrators can add, delete, modify to manage library resources efficiently.
File-Locking Mechanisms: System calls are used to implement file-locking mechanisms, protecting critical data sections and ensuring data consistency.
Concurrent Access: Socket programming is employed to allow multiple clients to access library resources simultaneously.

# Implementation Details

I have implemented the OLMS using system calls for operations such as process management, file handling, file locking, multithreading, and interprocess communication. Socket programming is used for client-server communication, enabling multiple clients to access the library database concurrently.

# Instructions to Run
1. In /Online-Library-Management-System: make clean
2. In /Online-Library-Management-System: make all
3. cd bin
4. ./create_admin and proceed as per prompts
5. ./server
6. ./client and proceed as per prompts

