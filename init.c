#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>

int main()
{
        int fd1;
        struct {
            int id;
            char db_name[100];
            char db_username[100];
            char db_password[100];
            int active;
        } admin, customer, employee,manager;

        struct {
            int cid;
            int balance;
            int loan;
            int eid;
            char status[50];
        } account;


        fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/admin.db", O_RDWR | O_CREAT,0744);

        admin.id=1;
        strcpy(admin.db_name,"Admin");
        strcpy(admin.db_username,"admin");
        strcpy(admin.db_password,"admin123");
        admin.active=1;
        write(fd1,&admin, sizeof(admin));
        lseek(fd1,sizeof(admin),SEEK_CUR);
        
        close(fd1);

        fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/employee.db", O_RDWR | O_CREAT,0744);

        employee.id=1;
        strcpy(employee.db_name,"Asmita");
        strcpy(employee.db_username,"asmita");
        strcpy(employee.db_password,"asmita");
        employee.active=1;
        write(fd1,&employee, sizeof(employee));
        //lseek(fd1,sizeof(employee),SEEK_CUR);

        employee.id=2;
        strcpy(employee.db_name,"Jass");
        strcpy(employee.db_username,"jass");
        strcpy(employee.db_password,"jass");
        employee.active=1;
        write(fd1,&employee, sizeof(employee));
        lseek(fd1,sizeof(employee),SEEK_CUR);

        close(fd1);

        fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/customers.db", O_RDWR | O_CREAT,0744);

        customer.id=1;
        strcpy(customer.db_name,"Yash");
        strcpy(customer.db_username,"yash");
        strcpy(customer.db_password,"yash");
        customer.active=1;
        write(fd1,&customer, sizeof(customer));

        customer.id=2;
        strcpy(customer.db_name,"Riya");
        strcpy(customer.db_username,"riya");
        strcpy(customer.db_password,"riya");
        customer.active=1;
        write(fd1,&customer, sizeof(customer));
        close(fd1);

        fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/accounts.db", O_RDWR | O_CREAT,0744);

        account.cid=1;
        account.balance=2000;
        account.loan=0;
        account.eid=0;
        strcpy(account.status,"NULL");
        write(fd1,&account, sizeof(account));

        account.cid=2;
        account.balance=4000;
        account.loan=100000;
        account.eid=1;
        strcpy(account.status,"NA");
        write(fd1,&account, sizeof(account));
        close(fd1);

        fd1 = open("/home/asmita/linux_bash/minipro/SS-LAB-Demo-Project-CS-513/db/manager.db", O_RDWR | O_CREAT,0744);

        manager.id=1;
        strcpy(manager.db_name,"Vikram");
        strcpy(manager.db_username,"vik");
        strcpy(manager.db_password,"vik");
        manager.active=1;
        write(fd1,&manager, sizeof(manager));

        close(fd1);
        return 0;
}