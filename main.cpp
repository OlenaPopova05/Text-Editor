#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class TextEditor {
    char text[1000];
    char buffer[100];
public:

    char* inputText() {
        cout << "Enter text: ";
        char input[100];
        cin.getline(input, 100);
        return input;
    }

    char* fileName() {
        cout << "Enter the file name: ";
        static char filename[21];
        cin.getline(filename, 21);
        return filename;
    }

    void appendInput() {
        char* input = inputText();
        strcat(text, input);
        cout << "Text appended\n";
    }

    void startNewLine() {
        strcat(text, "\n");
        cout << "New line started\n";
    }

    void saveToFile() {
        char* filename = fileName();
        cout << "Do you want to overwrite the file or append to it? (o/a): ";
        char mode;
        cin >> mode;
        cin.ignore(); // Ignore the newline character left in the input stream
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
            cout << "Saved to file\n";
        }
        else
        {
            cout << "Error saving to file\n";
        }
    }

    void loadFromFile() {
        char* filename = fileName();
        FILE* file = fopen(filename, "r");
        if (file != NULL)
        {
            char line[1000];
            while (fgets(line, sizeof(line), file)) {
                strcat(text, line);
            }
            fclose(file);
            cout << "Loaded from file\n";
        }
        else
        {
            cout << "Error loading from file\n";
        }
    }

    void printText() {
        cout << "Current text: " << text << endl;
    }

    void insertText() {
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char* input = inputText();
        int size = strlen(text);
        int new_size = size + strlen(input);
        int lines = 0;
        int char_index = 0;
        int previous_chars = 0;
        for (int i = 0; i < size; i++) {
            if (text[i] == '\n') {
                lines++;
                char_index = 0;
            }
            previous_chars = i + 1;
            if (lines == line && char_index == index) {
                for (int k = new_size; k >= previous_chars + strlen(input); k--) {
                    text[k] = text[k - strlen(input)];
                }
                for (int j = 0; j < strlen(input); j++) {
                    text[previous_chars + j] = input[j];
                }
                break; // Assuming insertion is complete after this
            }
            char_index++;
        }
    }

    void searchWord() {
        cout << "Enter the word to search: ";
        char word[100];
        cin >> word;
        int text_length = strlen(text);
        int word_length = strlen(word);
        int line = 0;
        int char_index = 0;
        for (int i = 0; i <= text_length - word_length; i++) {
            if (strncmp(&text[i], word, word_length) == 0) {
                cout << "Word found at line " << line << ", index " << char_index << endl;
            }
            char_index++;
            if (text[i] == '\n') {
                line++;
                char_index = 0;
            }
        }
    }

    void deleteText() {
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int new_size = strlen(text) - n;
        int previous = findPosition(line, index);

        while (previous < new_size) {
            text[previous] = text[previous + n];
            previous++;
        }

        text[new_size] = '\0';
        cout << "Text deleted\n";
    }

    void insertWithReplacement() {
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char* input = inputText();
        int input_size = strlen(input);
        int previous = findPosition(line, index);

        int k = 0;
        for (int i = previous; i < input_size + previous; i++) {
            text[i] = input[k];
            k++;
        }
        cout << "Text inserted with replacement\n";
    }

    void cutText() {
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int new_size = strlen(text) - n;
        int previous = findPosition(line, index);

        for (int i = previous; i < previous + n; i++) {
            buffer[i - previous] = text[i];
        }

        while (previous < new_size) {
            text[previous] = text[previous + n];
            previous++;
        }

        text[new_size] = '\0';
        cout << "Text cut\n";
    }

    void copyText() {
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int previous = findPosition(line, index);

        for (int i = previous; i < previous + n; i++) {
            buffer[i - previous] = text[i];
        }
        cout << "Text copied\n";
    }

    void pasteText() {
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int previous = findPosition(line, index);
        int buffer_size = strlen(buffer);
        int new_size = strlen(text) + buffer_size;

        for (int i = new_size; i >= previous + buffer_size; i--) {
            text[i] = text[i - buffer_size];
        }

        for (int i = 0; i < buffer_size; i++) {
            text[previous + i] = buffer[i];
        }
        cout << "Text pasted\n";
    }

    int findPosition(int line, int index) {
        int previous = 0;
        int size = strlen(text);
        int lines = 0;
        int char_index = -1;
        while (previous < size) {
            if (text[previous] == '\n') {
                lines++;
                char_index = -1;
            }
            else {
                char_index++;
            }
            if (lines == line && char_index == index) {
                break;
            }
            previous++;
        }
        return previous;
    }
};


int main() {
    while (true) {
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        fflush(stdin);
        TextEditor tE;
        switch (choice) {
            case 1:
                tE.appendInput();
                break;
            case 2:
                tE.startNewLine();
                break;
            case 3:
                tE.saveToFile();
                break;
            case 4:
                tE.loadFromFile();
                break;
            case 5:
                tE.printText();
                break;
            case 6:
                tE.insertText();
                break;
            case 7:
                tE.searchWord();
                break;
            case 8:
                tE.deleteText();
                break;
            case 9:
                tE.insertWithReplacement();
                break;
            case 10:
                tE.cutText();
                break;
            case 11:
                tE.copyText();
                break;
            case 12:
                tE.pasteText();
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
}
