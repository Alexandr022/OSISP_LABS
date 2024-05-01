#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

extern char **environ;

void sort_env_vars() {
    int i, j;
    char *temp;
    int count = 0;
    while(environ[count]) {
        count++;
    }
    for (i = 0; i < count-1; i++) {
        for (j = i+1; j < count; j++) {
            if (strcmp(environ[i], environ[j]) > 0) {
                temp = environ[i];
                environ[i] = environ[j];
                environ[j] = temp;
            }
        }
    }
}

int main() {
    char *child_path = getenv("CHILD_PATH");
    if (child_path == NULL) {
        fprintf(stderr, "Error: CHILD_PATH environment variable not set\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sort_env_vars();
        printf("Environment variables:\n");
        for (char **env = environ; *env != NULL; env++) {
            printf("%s\n", *env);
        }
        printf("Press 'Ctrl + C' to quit\n");

        char choice;
        scanf(" %c", &choice);
    }
    return 0;
}
