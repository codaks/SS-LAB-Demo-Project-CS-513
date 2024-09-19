#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVER_PORT 8088
#define MAX_CONNECTIONS 100

enum user_type { ADMIN, EMPLOYEE, CUSTOMER };

typedef struct {
  int id;
  enum user_type type;
  char username[128];
  char password[128];
  int active;
} admin_t, employee_t, customer_t, user_t;

typedef struct {
  int socket_fd;
  user_t user;
} client_connection_t;

typedef struct {
  int id;
  int balance;
  int loan;
} account_t;

pthread_mutex_t admin_db_mutex;
pthread_mutex_t employee_db_mutex;
pthread_mutex_t customer_db_mutex;
pthread_mutex_t account_lock;

customer_t customer_db[100];
admin_t admin_db[100];
account_t account_db[100];
employee_t employee_db[100];

void load_admin_db() {
  FILE *fp = fopen("db/admin.db", "r");
  if (fp < 0) {
    perror("open");
    exit(1);
  }
  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    admin_t admin;
    sscanf(line, "%d %d %s %s %d\n", &admin.id, &admin.type, admin.username,
           admin.password, &admin.active);
    admin_db[admin.id] = admin;
  }
  fclose(fp);
}

void load_employee_db() {
  FILE *fp = fopen("db/employee.db", "r");
  if (fp < 0) {
    perror("open");
    exit(1);
  }
  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    employee_t employee;
    sscanf(line, "%d %d %s %s %d\n", &employee.id, &employee.type,
           employee.username, employee.password, &employee.active);
    employee_db[employee.id] = employee;
  }
  fclose(fp);
}

void load_customer_db() {
  FILE *fp = fopen("db/customers.db", "r");
  if (fp < 0) {
    perror("open");
    exit(1);
  }
  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    customer_t customer;
    sscanf(line, "%d %d %s %s %d\n", &customer.id, &customer.type,
           customer.username, customer.password, &customer.active);
    customer_db[customer.id] = customer;
  }
  fclose(fp);
}

void load_account_db() {
  FILE *fp = fopen("db/accounts.db", "r");
  if (fp < 0) {
    perror("open");
    exit(1);
  }
  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    account_t account;
    sscanf(line, "%d %d %d\n", &account.id, &account.balance, &account.loan);
    account_db[account.id] = account;
  }
  fclose(fp);
}

void save_employee_db() {
  int fd = open("db/employee.db", O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    perror("open");
    exit(1);
  }
  char buf[1024];
  for (int i = 0; i < 100; i++) {
    employee_t employee = employee_db[i];
    if (employee.active) {
      if (employee.username[0] != '\0') {
        sprintf(buf, "%d %d %s %s %d\n", employee.id, employee.type,
                employee.username, employee.password, employee.active);
        int bytes_written = write(fd, buf, strlen(buf));
        if (bytes_written < 0) {
          perror("write");
          break;
        }
      }
    }
  }
  close(fd);
}

void save_customer_db() {
  int fd = open("db/customer.db", O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    perror("open");
    exit(1);
  }
  char buf[1024];
  for (int i = 0; i < 100; i++) {
    customer_t customer = customer_db[i];
    if (customer.active) {
      if (customer.username[0] != '\0') {
        sprintf(buf, "%d %d %s %s %d\n", customer.id, customer.type,
                customer.username, customer.password, customer.active);
        int bytes_written = write(fd, buf, strlen(buf));
        if (bytes_written < 0) {
          perror("write");
          break;
        }
      }
    }
  }
  close(fd);
}

void save_accounts_db() {
  int fd = open("db/account.db", O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    perror("open");
    exit(1);
  }
  char buf[1024];
  for (int i = 0; i < 100; i++) {
    account_t accounts = account_db[i];
    sprintf(buf, "%d %d %d\n", accounts.id, accounts.balance, accounts.loan);
    int bytes_written = write(fd, buf, strlen(buf));
    if (bytes_written < 0) {
      perror("write");
      break;
    }
  }
  close(fd);
}

int authenticate_admin(char *username, char *password) {
  pthread_mutex_lock(&admin_db_mutex);
  int i, id;
  for (i = 0; i < 100; i++) {
    admin_t admin = admin_db[i];
    if (admin.active == 1 && strcmp(admin.username, username) == 0 &&
        strcmp(admin.password, password) == 0) {
      id = admin.id;
      break;
    }
  }
  pthread_mutex_unlock(&admin_db_mutex);
  return (i < 100) ? id : -1;
}

int authenticate_employee(char *username, char *password) {
  pthread_mutex_lock(&employee_db_mutex);
  int i, id;
  for (i = 0; i < 100; i++) {
    employee_t employee = employee_db[i];
    if (employee.active == 1 && strcmp(employee.username, username) == 0 &&
        strcmp(employee.password, password) == 0) {
      id = employee.id;
      break;
    }
  }
  pthread_mutex_unlock(&employee_db_mutex);
  return (i < 100) ? id : -1;
}

