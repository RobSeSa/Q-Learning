#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *matrix_data = fopen("../matrix_data.csv", "r");
    if (matrix_data == NULL) {
        perror("Error opening file matrix_data.csv");
        exit(1);
    }

    char buffer[1024];

    int temp;
    while(fgets(buffer, sizeof(buffer), matrix_data)) {
        char *token;

        temp = 0;
        token = strtok(buffer, ",");
        while(token != NULL) {
            if(temp != 0) {
                printf(",");
            }
            temp += 1;
            printf("%s", token);
            token = strtok(NULL, ",");
        }
    }
}
