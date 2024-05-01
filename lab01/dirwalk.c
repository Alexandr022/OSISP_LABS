#define _GNU_SOURCE // Для определения PATH_MAX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_usage() {
    printf("Usage: dirwalk [dir] [options]\n");
    printf("Options:\n");
    printf("\t-l\tOnly symbolic links\n");
    printf("\t-d\tOnly directories\n");
    printf("\t-f\tOnly regular files\n");
    printf("\t-s\tSort output according to LC_COLLATE\n");
    printf("\t-F\tOutput format: 'find' or 'ls -l'\n");
}

void print_file_info_find(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return;
    }

    printf("%s\n", filename);
}

void print_file_info_ls(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return;
    }

    char date_string[80];
    strftime(date_string, sizeof(date_string), "%c", localtime(&file_stat.st_mtime));

    printf("%c%s %ld %s %s %ld %s %s\n",
           (S_ISDIR(file_stat.st_mode)) ? 'd' : '-',
           (file_stat.st_mode & S_IRUSR) ? "r" : "-",
           (long) file_stat.st_nlink,
           getpwuid(file_stat.st_uid)->pw_name,
           getgrgid(file_stat.st_gid)->gr_name,
           (long) file_stat.st_size,
           date_string,
           filename);
}

int main(int argc, char *argv[]) {
    int symbolic_links = 1, directories = 1, regular_files = 1;
    int format_option = 0; // 0 - find, 1 - ls -l
    int opt;

    while ((opt = getopt(argc, argv, "ldfsF:")) != -1) {
        switch (opt) {
            case 'l':
                directories = 0;
                regular_files = 0;
                break;
            case 'd':
                symbolic_links = 0;
                regular_files = 0;
                break;
            case 'f':
                symbolic_links = 0;
                directories = 0;
                break;
            case 's':
                // логика для опции 's'
                break;
            case 'F':
                if (strcmp(optarg, "find") == 0)
                    format_option = 0;
                else if (strcmp(optarg, "ls") == 0)
                    format_option = 1;
                else {
                    printf("Invalid output format. Supported formats: 'find' or 'ls'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (optind == argc) {
        // Если не указаны файлы, используем текущую директорию
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (format_option == 0)
                print_file_info_find(entry->d_name);
            else
                print_file_info_ls(entry->d_name);
        }

        closedir(dir);
    } else {
        // Вывод информации о каждом переданном файле
        for (int i = optind; i < argc; i++) {
            if (format_option == 0)
                print_file_info_find(argv[i]);
            else
                print_file_info_ls(argv[i]);
        }
    }

    return 0;
}
