#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

/*
 * Alloziert dynamisch Speicher und
 * konkateniert beide Parameter in einem String, inkl. Slash.
 * Es wird ein Zeiger auf den allozierten Speicher zurückgegeben.
 */
static char *concat_paths(const char *first, const char *second) {
    // calculate needed length: first + "/" + second + '\0'
    size_t len = strlen(first) + strlen(second) + 2;
    char *result = malloc(len);
    if (result == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    strcpy(result, first);

    // add slash only if first doesn’t already end with one
    if (first[strlen(first) - 1] != '/')
        strcat(result, "/");

    strcat(result, second);

    return result; // caller must free()
}

/*
 * Bestimmt für den Parameter path,
 * ob es eine Datei (Rückgabewert 1)
 * oder ein Verzeichnis (Rückgabewert 2) ist.
 */
static int dir_entry_type(char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat failed");
        return -1; // error case
    }

    if (S_ISREG(st.st_mode)) {
        return 1; // file
    } else if (S_ISDIR(st.st_mode)) {
        return 2; // directory
    } else {
        return 0; // something else (symlink, device, etc.)
    }
}

static int is_dot_dir(char *entryName) {
    return !strncmp(entryName, ".", 1) || !strncmp(entryName, "..", 2);
}

int count_files(char *dirName) {
    int total = 0;
    DIR *dir = opendir(dirName);
    
    if(dir == NULL) {
        printf("opendir\n");
        exit(EXIT_FAILURE);
    }
    
    errno = 0;
    // if(entry == NULL && errno != 0) {
    //     printf("readdir\n");
    //     exit(EXIT_FAILURE);
    // }
    struct dirent *entry ;
    while ((entry = readdir(dir)) != NULL) {
        if(is_dot_dir(entry->d_name)) {
            continue;
        }
        
        char *last_path = concat_paths(dirName, entry->d_name);
        if(dir_entry_type(last_path) == 1) {
            total += 1;
        }
        free(last_path);
    }
    
    if(closedir(dir) == -1) {
        printf("closedir\n");
        exit(EXIT_FAILURE);
    }
    return total;
}

int main(int argc, const char *argv[]) {
    int sum = 0;
    if(argc < 2) {
        printf("argc\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < argc; i++) {
        sum += count_files(argv[i]);
    }
    printf("Sum of all files: %d\n", sum);
    return 0;
}