#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

int fd1,fd,fd2;
int j;

char servers_message[2000], clients_message[2000];

int view_all_employees(int client_sock)
{
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDONLY,0744);

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

int view_all_customersM(int client_sock)
{
    int status=0;
    fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/account.db", O_RDONLY,0744);
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/customers.db", O_RDONLY,0744);

    memset(server_message, '\0', sizeof(server_message));
    i = read(fd1,&customer,sizeof(customer));
    while(i)
    {
            
            strcat(server_message," ID: ");
            sprintf(server_message, "%d", customer.id);
            strcat(server_message," Name: ");
            strcat(server_message,customer.db_name);
            strcat(server_message," Username: ");
            strcat(server_message,customer.db_username);
            strcat(server_message," Loan Amount: ");
            sprintf(server_message, "%d", account.loan);
            
            strcat(server_message,"\n");
            send(client_sock, server_message, strlen(server_message), 0);
            i = read(fd1,&customer,sizeof(customer));

    }

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    strcpy(server_message, " Above are all customers! Press # to exit :");
    send(client_sock, server_message, strlen(server_message), 0);
    recv(client_sock, client_message, sizeof(client_message), 0);  
    
    close(fd1);
    return 1;
}





int assign_employee(int client_sock){
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);
    //fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDWR,0744);
    
    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter customer id: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int cust=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter employee id: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int emp=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    i = read(fd1,&account,sizeof(account));
    while(i)
    {
            if(cust==account.cid)
            {
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s Emp Id : %d\n ",account.cid, account.balance, account.loan, account.status, account.eid);
                account.eid = emp;
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s Emp Id : %d\n ",account.cid, account.balance, account.loan, account.status, account.eid);
                strcpy(account.status,"Pending");
                lseek(fd1,-1*sizeof(account),SEEK_CUR);
                i = write(fd1,&account, sizeof(account));
                strcpy(servers_message," Verifier assigned.  Press # to exit :");
                send(client_sock, servers_message, strlen(servers_message), 0);
                recv(client_sock, clients_message, sizeof(clients_message), 0); 
                status = 1;
            }      
            i = read(fd1,&account,sizeof(account));
    }
    if(status!=1)
    {
        strcpy(servers_message, "Some error occured ! ");
        send(client_sock, servers_message, strlen(servers_message), 0);
    }

    close(fd1);
    return 1;
}