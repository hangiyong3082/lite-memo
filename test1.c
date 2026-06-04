#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


#define MAX_LEN 256

#define MODE_NONE     0
#define MODE_DELETE   1
#define MODE_APPEND   2
#define MODE_REWRITE  3

FILE* openFile(const char* fileName, const char* format) {
    FILE* file = fopen(fileName, format);
    if (file == NULL) {
        printf("Can't open file\n");
        return NULL;
    }
    return file;
}

int main() {
    char fileName[100];
    FILE* fp;

    printf("Type name of text file >>> ");
    scanf("%99s", fileName);
    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Can't open file\n");
        return 1;
    }
    printf("File was successfully opened\n\n");
    printf("-----content-----\n");
    char line[MAX_LEN];
    int currentLine = 1;
    int maxLine = 1;
    char input[MAX_LEN];
    char contentToAdd[MAX_LEN];
    int inputLineNum;
    int inputLineNum2;

    while (1) {
        fp = fopen(fileName, "r");
        while (fscanf(fp, "%255[^\n]%*c", line) == 1) {
            printf("%3d] %s\n", currentLine++, line);
        }
        maxLine = currentLine - 1;
        currentLine = 1;
        fclose(fp);
        printf(">>> ");
        scanf(" %[^\n]", input);
        getchar();

        FILE* temp;
        char buffer[MAX_LEN];
        int currLine = 1;
        int targetLine;
        int mode = MODE_NONE;
        int startLine = 0, endLine = 0;
        char tail;
        if (strcmp(input, "end]") == 0) {
            break;
        }
        else if (sscanf(input, "del]%d-%d", &inputLineNum, &inputLineNum2) == 2) {
            mode = MODE_DELETE;
            startLine = inputLineNum;
            endLine = inputLineNum2;
        }
        else if (sscanf(input, "del]%d%c", &inputLineNum, &tail) == 1) {
            mode = MODE_DELETE;
            startLine = inputLineNum;
            endLine = inputLineNum;
        }
        else if (sscanf(input, "del]%d-", &inputLineNum) == 1) {
            mode = MODE_DELETE;
            startLine = inputLineNum;
            endLine = maxLine;
        }
        else if (sscanf(input, "%d]+%255[^\n]", &inputLineNum, contentToAdd) == 2) {
            mode = MODE_APPEND;
            startLine = inputLineNum;
        }
        else if (sscanf(input, "%d]%255[^\n]", &inputLineNum, contentToAdd) == 2) {
            mode = MODE_REWRITE;
            startLine = inputLineNum;
        }
        if (mode != MODE_NONE) {
            fp = openFile(fileName, "r");
            temp = openFile("temp.txt", "w");
            if (temp == NULL) {
                printf("temp.txt file can't be created\n");
            }

            currLine = 1;
            while (fgets(buffer, MAX_LEN, fp) != NULL) {

                if (mode == MODE_DELETE) {
                    if (currLine < startLine || currLine > endLine) {
                        fputs(buffer, temp);
                    }
                }
                else if (currLine == startLine) {
                    char result[256];

                    if (mode == MODE_APPEND) {
                        strcpy(result, buffer);
                        result[strcspn(result, "\n")] = '\0';
                        strcat(result, contentToAdd);
                    }
                    else if (mode == MODE_REWRITE) {
                        strcpy(result, contentToAdd);
                    }

                    strcat(result, "\n");
                    fputs(result, temp);
                }
                else {
                    fputs(buffer, temp);
                }

                currLine++;
            }

            fclose(fp);
            fclose(temp);
            remove(fileName);
            rename("temp.txt", fileName);
        }
        else {
            fp = openFile(fileName, "a");
            fprintf(fp, "%s\n", input);
            fclose(fp);
        }
        for (int i = 0;i < 100;i++)
            printf("\n");
    }

    printf("exit memo\n");
    if (fp != NULL)
        fclose(fp);
    return 0;
}
