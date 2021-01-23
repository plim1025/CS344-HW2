#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

// Gets file name to process based on user input
char *getFile() {
    char *fileName = NULL;
    while(1) {
        // Gets user option and handles bad input
        int choice2 = ' ';
        printf("%s", "\nWhich file do you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ");
        scanf("%d", &choice2);
        while(choice2 != 1 && choice2 != 2 && choice2 != 3) {
            printf("%s", "\nError, please select either 1, 2, or 3.\nWhich file do you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ");
            scanf("%d", &choice2);
        }
        // Find the largest .csv file starting with prefix movies_
        if(choice2 == 1) {
            DIR *dir;
            struct dirent *dp;
            long long maxFileSize = -1;
            dir = opendir(".");
            // Loop over all files in directory
            while ((dp=readdir(dir)) != NULL) {
                FILE *fp = fopen(dp->d_name, "r");
                long long size = 0;
                // If file isn't a directory, get size of file
                if(fp != NULL) {
                    fseek(fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                }
                // File must have 'movies_' prefix, be of type csv, and have greater size than previous files to overwrite
                if(strlen(dp->d_name) > 10 && strstr(dp->d_name, "movies_") && (strstr(dp->d_name, ".csv") || strstr(dp->d_name, ".CSV")) && size > maxFileSize) {
                    fileName = malloc(strlen(dp->d_name) * sizeof(char));
                    strcpy(fileName, dp->d_name);
                    maxFileSize = size;
                }
            }
            // If no file matching conditions found, print error, otherwise return file name
            if(fileName == NULL) {
                printf("%s", "No .csv files that start with the prefix movies_ in the current directory ");
            } else {
                return fileName;
            }
        }
        // Find the smallest .csv file starting with prefix movies_
        else if (choice2 == 2) {
            DIR *dir;
            struct dirent *dp;
            long long minFileSize = 9223372036854775807;
            dir = opendir(".");
            // Loop over all files in directory
            while ((dp=readdir(dir)) != NULL) {
                FILE *fp = fopen(dp->d_name, "r");
                long long size = 0;
                // If file isn't a directory, get size of file
                if(fp != NULL) {
                    fseek(fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                }
                // File must have 'movies_' prefix, be of type csv, and have smaller size than previous files to overwrite
                if(strlen(dp->d_name) > 10 && strstr(dp->d_name, "movies_") && (strstr(dp->d_name, ".csv") || strstr(dp->d_name, ".CSV")) && size < minFileSize) {
                    fileName = malloc(strlen(dp->d_name) * sizeof(char));
                    strcpy(fileName, dp->d_name);
                    minFileSize = size;
                }
            }
            // If no file matching conditions found, print error, otherwise return file name
            if(fileName == NULL) {
                printf("%s", "No .csv files that start with the prefix movies_ in the current directory ");
            } else {
                return fileName;
            }
        }
        // Ask the user to enter a file name and handle error if doesn't exist
        else {
            char tempFileName[256];
            printf("%s", "Enter the complete file name: ");
            scanf("%s", tempFileName);
            DIR *dir;
            struct dirent *dp;
            int fileExists = 0;
            dir = opendir(".");
            // Loop over all files in directory and check if given fileName exists
            while ((dp=readdir(dir)) != NULL) {
                if(strcmp(dp->d_name, tempFileName) == 0) {
                    fileExists = 1;
                }
            }
            // If no file matching conditions found, print error, otherwise return file name
            if(fileExists) {
                fileName = malloc(strlen(tempFileName) * sizeof(char));
                strcpy(fileName, tempFileName);
                return fileName;
            } else {
                printf("%s %s %s", "The file", tempFileName, " was not found. Try again\n");
            }
        }
    }
}

// Create new directory and set permissions
char *createDirectory() {
    int randNum = rand() % 100000;
    char prefix[13] = {'l', 'i', 'm', 'p', '.', 'm', 'o', 'v', 'i', 'e', 's', '.', '\0'};
    char suffix[6];

    // Make directory based on ONID and random number
    sprintf(suffix, "%d", randNum);
    char *pathName = malloc(strlen(prefix) + strlen(suffix) + 1);
    strcpy(pathName, prefix);
    strcat(pathName, suffix);
    mkdir(pathName, 0750);
    chmod(pathName, 0750);
    printf("%s %s", "\nCreated directory with name", pathName);
    return pathName;
}

void processFile(char *fileName, char *directoryName) {
    FILE *readFile = fopen(fileName, "r");
    // Print error if can't open file
    if(readFile == NULL) {
        printf("Could not open %s\n", fileName);
        exit(1);
    }

    char *curLine = NULL;
    char *savePtr;
    size_t len = 0;
    ssize_t nread;

    // Skip first line
    getline(&curLine, &len, readFile);

    // Parses file line-by-line
    while((nread = getline(&curLine, &len, readFile)) != -1) {
        // Assign current movie title
        char *token = strtok_r(curLine, ",", &savePtr);
        char *title = calloc(strlen(token) + 1, sizeof(char));
        strcpy(title, token);

        // Assign current movie year
        token = strtok_r(NULL, ",", &savePtr);
        char *year = calloc(strlen(token) + 1, sizeof(char));
        strcpy(year, token);

        // Get write file path
        char writeFileName[27];
        strcpy(writeFileName, directoryName);
        strcat(writeFileName, "/");
        strcat(writeFileName, year);
        strcat(writeFileName, ".txt");

        // Append to file and set permissions of file
        FILE *writeFile = fopen(writeFileName, "a");
        chmod(writeFileName, 0640);
        fprintf(writeFile, "%s\n", title);
        fclose(writeFile);
    }
    // Close file
    fclose(readFile);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int choice1 = 1;
    while(1) {
        // Gets user input and handles errors
        printf("%s", "\n1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ");
        scanf("%d", &choice1);
        while(choice1 != 1 && choice1 != 2) {
            printf("%s", "\nError, please enter either 1 or 2.\n1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2: ");
            scanf("%d", &choice1);
        }

        // Exits program
        if(choice1 == 2) {
            break;
        }

        // Gets name of file to parse
        char *fileName = getFile();
        printf("%s %s", "Now processing the chosen file named", fileName);

        // Create directory and return directory name
        char *directoryName = createDirectory();

        // For each year with at least one movie in file create file with permissions set to rw-r----- and append movie titles in that year
        processFile(fileName, directoryName);
    }
    return 0;
}