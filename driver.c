#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Returns pointer to file based on option
char *getFile() {
    char *fileName = NULL;
    while(1) {
        // Gets user input and handles errors
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
            long long maxFileSize = 0;
            dir = opendir(".");
            while ((dp=readdir(dir)) != NULL) {
                FILE *fp = fopen(dp->d_name, "r");
                fseek(fp, 0, SEEK_END);
                long long size = ftell(fp);
                fclose(fp);
                // File must have 'movies_' prefix and be of type csv
                if(strlen(dp->d_name) > 10 && strstr(dp->d_name, "movies_") && (strstr(dp->d_name, ".csv") || strstr(dp->d_name, ".CSV")) && size > maxFileSize) {
                    fileName = malloc(strlen(dp->d_name) * sizeof(char));
                    strcpy(fileName, dp->d_name);
                    maxFileSize = size;
                }
            }
            // No file matching conditions found, print error
            if(fileName == NULL) {
                printf("%s", "No .csv files that start with the prefix movies_ in the current directory ");
            }
            break;
        }
        // Find the smallest .csv file starting with prefix movies_
        else if (choice2 == 2) {
            DIR *dir;
            struct dirent *dp;
            long long minFileSize = 9223372036854775807;
            dir = opendir(".");
            while ((dp=readdir(dir)) != NULL) {
                FILE *fp = fopen(dp->d_name, "r");
                fseek(fp, 0, SEEK_END);
                long long size = ftell(fp);
                fclose(fp);
                // File must have 'movies_' prefix and be of type csv
                if(strlen(dp->d_name) > 10 && strstr(dp->d_name, "movies_") && (strstr(dp->d_name, ".csv") || strstr(dp->d_name, ".CSV")) && size < minFileSize) {
                    fileName = malloc(strlen(dp->d_name) * sizeof(char));
                    strcpy(fileName, dp->d_name);
                    minFileSize = size;
                }
            }
            // No file matching conditions found, print error
            if(fileName == NULL) {
                printf("%s", "No .csv files that start with the prefix movies_ in the current directory ");
            }
            break;
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
            while ((dp=readdir(dir)) != NULL) {
                if(strcmp(dp->d_name, tempFileName) == 0) {
                    fileExists = 1;
                }
            }
            // No file matching conditions found, print error
            if(fileExists) {
                fileName = tempFileName;
                break;
            } else {
                printf("%s %s %s", "The file", tempFileName, " was not found. Try again\n");
            }
        }
    }
    return fileName;
}

int main(int argc, char **argv) {
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
        printf("%s %s\n", "Now processing the chosen file named", fileName);

        // Create new directory with permissions set to rwxr-x---
        int randNum = rand() % 100000;
        char prefix[13] = {'l', 'i', 'm', 'p', '.', 'm', 'o', 'v', 'i', 'e', 's', '.', '\0'};
        char suffix[6];
        sprintf(suffix, "%d", randNum);
        char *pathName = malloc(strlen(prefix) + strlen(suffix) + 1);
        strcpy(pathName, prefix);
        strcpy(pathName, suffix);
        mkdir(pathName, 0700);

        // For each year with at least one movie in file create file with permissions set to rw-r-----

        // Write in file name of movies that aired in that year
    }
    return 0;
}