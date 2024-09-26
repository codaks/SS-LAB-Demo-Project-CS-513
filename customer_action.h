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
struct {
    int cid;
    int balance;
    int loan;
    int eid;
    char status[50];
} account, raccount;

char servers_message[2000], clients_message[2000];

int view_balance(int client_sock,int session_cid){
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDONLY,0744);
    
    memset(servers_message, '\0', sizeof(servers_message));
    i = read(fd1,&account,sizeof(account));
    while(i)
    {
            if(session_cid==account.cid)
            {
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s \n",account.cid, account.balance, account.loan, account.status);
                
                strcat(servers_message," Balance: ");
                sprintf(servers_message, "%d", account.balance);
                strcat(servers_message," Loan: ");
                sprintf(servers_message, "%d", account.loan);
                strcat(servers_message," Press # to exit: ");
                printf("Sending : %s \n",servers_message);
                send(client_sock, servers_message, strlen(servers_message), 0);
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

int deposit_money(int client_sock,int session_cid){
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);
    
    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter amount: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int amount=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    //memset(servers_message, 0, sizeof(servers_message));
    i = read(fd1,&account,sizeof(account));
    while(i)
    {
            if(session_cid==account.cid)
            {
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s \n",account.cid, account.balance, account.loan, account.status);
                account.balance = account.balance + amount;
                lseek(fd1,-1*sizeof(account),SEEK_CUR);
                i = write(fd1,&account, sizeof(account));
                strcpy(servers_message," Balance updated :");
                sprintf(servers_message, "%d", account.balance);
                printf("Sending : %s \n",servers_message);
                send(client_sock, servers_message, strlen(servers_message), 0);
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

int withdraw_money(int client_sock,int session_cid){
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);
    
    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter amount: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int amount=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    //memset(servers_message, 0, sizeof(servers_message));
    i = read(fd1,&account,sizeof(account));
    while(i)
    {
            if(session_cid==account.cid)
            {
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s \n",account.cid, account.balance, account.loan, account.status);
                if(account.balance<amount){
                    strcpy(servers_message, "Insufficent funds ! ");
                    send(client_sock, servers_message, strlen(servers_message), 0);
                }
                else{
                    account.balance = account.balance - amount;
                    lseek(fd1,-1*sizeof(account),SEEK_CUR);
                    i = write(fd1,&account, sizeof(account));
                    strcpy(servers_message," Amount debited, balance updated :");
                    sprintf(servers_message, "%d", account.balance);
                    printf("Sending : %s \n",servers_message);
                    send(client_sock, servers_message, strlen(servers_message), 0);
                }
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

int money_transfer( int client_sock,int session_cid) {
    struct flock lock;
    
    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter account id where you want to transfer: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int rid=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter amount you want to transfer: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int amount=atoi(clients_message);

    
    fd = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);

    
    if (rid == session_cid) {
        strcpy(servers_message, "Self transfer not allowed! ");
        send(client_sock, servers_message, strlen(servers_message), 0);
        close(fd);
        return 0;
    }
    
    int id = -1;
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &raccount, sizeof(raccount)) > 0) {
        if (raccount.cid == rid) {
            id = raccount.cid;
            break;
        }
    }

    if (id == -1) {
        printf("Receiver not found\n");
        close(fd);
        return 0;
    }

    lseek(fd1, 0, SEEK_SET);
    
    while (read(fd1, &account, sizeof(account)) > 0) {
        if (account.cid == session_cid) {
            
            lock.l_type = F_WRLCK;
            lock.l_start = lseek(fd, -sizeof(account), SEEK_CUR);
            lock.l_whence = SEEK_SET;
            lock.l_len = sizeof(account);
            fcntl(fd1, F_SETLKW, &lock);

            int avail = account.balance;
            printf("Available balance is: %d\n", avail);
        
            if (amount > avail) {
                
                strcpy(servers_message, "Insufficent funds ! ");
                send(client_sock, servers_message, strlen(servers_message), 0);
                fcntl(fd1, F_UNLCK, &lock);
                close(fd1);
                return 0;
            }

            account.balance -= amount;

            lseek(fd1, -sizeof(account), SEEK_CUR);
            write(fd1, &account, sizeof(account));
            
            fcntl(fd1, F_UNLCK, &lock);

            lseek(fd, 0, SEEK_SET);
            while (read(fd, &raccount, sizeof(raccount)) > 0) {
                if (raccount.cid == rid) {
                    
                    lock.l_type = F_WRLCK;
                    lock.l_start = lseek(fd, -sizeof(raccount), SEEK_CUR);
                    lock.l_whence = SEEK_SET;
                    lock.l_len = sizeof(raccount);
                    fcntl(fd, F_SETLKW, &lock);
                    
                    raccount.balance += amount;
                    
                    lseek(fd, -sizeof(raccount), SEEK_CUR);
                    write(fd, &raccount, sizeof(raccount));
                    
                    fcntl(fd, F_UNLCK, &lock);

                    memset(servers_message, '\0', sizeof(servers_message));
                    
                    strcpy(servers_message, "Transaction successful ");
                    printf("Sender : %d Receiver: %d \n\n", raccount.balance, account.balance);
                    send(client_sock, servers_message, strlen(servers_message), 0);
                    //recv(client_sock, clients_message, sizeof(clients_message), 0);  
                    close(fd);
                    return 0;
                }
            }
        }
    }

    printf("Transaction unsuccessful\n");
    memset(servers_message, '\0', sizeof(servers_message));
    
    strcpy(servers_message, "Transaction unsuccessful ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    close(fd);
    return 0;
}

int apply_for_loan(int client_sock,int session_cid){
    int status=0;
    fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR,0744);
    //fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDWR,0744);
    
    memset(servers_message, '\0', sizeof(servers_message));
    memset(clients_message, '\0', sizeof(clients_message));
    strcpy(servers_message, "Enter amount: ");
    send(client_sock, servers_message, strlen(servers_message), 0);
    recv(client_sock, clients_message, sizeof(clients_message), 0);  
    int amount=atoi(clients_message);

    memset(servers_message, '\0', sizeof(servers_message));
    i = read(fd1,&account,sizeof(account));
    while(i)
    {
            if(session_cid==account.cid)
            {
                printf("\n Record Found for : %d Balance : %d Loan : %d Status : %s \n",account.cid, account.balance, account.loan, account.status);
                account.loan = account.loan + amount;
                account.eid = 1;
                strcpy(account.status,"NA");
                lseek(fd1,-1*sizeof(account),SEEK_CUR);
                i = write(fd1,&account, sizeof(account));
                strcpy(servers_message," Loan initiated. Press # to exit :");
                //sprintf(servers_message, "%d", account.loan);
                printf("Sending : %s \n",servers_message);
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