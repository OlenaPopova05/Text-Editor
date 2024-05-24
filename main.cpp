#include <iostream>

void append_input(char* text, char* input) {
    printf("Enter text to append: ");
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    strcat(text, input);
    printf("Text appended\n");
}

void save_to_file(char* text) {
    printf("Enter the file name for saving: ");
    char filename[21];
    fgets(filename, 21, stdin);
    filename[strcspn(filename, "\n")] = 0;
    printf("Do you want to overwrite the file or append to it? (o/a): ");
    char mode;
    mode = getchar();
    FILE* file;
    if (mode == 'o') {
        file = fopen(filename, "w");
    }
    else if (mode == 'a') {
        file = fopen(filename, "a");
    }
    if (file != NULL)
    {
        fprintf(file, "%s", text);
        fclose(file);
        printf("Saved to file\n");
    }
    else
    {
        printf("Error saving to file\n");
    }
}

void load_from_file(char* text) {
    printf("Enter the file name for loading: ");
    char filename[21];
    fgets(filename, 21, stdin);
    filename[strcspn(filename, "\n")] = 0;
    FILE* file = fopen(filename, "r");
    if (file != NULL)
    {
        char line[1000];
        while (fgets(line, sizeof(line), file)) {
            strcat(text, line);
        }
        fclose(file);
        printf("Loaded from file\n");
    }
    else
    {
        printf("Error loading from file\n");
    }
}

void print_commands() {
    printf("Commands:\n");
    printf("1. Append text symbols to the end\n");
    printf("2. Start the new line\n");
    printf("3. Save to file\n");
    printf("4. Load from file\n");
    printf("5. Print the current text\n");
}

int main()
{
    while (true){
        //print_commands();
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        fflush(stdin); // clear input buffer
        char input[100];
        char text[1000];
        switch (choice) {
            case 1:
                append_input(text, input);
                break;
            case 2:
                text[strlen(text)] = '\n';
                printf("New line started\n");
                break;
            case 3:
                save_to_file(text);
                break;
            case 4:
                load_from_file(text);
                break;
            case 5:
                printf("Current text: ");
                printf("%s\n", text);
                break;
            default:
                printf("Invalid choice\n");
        }
    }
}
