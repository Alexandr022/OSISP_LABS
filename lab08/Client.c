#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define DIR_WITH_PROGRAM "/home/pahan/tar_working_dir/Hvesco/OSISP/lab08/build/debug/"

void print_prompt() {
    printf("> ");
    fflush(stdout);
}

void process_file_requests(int client_socket, const char* filename) {
    char patch_to_file[BUFFER_SIZE];
    sprintf(patch_to_file, "%s%s", DIR_WITH_PROGRAM, filename);
    FILE* file = fopen(patch_to_file, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            break;
        }

        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == -1) {
            perror("recv");
            break;
        } else if (bytes_received == 0) {
            printf("Сервер закрыл соединение\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_socket);
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        return 1;
    }

    system("clear");
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_socket);
        return 1;
    } else if (bytes_received == 0) {
        printf("Сервер закрыл соединение\n");
        close(client_socket);
        return 1;
    }

    buffer[bytes_received] = '\0';
    printf("%s\n", buffer);

    while (1) {
        print_prompt();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '@') {
            process_file_requests(client_socket, buffer + 1);
        } else {
            if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
                perror("send");
                break;
            }

            if (strcmp(buffer, "QUIT") == 0) {
                break;
            }

            bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received == -1) {
                perror("recv");
                break;
            } else if (bytes_received == 0) {
                printf("Сервер закрыл соединение\n");
                break;
            }

            buffer[bytes_received] = '\0';
            printf("%s\n", buffer);
        }
    }

    close(client_socket);
    return 0;
}
