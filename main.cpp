#include <iostream>

char* append_input(char* text, char* input) {

    printf("Enter text to append: ");
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    strcat(text, input);
    printf("Text appended\n");
    return strdup(text);
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
                printf("Saved to file\n");
                break;
            case 4:
                printf("Loaded from file\n");
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
