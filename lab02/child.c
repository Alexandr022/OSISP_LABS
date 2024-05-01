#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void print_usage() {
    printf("Usage: child [env_file]\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    FILE *env_file = fopen(argv[1], "r");
    if (env_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char env_name[100];
    while (fgets(env_name, sizeof(env_name), env_file) != NULL) {
        env_name[strcspn(env_name, "\n")] = 0; 
        char *value = getenv(env_name);
        if (value != NULL) {
            printf("%s=%s\n", env_name, value);
        }
    }

    printf("Child process:\n");
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());

    fclose(env_file);

    return 0;
}
