typedef struct {
    int customer_id;
    int account_id;
    int employee_id;
    int loan_amount;
    char processing_status[20];  
    char loan_status[20];        
} loan_t;

loan_t loan_db[100];
int loan_count = 0; 

void load_loan_db() {
    FILE *fp = fopen("db/loans.db", "r");
    if (fp < 0) {
        perror("open");
        exit(1);
    }
    char line[1024];
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
    
        sscanf(line, "%d %d %d %d %s %s\n", &loan_db[i].customer_id, &loan_db[i].account_id, 
               &loan_db[i].employee_id, &loan_db[i].loan_amount, loan_db[i].processing_status, 
               loan_db[i].loan_status);
        i++;
    }
    loan_count = i; 
    fclose(fp);
}

void view_loans_by_status(int client_socket, int employee_id, const char *status) {
    char response[4096] = {0};  
    int loans_found = 0;
    char heading[100];
    snprintf(heading, sizeof(heading), "Loan applications with status - %s\n", status);         
    strcat(response, heading);
    for (int i = 0; i < loan_count; i++) {
        loan_t loan = loan_db[i];  

        if (loan.employee_id == employee_id && strcmp(loan.processing_status, status) == 0) {
            loans_found = 1;  

            char loan_info[256];
            snprintf(loan_info, sizeof(loan_info), "Customer ID: %d, Account ID: %d, Loan Amount: %d, Status: %s\n",
                     loan.customer_id, loan.account_id, loan.loan_amount, loan.loan_status);
            strcat(response, loan_info);
        }
    }

    if (!loans_found) {
        strcpy(response, "No loans found with the specified status.\n");
    }

    send(client_socket, response, sizeof(response), 0);
}

void process_loan_application(int client_socket, int employee_id) {
    char buffer[256], response[256];
    int account_id;
    char decision[1]; 
    memset(decision, 0, sizeof(decision));
    view_loans_by_status(client_socket, employee_id, "under_process");
    // Ask for the account ID
    strcpy(response, "Enter the Account ID of the loan to approve/reject: ");
    send(client_socket, response, sizeof(response), 0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%d", &account_id);

    // Ask for the decision
    strcpy(response, "Enter '1' to approve or '2' to reject the loan: ");
    send(client_socket, response, sizeof(response), 0);
    int recv_size = recv(client_socket, decision, sizeof(decision), 0);
    if (recv_size > 0) {
        int status = atoi(decision);
        int found = 0;
        int record_index = -1;
        for (int i = 0; i < loan_count; i++) {
            loan_t *loan = &loan_db[i]; 

            if (loan->account_id == account_id && loan->employee_id == employee_id && 
                strcmp(loan->processing_status, "under_process") == 0) {
                found = 1;
                record_index = i;
                if (status == 10) {  
                    strcpy(loan->processing_status, "completed");
                    strcpy(loan->loan_status, "approved");  
                } else if (status == 20) { 
                    strcpy(loan->processing_status, "completed");
                    strcpy(loan->loan_status, "rejected");
                } else {
                    strcpy(response, "Invalid decision. Please enter 1 for approve or 2 for reject.\n");
                    send(client_socket, response, sizeof(response), 0);
                    return;  
                }
                break;  
            }
        }
        
        int fd = open("db/loans.db", O_RDWR);
        if (fd < 0) {
            perror("Failed to open loans.db for writing");
            return;
        }

        off_t offset = record_index * sizeof(loan_t);  
        struct flock lock;
        lock.l_type = F_WRLCK; 
        lock.l_whence = SEEK_SET;
        lock.l_start = offset; 
        lock.l_len = sizeof(loan_t); 

        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror("Failed to lock the file");
            close(fd);
            return;
        }

        lseek(fd, offset, SEEK_SET); 
        if (write(fd, &loan_db[record_index], sizeof(loan_t)) == -1) {
            perror("Failed to write the loan record");
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        if (found) {
            strcpy(response, "Loan application processed successfully.\n");
        } else {
            strcpy(response, "No matching loan application found.\n");
        }
        send(client_socket, response, sizeof(response), 0);
    } else {
        perror("Error receiving choice from client");
    }
}

void employee_menu(int client_socket, int employee_id) {
    char buffer[1];  
    char response[4096]; 

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive the client's choice
        int recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recv_size > 0) {
            int choice = atoi(buffer); 
            switch (choice) {
                case 1:
                    view_loans_by_status(client_socket, employee_id, "under_process");
                    break;
                case 2:
                    view_loans_by_status(client_socket, employee_id, "completed");
                    break;
                case 3:
                    process_loan_application(client_socket, employee_id);
                    break;
                case 4:
                    strcpy(response, "Exiting...\n");
                    if (send(client_socket, response, strlen(response), 0) < 0) {
                        perror("Error sending exit message to client");
                    }
                    return;
                default:
                    strcpy(response, "Invalid choice. Please try again.\n");
                    if (send(client_socket, response, strlen(response), 0) < 0) {
                        perror("Error sending invalid choice message to client");
                    }
            }
        } else {
            perror("Error receiving choice from client");
            break;
        }
    }
}
