#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

int fd1;
int i;

struct {
    int id;
    char db_name[100];
    char db_username[100];
    char db_password[100];
    int active;
} admin, user,customer, employee,manager;

// struct {
//     int cid;
//     int balance;
//     int loan;
//     int eid;
//     char status[50];
// } account;
char username_prompt[100] = "\n Enter Username: ";
char password_prompt[100] = "\n Enter Password: ";
char name_prompt[100] = "\n Enter name : ";
char id_prompt[100]="\n Enter ID : ";
char server_message[2000], client_message[2000], username[100], password[100],id[5];

int add_employee(int client_sock)
{
    
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_APPEND|O_RDWR,0744);

    // get name for the employee
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, name_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(employee.db_name,client_message);

    // get ID for the employee
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, id_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    employee.id=atoi(client_message);

    // get username for the employee
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, username_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(employee.db_username,client_message);

    // get password for the employee
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, password_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(employee.db_password,client_message);

    employee.active=1;
    

    i = write(fd1,&employee, sizeof(employee));
    if(i<0)
    {
        perror("employee couldn't be added!");
    }
    strcpy(server_message, "employee Added Successfully! Press # to exit :");
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    lseek(fd1,0,SEEK_SET);
    i = read(fd1,&employee,sizeof(employee));
    while(i)
    {
            printf("\n Id: %d Name : %s Username : %s , Password : %s\n",employee.id, employee.db_name,employee.db_username, employee.db_password);
            i = read(fd1,&employee,sizeof(employee));
    }
    close(fd1);
    return 1;
}

int add_customer(int client_sock)
{
    
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/customers.db", O_APPEND|O_RDWR,0744);

    // get name for the customer
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, name_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(customer.db_name,client_message);

    // get ID for the customer
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, id_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    customer.id=atoi(client_message);

    // get username for the customer
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, username_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(customer.db_username,client_message);

    // get password for the customer
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, password_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(customer.db_password,client_message);

    customer.active=1;
    

    i = write(fd1,&customer, sizeof(customer));
    if(i<0)
    {
        perror("customer couldn't be added!");
    }
    strcpy(server_message, "customer Added Successfully! Press # to exit :");
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    lseek(fd1,0,SEEK_SET);
    i = read(fd1,&customer,sizeof(customer));
    while(i)
    {
            printf("\n Id: %d Name : %s Username : %s , Password : %s\n",customer.id, customer.db_name,customer.db_username, customer.db_password);
            i = read(fd1,&customer,sizeof(customer));
    }
    close(fd1);
    return 1;
}

int view_employee(int client_sock)
{
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDONLY,0744);
    // get ID for the employee
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, id_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    int id=atoi(client_message);

    memset(server_message, '\0', sizeof(server_message));
    i = read(fd1,&employee,sizeof(employee));
    while(i)
    {
            if(id==employee.id)
            {
                strcat(server_message,"employee record found !");
                strcat(server_message," ID: ");
                sprintf(server_message, "%d", employee.id);
                //strcat(server_message,itoa(employee.id));
                strcat(server_message," Name: ");
                strcat(server_message,employee.db_name);
                strcat(server_message," Username: ");
                strcat(server_message,employee.db_username);
                strcat(server_message," Status: ");
                if(employee.active==1)
                    strcat(server_message,"Active ");
                else
                    strcat(server_message,"Blocked ");
                 
                send(client_sock, server_message, strlen(server_message), 0);
                status = 1;
            }      
            i = read(fd1,&employee,sizeof(employee));
    }
    if(status!=1)
    {
        strcpy(server_message, "employee not found ! ");
        send(client_sock, server_message, strlen(server_message), 0);
    }

    close(fd1);
    return 1;
}

int view_all_customers(int client_sock)
{
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/customers.db", O_RDONLY,0744);

    memset(server_message, '\0', sizeof(server_message));
    i = read(fd1,&employee,sizeof(employee));
    while(i)
    {
            
            strcat(server_message," ID: ");
            sprintf(server_message, "%d", employee.id);
            strcat(server_message," Name: ");
            strcat(server_message,employee.db_name);
            strcat(server_message," Username: ");
            strcat(server_message,employee.db_username);
            strcat(server_message," Status: ");
            if(employee.active==1)
                strcat(server_message,"Active ");
            else
                strcat(server_message,"Blocked ");
            strcat(server_message,"\n");
            send(client_sock, server_message, strlen(server_message), 0);
            i = read(fd1,&employee,sizeof(employee));

    }

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, " Above are all customers! Press # to exit :");
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    
    close(fd1);
    return 1;
}



// int modify_student(int client_sock)
// {
//     int status=0;
//     fd1 = open("/home/asmita/linux_bash/miniproj/student_login.db", O_RDWR,0744);
//     // get ID for the employee
//     memset(server_message, '\0', sizeof(server_message));
//     memset(client_message, '\0', sizeof(client_message));
//     strcpy(server_message, "Enter the ID to modify : ");
//     send(client_sock, server_message, strlen(server_message), 0);
//     recv(client_sock, client_message, sizeof(client_message), 0);  
//     strcpy(id,client_message);

//     memset(server_message, '\0', sizeof(server_message));
//     i = read(fd1,&employee,sizeof(employee));
//     while(i)
//     {
//             if(strcmp(id,employee.db_ID)==0)
//             {
//                 memset(server_message, '\0', sizeof(server_message));
//                 memset(client_message, '\0', sizeof(client_message));
//                 strcat(server_message,employee.db_name);
//                 strcat(server_message," is the current name, the modified name is :  ");
//                 send(client_sock, server_message, strlen(server_message), 0);
//                 recv(client_sock, client_message, sizeof(client_message), 0);  
//                 strcpy(employee.db_name,client_message);

//                 memset(server_message, '\0', sizeof(server_message));
//                 memset(client_message, '\0', sizeof(client_message));
//                 strcat(server_message,employee.db_phone);
//                 strcat(server_message," is the current phone num, the modified number is :  ");
//                 send(client_sock, server_message, strlen(server_message), 0);
//                 recv(client_sock, client_message, sizeof(client_message), 0);  
//                 strcpy(employee.db_phone,client_message);

//                 status = 1;
//                 lseek(fd1,-1*sizeof(employee),SEEK_CUR);
//                 i = write(fd1,&employee, sizeof(employee));
//                 strcpy(server_message, "employee Record Modified! Press # to exit : ");
                
//             }      
//             i = read(fd1,&employee,sizeof(employee));
//     }
//     if(status!=1)
//     {
//         strcpy(server_message, "employee not found ! ");
//         send(client_sock, server_message, strlen(server_message), 0);
//     }
//     close(fd1);
//     return 1;
// }


