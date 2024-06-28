#include <iostream>
#include <cstdio>
#include <cstring>
#include <stack>
#include <dlfcn.h>

using namespace std;

class Text {
public:
    char textArray[1000];
};

class CaesarCipher {
public:
    void* handle;

    CaesarCipher() {
        handle = dlopen("/Users/olenapopova/Documents/GitHub/Caesar-Encryption-Algorithm/caesar_algorithm.dylib", RTLD_LAZY);
        if (!handle) {
            std::cerr << "Error: " << dlerror() << std::endl;
            exit(1);
        }
    }

    ~CaesarCipher() {
        if (handle) {
            dlclose(handle);
        }
    }

    char* encryption(char* text, int& key) {
        typedef char* (*encryption_func)(char*, int&);
        encryption_func encrypt = (encryption_func) dlsym(handle, "encryption");
        if (!encrypt) {
            std::cerr << "Error: " << dlerror() << std::endl;
            dlclose(handle);
            return nullptr;
        }
        return encrypt(text, key);
    }

    char* decryption(char* text, int& key) {
        typedef char* (*decryption_func)(char*, int&);
        decryption_func decrypt = (decryption_func) dlsym(handle, "decryption");
        if (!decrypt) {
            std::cerr << "Error: " << dlerror() << std::endl;
            dlclose(handle);
            return nullptr;
        }
        return decrypt(text, key);
    }
};

class TextEditor {
    Text text;
    char buffer[100];
    stack<char*> undoStack;
    stack<char*> redoStack;
    int key;
    CaesarCipher CC;
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
        saveToUndoStack();
        char* input = inputText();
        strcat(text.textArray, input);
        cout << "Text appended\n";
    }

    void startNewLine() {
        saveToUndoStack();
        strcat(text.textArray, "\n");
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
            fprintf(file, "%s", text.textArray);
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
                strcat(text.textArray, line);
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
        cout << "Current text: " << text.textArray << endl;
    }

    void insertText() {
        saveToUndoStack();
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char* input = inputText();
        int size = strlen(text.textArray);
        int new_size = size + strlen(input);
        int lines = 0;
        int char_index = 0;
        int previous_chars = 0;
        for (int i = 0; i < size; i++) {
            if (text.textArray[i] == '\n') {
                lines++;
                char_index = 0;
            }
            previous_chars = i + 1;
            if (lines == line && char_index == index) {
                for (int k = new_size; k >= previous_chars + strlen(input); k--) {
                    text.textArray[k] = text.textArray[k - strlen(input)];
                }
                for (int j = 0; j < strlen(input); j++) {
                    text.textArray[previous_chars + j] = input[j];
                }
                break;
            }
            char_index++;
        }
    }

    void searchWord() {
        cout << "Enter the word to search: ";
        char word[100];
        cin >> word;
        int text_length = strlen(text.textArray);
        int word_length = strlen(word);
        int line = 0;
        int char_index = 0;
        for (int i = 0; i <= text_length - word_length; i++) {
            if (strncmp(&text.textArray[i], word, word_length) == 0) {
                cout << "Word found at line " << line << ", index " << char_index << endl;
            }
            char_index++;
            if (text.textArray[i] == '\n') {
                line++;
                char_index = 0;
            }
        }
    }

    void deleteText() {
        saveToUndoStack();
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int new_size = strlen(text.textArray) - n;
        int previous = findPosition(line, index);

        while (previous < new_size) {
            text.textArray[previous] = text.textArray[previous + n];
            previous++;
        }

        text.textArray[new_size] = '\0';
        cout << "Text deleted\n";
    }

    void insertWithReplacement() {
        saveToUndoStack();
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char* input = inputText();
        int input_size = strlen(input);
        int previous = findPosition(line, index);

        int k = 0;
        for (int i = previous; i < input_size + previous; i++) {
            text.textArray[i] = input[k];
            k++;
        }
        cout << "Text inserted with replacement\n";
    }

    void cutText() {
        saveToUndoStack();
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int new_size = strlen(text.textArray) - n;
        int previous = findPosition(line, index);

        for (int i = previous; i < previous + n; i++) {
            buffer[i - previous] = text.textArray[i];
        }

        while (previous < new_size) {
            text.textArray[previous] = text.textArray[previous + n];
            previous++;
        }

        text.textArray[new_size] = '\0';
        cout << "Text cut\n";
    }

    void copyText() {
        cout << "Choose line and index and number of symbols: ";
        int line, index, n;
        cin >> line >> index >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        int previous = findPosition(line, index);

        for (int i = previous; i < previous + n; i++) {
            buffer[i - previous] = text.textArray[i];
        }
        cout << "Text copied\n";
    }

    void pasteText() {
        saveToUndoStack();
        cout << "Choose line and index: ";
        int line, index;
        cin >> line >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int previous = findPosition(line, index);
        int buffer_size = strlen(buffer);
        int new_size = strlen(text.textArray) + buffer_size;

        for (int i = new_size; i >= previous + buffer_size; i--) {
            text.textArray[i] = text.textArray[i - buffer_size];
        }

        for (int i = 0; i < buffer_size; i++) {
            text.textArray[previous + i] = buffer[i];
        }
        cout << "Text pasted\n";
    }

    int findPosition(int line, int index) {
        int previous = 0;
        int size = strlen(text.textArray);
        int lines = 0;
        int char_index = -1;
        while (previous < size) {
            if (text.textArray[previous] == '\n') {
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

    void saveToUndoStack() {
        char* textCopy = new char[1000];
        strcpy(textCopy, text.textArray);
        undoStack.push(textCopy);
    }

    void undo() {
        if (!undoStack.empty()) {
            char* textCopy = undoStack.top();
            char* redoText = new char[1000];
            redoStack.push(strcpy(redoText, text.textArray));
            strcpy(text.textArray, textCopy);
            undoStack.pop();
            delete textCopy;
            cout << "Undo successful\n";
        }
        else {
            cout << "Nothing to undo\n";
        }
    }
    void redo() {
        if (!redoStack.empty()) {
            char* textCopy = redoStack.top();
            char* undoText = new char[1000];
            strcpy(undoText, text.textArray);
            undoStack.push(undoText);
            strcpy(text.textArray, textCopy);
            redoStack.pop();
            delete textCopy;
            cout << "Redo successful\n";
        }
        else {
            cout << "Nothing to redo\n";
        }
    }

    void encryptText() {
        cout << "Enter the key: ";
        cin >> key;

        char* encryptedText = CC.encryption(text.textArray, key);
        cout << "Encrypted text: " << encryptedText << endl;
    }

    void decryptText() {
        cout << "Enter the key: ";
        cin >> key;

        char* decryptedText = CC.decryption(text.textArray, key);
        cout << "Decrypted text: " << decryptedText << endl;
    }
};


int main() {
    TextEditor tE;
    while (true) {
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        fflush(stdin);
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
            case 13:
                tE.undo();
                break;
            case 14:
                tE.redo();
                break;
            case 15:
                tE.encryptText();
                break;
            case 16:
                tE.decryptText();
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
}