int authenticate_customer(char *username, char *password) {
  pthread_mutex_lock(&customer_db_mutex);
  int i, id;
  for (i = 0; i < 100; i++) {
    customer_t customer = customer_db[i];
    if (customer.active == 1 && strcmp(customer.username, username) == 0 &&
        strcmp(customer.password, password) == 0) {
      id = customer.id;
      break;
    }
  }
  pthread_mutex_unlock(&customer_db_mutex);
  return (i < 100) ? id : -1;
}

int create_employee(char *username, char *password) {
  pthread_mutex_lock(&employee_db_mutex);
  int id = 0;
  while (employee_db[id].username[0] != '\0') {
    id++;
  }
  employee_t employee;
  employee.id = id;
  employee.type = EMPLOYEE;
  strcpy(employee.username, username);
  strcpy(employee.password, password);
  employee.active = 1;
  employee_db[id] = employee;
  save_employee_db();
  pthread_mutex_unlock(&employee_db_mutex);
  return id;
}

int create_customer(char *username, char *password) {
  pthread_mutex_lock(&customer_db_mutex);
  int id = 0;
  while (customer_db[id].username[0] != '\0') {
    id++;
  }
  customer_t customer;
  customer.id = id;
  customer.type = CUSTOMER;
  strcpy(customer.username, username);
  strcpy(customer.password, password);
  customer.active = 1;
  customer_db[id] = customer;
  save_customer_db();
  pthread_mutex_unlock(&customer_db_mutex);
  return id;
}

customer_t *get_customer_by_id(int id) {
  struct flock customer_db_lock;
  customer_db_lock.l_type = F_WRLCK;
  customer_db_lock.l_whence = SEEK_SET;
  customer_db_lock.l_start = 0;
  customer_db_lock.l_len = 0;
  int customer_db_fd = open("db/customer.db", O_RDWR);
  fcntl(customer_db_fd, F_SETLK, &customer_db_lock);
  customer_t *customer = NULL;
  for (int i = 0; i < 100; i++) {
    customer_t current_customer = customer_db[i];
    if (current_customer.id == id && current_customer.active == 1) {
      customer = &current_customer;
      break;
    }
  }
  customer_db_lock.l_type = F_UNLCK;
  fcntl(customer_db_fd, F_SETLK, &customer_db_lock);
  close(customer_db_fd);
  return customer;
}

void save_customer(customer_t *customer) {
  struct flock customer_db_lock;
  customer_db_lock.l_type = F_WRLCK;
  customer_db_lock.l_whence = SEEK_SET;
  customer_db_lock.l_start = 0;
  customer_db_lock.l_len = 0;
  int customer_db_fd = open("db/customer.db", O_RDWR);
  fcntl(customer_db_fd, F_SETLK, &customer_db_lock);
  int customer_id = customer->id;
  customer_db[customer_id].active = customer->active;
  save_customer_db();
  customer_db_lock.l_type = F_UNLCK;
  fcntl(customer_db_fd, F_SETLK, &customer_db_lock);
  close(customer_db_fd);
}

void activate_deactivate_customer(int id, int active) {
  customer_t *customer = get_customer_by_id(id);
  char snum[2];
  customer->active = active;
  save_customer(customer);
}

void *handle_client_connection(void *args) {
  client_connection_t *connection = (client_connection_t *)args;
  char choice[1];
  if ((recv(connection->socket_fd, choice, sizeof(choice), 0)) < 0) {
    perror("recv");
    exit(1);
  }
  int ch = atoi(choice);
  connection->user.type = ch - 1;
  if (ch - 1 == 0) {
    if ((recv(connection->socket_fd, connection->user.username,
              sizeof(connection->user.username), 0)) < 0) {
      perror("recv");
    }
    if ((recv(connection->socket_fd, connection->user.password,
              sizeof(connection->user.password), 0)) < 0) {
      perror("recv");
    }
    int user_id = authenticate_admin(connection->user.username,
                                     connection->user.password);
    if (user_id == -1) {
      if ((send(connection->socket_fd, "Failed to authenticate",
                sizeof("Failed to authenticate"), 0)) < 0) {
        perror("send");
        close(connection->socket_fd);
        exit(EXIT_FAILURE);
      }
    }
    send(connection->socket_fd, "Authenticated", sizeof("Authenticated"), 0);
  }
}

void main(void) {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("socket");
    exit(1);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(SERVER_PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;
  int bind_result = bind(server_socket, (struct sockaddr *)&server_address,
                         sizeof(server_address));
  if (bind_result < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  listen(server_socket, MAX_CONNECTIONS);
  load_admin_db();
  load_employee_db();
  load_customer_db();
  load_account_db();
  while (1) {
    client_connection_t *client_connection =
        malloc(sizeof(client_connection_t));
    client_connection->socket_fd = accept(server_socket, NULL, NULL);
    if (client_connection->socket_fd < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    int send_ack =
        send(client_connection->socket_fd, "Connection Success!", 20, 0);
    if (send_ack < 0) {
      perror("accept");
    }
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client_connection, client_connection);
  }
  close(server_socket);
}
