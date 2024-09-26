/*
ID : MT2023148
Name : Asmita Tripathi
Description : The server program running on port 8443
*/

#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "admin_action.h"
#include "customer_action.h"
#include "manager_action.h"

char server_message[2000], client_message[2000];

void clearMemory(){
    memset(server_message,0,sizeof(server_message));
    memset(client_message,0, sizeof(client_message));
}
int session_cid;
int *user_exist;

char* login(int role,int client_sock, int *user_exist)
{
    int login_fd;
    clearMemory();
    
    //get username for role
    strcpy(server_message, username_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(username,client_message);

    clearMemory();
    //get password for role
    strcpy(server_message, password_prompt);
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    strcpy(password,client_message);
    printf("Username : %s and password : %s \n", username, password);

    switch(role)
    {
        case 1: login_fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/admin.db", O_RDWR);
                i = read(login_fd,&user,sizeof(user));
                printf("\n Username : %s , Password : %s \n", user.db_username, user.db_password);
                while(i)
                {
                        printf("\n Id : %d , Username : %s , Password : %s , isActive : %d", user.id, user.db_username, user.db_password, user.active);
                        if(strcmp(username ,user.db_username)==0 && strcmp(password,user.db_password)==0)
                        {
                            *user_exist = 1;
                            return "valid";
                        }
                        lseek(login_fd,sizeof(user),SEEK_CUR);
                        i = read(login_fd,&user,sizeof(user));
                }
                close(login_fd);
                break;
        case 2: login_fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/customers.db", O_RDWR);
                i = read(login_fd,&user,sizeof(user));
                while(i)
                {
                        printf("\n Id : %d , Username : %s , Password : %s , isActive : %d\n ", user.id, user.db_username, user.db_password, user.active);
                        if(strcmp(username ,user.db_username)==0 && strcmp(password,user.db_password)==0)
                        {
                            session_cid=user.id;
                            *user_exist = 1;
                            return "valid";
                        }
                        lseek(login_fd,sizeof(user),SEEK_CUR);
                        i = read(login_fd,&user,sizeof(user));
                }
                close(login_fd);
                break;
        case 3: login_fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDWR);
                i = read(login_fd,&user,sizeof(user));
                while(i)
                {
                        printf("\n Id : %d , Username : %s , Password : %s , isActive : %d", user.id, user.db_username, user.db_password, user.active);
                        if(strcmp(username ,user.db_username)==0 && strcmp(password,user.db_password)==0)
                        {
                            *user_exist = 1;
                            return "valid";
                        }
                        lseek(login_fd,sizeof(user),SEEK_CUR);
                        i = read(login_fd,&user,sizeof(user));
                }
                close(login_fd);
                break;
        case 4: login_fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/manager.db", O_RDWR);
                i = read(login_fd,&user,sizeof(user));
                while(i)
                {
                        printf("\n Id : %d , Username : %s , Password : %s , isActive : %d", user.id, user.db_username, user.db_password, user.active);
                        if(strcmp(username ,user.db_username)==0 && strcmp(password,user.db_password)==0)
                        {
                            *user_exist = 1;
                            return "valid";
                        }
                        lseek(login_fd,sizeof(user),SEEK_CUR);
                        i = read(login_fd,&user,sizeof(user));
                }
                close(login_fd);
                break;
    }
    
    return 0;

}
int main(void)
{
    int socket_fd, client_sock, client_size,login_fd;
    int user_exist,i,j;
    struct sockaddr_in server_addr, client_addr;
    char username[100], password[100], id[100];

    /* Set of messages*/
    char welcome_message[2000]= "\n-----------------Welcome-----------------\n Choose from below : \n 1. Admin\n 2. Customer\n 3. Employee\n 4.Manager\n Enter your choice :";
    char admin_menu[2000] = "\n------------Admin Menu--------------\n 1. Add Employee\n 2. View Employee\n 3. Add Customer\n 4. Modify customer\n 5. View all customers\n 6. Manage Roles\n 7. Logout and Exit\n Enter your choice :";
    char customer_menu[2000] = "\n----------Customer Menu-------------\n 1. View Account Balance\n 2. Deposit Money\n 3. Withdraw Money\n 4. Transfer Funds\n 5. Just\n 6. Apply for loan\n Enter your choice: ";
    char employee_menu[2000] = "\n----------Employee Menu-------------\n 1. Add New Customer\n 2. Modify Customer Details\n 3. Loan Applications\n 4. Update Loan Status\n 5. Logout and Exit\n Enter your choice: ";
    char manager_menu[2000] = "\n----------Manager Menu-------------\n 1. Assign Employee for Loan\n 2. View Customer\n 3. View Employee\n 4. Logout and Exit\n Enter your choice: ";
    char username_prompt[2000] = "\n Enter Username: ";
    char password_prompt[2000] = "\n Enter Password: ";
    char admin_pathname[100]="./admin_action";

    clearMemory();

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_fd< 0){
        printf("Error while creating socket\n");
        return -1;
    }

    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8440);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    

    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    

    if(listen(socket_fd, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    while(1){
    client_size = sizeof(client_addr);
    client_sock = accept(socket_fd, (struct sockaddr*)&client_addr, &client_size);
    
    
    if (!fork())
    {
        strcpy(server_message, welcome_message);
        
        if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
        }
        if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Error while receiving client's msg\n");
            return -1;
        }
        
        //printf("%s",client_message);
        char *p = client_message;
        int val = strtol(p, &p, 10); 
        int val1;
        switch(val)
        {
            case 1: 
                        strcpy(id,login(val,client_sock,&user_exist));
                        if (user_exist == 1)
                        {
                            do
                            {
                                clearMemory();
                                // send admin menu
                                strcpy(server_message, admin_menu);
                                if (send(client_sock, server_message, strlen(server_message), 0) < 0){
                                    printf("Can't send\n");
                                    return -1;
                                }
                                // receive action
                                if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
                                    printf("Error while receiving profile action\n");
                                    return -1;
                                }
                                char *p = client_message;
                                val1 = strtol(p, &p, 10); 
                                switch(val1)
                                {
                                    case 1:
                                            add_employee(client_sock);
                                            break;
                                    case 2:
                                            view_employee(client_sock);
                                            break;
                                    case 3:
                                            add_customer(client_sock);
                                            break;
                                    case 4:
                                            //modify_customer(client_sock);
                                            break;
                                    case 5:
                                            view_all_customers(client_sock);
                                            break;
                                    case 6:
                                            //block_customer(client_sock);
                                            break;
                                    case 7:
                                            val1=7;
                                            break;
                                }
                            }while(val1<7);
                        }
                        else
                        {
                            clearMemory();
                            strcpy(server_message, "Username or password is invalid. Press 0 to exit");
                            send(client_sock, server_message, strlen(server_message), 0);
                            recv(client_sock, client_message, sizeof(client_message), 0);
                        }
                        close(login_fd);
                        break;
            case 2:
                        strcpy(id,login(val,client_sock,&user_exist));
                        if (user_exist>0)
                        {
                            do
                            {
                                //send customer menu
                                clearMemory();
                                strcpy(server_message, customer_menu);
                                if (send(client_sock, server_message, strlen(server_message), 0) < 0){
                                    printf("Can't send\n");
                                    return -1;
                                }
                                //receive action 
                                if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
                                    printf("Error while receiving client's msg\n");
                                    return -1;
                                }
                                char *p = client_message;
                                val1 = strtol(p, &p, 10); 

                                switch(val1)
                                {
                                    case 1:
                                            view_balance(client_sock,session_cid);
                                            break;
                                    case 2: 
                                            deposit_money(client_sock,session_cid);
                                            break;
                                    case 3: 
                                            withdraw_money(client_sock,session_cid);
                                            break;
                                    case 4:
                                            money_transfer(client_sock,session_cid);
                                            break;
                                    case 5:
                                            //apply_for_loan(client_sock,session_cid);
                                            break;
                                    case 6:
                                            apply_for_loan(client_sock,session_cid);
                                            break;
                                    
                                }
                            }while(val1<6);
                        }
                        else
                        {
                            clearMemory();
                            strcpy(server_message, "Username or password is invalid. Press 0 to exit");
                            send(client_sock, server_message, strlen(server_message), 0);
                            recv(client_sock, client_message, sizeof(client_message), 0);
                        }
                        close(login_fd);
                        break;
            case 4:
                        strcpy(id,login(val,client_sock,&user_exist));
                        if (user_exist == 1)
                        {
                            do
                            {
                                //get manager menu
                                clearMemory();

                                strcpy(server_message, manager_menu);
                                if (send(client_sock, server_message, strlen(server_message), 0) < 0){
                                    printf("Can't send\n");
                                    return -1;
                                }
                                if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
                                        printf("Error while receiving profile action\n");
                                        return -1;
                                    }
                                    char *p = client_message;
                                    val1 = strtol(p, &p, 10); 
                                    switch(val1)
                                    {
                                        case 1:
                                                view_all_customersM(client_sock);
                                                break;
                                        case 2:
                                                view_all_employees(client_sock);
                                                break;
                                        case 3:
                                                assign_employee(client_sock);
                                                break;
                                        case 4:
                                                val1=4;
                                                break;
                                    }
                            }while(val1<4);
                        }
                        else
                        {
                            clearMemory();
                            strcpy(server_message, "Username or password is invalid. Press 0 to exit");
                            send(client_sock, server_message, strlen(server_message), 0);
                            recv(client_sock, client_message, sizeof(client_message), 0);
                        }
                        close(login_fd);
                        break;
        }

        // Closing the socket:
        close(client_sock);
        close(socket_fd);
        exit(0); // Terminate the child process
    }
    else
    {
        
        close(client_sock);		
        waitpid(-1, NULL, WNOHANG); 
    }
    }
    
    return 0;
}