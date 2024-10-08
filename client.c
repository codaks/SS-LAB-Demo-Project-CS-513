#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#define SERVER_PORT 8088

void employee_menu(int client_socket) {
    char buffer[4096];  
    char choice[1];    

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        char *menu = "\nWelcome, Employee\n"
                           "1. View Loan Applications (Under Process)\n"
                           "2. View Loan Applications (Completed)\n"
                           "3. Approve/Reject Loan Application\n"
                           "4. Exit\n"
                           "Enter your Choice: ";
        printf("%s", menu);
  	scanf("%s", choice);
  	if (send(client_socket, choice, sizeof(choice), 0) < 0) {
    		perror("send");
    		exit(EXIT_FAILURE);
  	}
	if(atoi(choice) ==1 || atoi(choice) == 2){
		int recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
		if (recv_size > 0) {
		    printf("%s", buffer); 
		} else {
		    perror("Error receiving response from server");
		    break;
		}
        }
        else if(atoi(choice) == 3 ){
        	char buffer_2[256], response[256], decision[1];
        	int recv_1 = recv(client_socket, buffer, sizeof(buffer), 0);
		if (recv_1 > 0) {
		    printf("%s", buffer); 
		    memset(buffer, 0, sizeof(buffer));
		} else {
		    perror("Error receiving response from server");
		    break;
		}
		int recv_2 = recv(client_socket, response, sizeof(response), 0);
		if (recv_2 > 0) {
		    printf("%s", response); 
		    memset(response, 0, sizeof(response));
		} else {
		    perror("Error receiving response from server");
		    break;
		}
		scanf("%s", buffer_2);
	  	if (send(client_socket, buffer_2, sizeof(buffer_2), 0) < 0) {
	    		perror("send");
	    		exit(EXIT_FAILURE);
	  	}
	  	int recv_3 = recv(client_socket, response, sizeof(response), 0);
		if (recv_3 > 0) {
		    printf("%s", response); 
		    memset(response, 0, sizeof(response));
		} else {
		    perror("Error receiving response from server");
		    break;
		}
		scanf("%s", decision);
	  	if (send(client_socket, decision, sizeof(decision), 0) < 0) {
	    		perror("send");
	    		exit(EXIT_FAILURE);
	  	}
	  	int recv_4 = recv(client_socket, response, sizeof(response), 0);
		if (recv_4 > 0) {
		    printf("%s", response); 
		    memset(response, 0, sizeof(response));
		} else {
		    perror("Error receiving response from server");
		    break;
		}
		memset(decision, 0, sizeof(decision));
		memset(buffer_2, 0, sizeof(buffer_2));
        }
        else if(atoi(choice) == 4) {
            break;
        }
    }
}




void main(void) {
  struct termios old_settings, new_settings;
  tcgetattr(STDIN_FILENO, &old_settings);
  char username[100] = {0};
  char password[100] = {0};
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(SERVER_PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int connect_result =
      connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address));
  if (connect_result < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  char message[1024];
  int recv_result = recv(client_socket, message, sizeof(message), 0);
  if (recv_result < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", message);
  char choice[1];
  char *main_menu = "Welcome to the Bank\n"
                    "1. Admin\n"
                    "2. Employee\n"
                    "3. Customer\n"
                    "Enter your Choice : ";
  printf("%s", main_menu);
  scanf("%s", choice);
  if (send(client_socket, choice, sizeof(choice), 0) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  printf("Enter your username : ");
  scanf("%s", username);
  if (send(client_socket, username, sizeof(username), 0) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  new_settings = old_settings;
  new_settings.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
  printf("Enter your password : ");
  scanf("%s", password);
  tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
  if (send(client_socket, password, sizeof(password), 0) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }

  char msg[1024];
  if ((recv(client_socket, msg, sizeof(msg), 0)) < 0) {
    perror("recv");
    close(client_socket);
    exit(EXIT_FAILURE);
  }
  printf("%s\n", msg);
  if (strcmp(msg, "Failed to authenticate user!\n") == 0) {
    close(client_socket);
    exit(EXIT_FAILURE);
  }
  int ch = atoi(choice);
  if (ch == 1) {
    char mch[1];
    char *menu = "\nWelcome, Admin\n"
                 "1. Add Employee\n"
                 "2. Activate/Deactivate Customer\n"
                 "3. Update Customer/Employee details\n"
                 "4. View Bank Reports\n"
                 "5. Exit\n"
                 "Enter your Choice : ";
    printf("%s", menu);
    scanf("%s", mch);
    if (send(client_socket, mch, sizeof(mch), 0) < 0) {
      perror("send");
      close(client_socket);
      exit(EXIT_FAILURE);
    }
    if (atoi(mch) == 1) {
    } else if (atoi(mch) > 4) {
      close(client_socket);
      exit(EXIT_SUCCESS);
    }
  }
  else if (ch == 2) {
        employee_menu(client_socket);
    }
}
