#include <stdio.h>

int main() {
    FILE *matrix_data = fopen("matrix_data.csv", "r");
    if (matrix_data == NULL) {
        perror("Error opening file matrix_data.csv");
        exit(1);
    }

    char buffer[10];

    while(fgets(buffer, sizeof(buffer), matrix_data)) {
        char *token;

        token = strtok(buffer, ",");
        while(token != NULL) {
            printf("%s", token);
            printf("\n");
            token = strtok(NULL, ",");
        }
        printf("\n");
    }
}