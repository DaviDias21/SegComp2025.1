#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>

#define MAXTXT 500
#define MAXKEYSIZE 20

bool userIsDone = false;

char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";
char guessedPlaintextMessage[MAXTXT];

FILE *plainTextFilePointer;
FILE *cipherTextFilePointer;

char plainTextMessage[MAXTXT];
char lc_plainTextMessage[MAXTXT];

char vigenereKey[MAXKEYSIZE];
char lc_vigenereKey[MAXKEYSIZE];

int keySize;
int keyIndex = 0;

char cipherTextMessage[MAXTXT];

int keysize();
int isRepeated();
void displayFrequency();

void keyletters();
void printFrequency();
void findFrequency();
bool isUserSatisfied = false;

//Array with most used letters in Brazilian Portuguese for reference
float pt_frequency_list[26] = {14.6, 1.0, 3.9, 5.0, 12.6, 1.0, 1.3, 1.3, 6.2, 0.4, 0.0, 2.8, 4.7, 5.0, 10.7, 2.5, 1.2, 6.5, 7.8, 4.3, 4.6, 1.7, 0.0, 0.2, 0.0, 0.5};
//Array with most used letters in English for reference
float en_frequency_list[26] = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.2, 0.8, 4.0, 2.4, 6.7, 7.5, 1.9, 0.1, 6.0, 6.3, 9.1, 2.8, 1.0, 2.4, 0.2, 2.0, 0.1};

/*In the Vigenere chart linked in the project requirements,
the 'a' character shifts a plaintext character by a factor of 0,
while the 'z' character shifts a plaintext character by a factor of 25,
so we will consider a character's position in the alphabet its index in the alphabet string
(Instead of using 1 to 26, we will use 0 to 25).*/
//Finds a character's position in the alphabet
int getNumberFromChar(char ch)
{
    int i=0;
    while(ch != alphabet[i]){i++;}
    return i;
}

//Finds a character from its position in the alphabet
char getCharFromNumber(int num)
{
    return alphabet[num];
}

//Checks if a character is in the alphabet string
bool isCharInTheAlphabet(char ch)
{
    bool isChar = false;
    for(int i=0;i<strlen(alphabet);i++)
    {
        if(alphabet[i] == ch){isChar = true;}
    }
    return isChar;
}

/*If a character is in the alphabet string, calculates and returns Vigenere of that character.
If the character is NOT in the alphabet, returns character as is. This maintains apostrophes, spaces, punctuation, etc.
in the same positions as in the plaintext.*/
char generateCharFromVigenere(char plainTextChar, char keyChar)
{
    char resultChar;
    if(isCharInTheAlphabet(plainTextChar))
    {
        resultChar = getCharFromNumber(((getNumberFromChar(plainTextChar) + getNumberFromChar(keyChar)) + 26) % (26));
    }
    else
    {
        resultChar = plainTextChar;
    }
    return resultChar;
}

char reverseGenerateCharFromVigenere(char cipherTextChar, char possibleKeyChar)
{
    char resultChar_;
    if(isCharInTheAlphabet(cipherTextChar))
    {
        resultChar_ = getCharFromNumber(((getNumberFromChar(cipherTextChar) - getNumberFromChar(possibleKeyChar)) + 26) % (26));
    }
    else
    {
        resultChar_ = cipherTextChar;
    }
    return resultChar_;
}

void vigenere()
{
    system("cls");
    printf("You are now encrypting.\n\n");
    //Takes plaintext from text file
    char plainTextFileName[] = "plaintext.txt";
    plainTextFilePointer = fopen(plainTextFileName, "r");
    //ERROR if the file does not exist
    if(plainTextFilePointer==NULL)
    {
        printf("ERROR: Input file not found.");
        exit(1);
    }
    fscanf(plainTextFilePointer, " %[^\n]", plainTextMessage);

    //Sets the plaintext to lowercase, for future character frequency analysis
    for(int i = 0; i < strlen(plainTextMessage); i++)
    {
        lc_plainTextMessage[i] = tolower(plainTextMessage[i]);
    }
    lc_plainTextMessage[strlen(plainTextMessage)]='\0';

    //User inputs desired Vigenere key
    printf("Please input desired key (max 20 characters): ");
    scanf(" %[^\n]", vigenereKey);
    keySize = strlen(vigenereKey);

    //Sets the inputted key to lowercase, to avoid errors
    for(int i = 0; i < keySize; i++)
    {
        lc_vigenereKey[i] = tolower(vigenereKey[i]);
    }
    lc_vigenereKey[keySize]='\0';

    //Ciphers plaintext (lowercase) one character at a time
    keyIndex=0;
    for(int j=0;j<strlen(lc_plainTextMessage);j++)
    {
        cipherTextMessage[j] = generateCharFromVigenere(lc_plainTextMessage[j], lc_vigenereKey[keyIndex]);
        //Key wrap-around
        if(keyIndex + 1 >= keySize){keyIndex = 0;}
        else{keyIndex++;}
    }
    cipherTextMessage[strlen(plainTextMessage)]='\0';

    //Closes "plaintext.txt" file
    fclose(plainTextFilePointer);

    printf("Plaintext message:\n\n%s", plainTextMessage);
    printf("\n\nCiphertext message (shifted lowercase):\n\n%s\n\n", cipherTextMessage);

    printf("Press ENTER to return to menu.");
    char input = getchar();
    input = getchar();

    char cipherTextFileName[] = "ciphertext.txt";
    cipherTextFilePointer = fopen(cipherTextFileName, "w");
    if (cipherTextFilePointer != NULL)
    {
        fputs(cipherTextMessage, cipherTextFilePointer);
        fclose(cipherTextFilePointer);
    }
}

void reverseVigenere(char cipherText[MAXTXT], char possibleKey[MAXKEYSIZE], int guessedKeySize_)
{
    int keyIndex_ = 0;
    printf("\nPossible plaintext from key:\n");
    for(int j=0;j<strlen(cipherText);j++)
    {
        guessedPlaintextMessage[j] = reverseGenerateCharFromVigenere(cipherText[j], tolower(possibleKey[keyIndex_]));
        printf("%c", guessedPlaintextMessage[j]);
        //Key wrap-around
        if(keyIndex_ + 1 >= guessedKeySize_){keyIndex_ = 0;}
        else{keyIndex_++;}
    }
    guessedPlaintextMessage[strlen(cipherText)]='\0';

    printf("\n");
}

int keysize(char ctext[MAXTXT]){

    int prev_group[MAXTXT][3];
    int cur_group[3];
    int index = 0;
    bool end_reached = false;
    int is_repeated = 0;

    printf("POSSIBLE KEYSIZES\n");
    printf("-----------------\n");
    printf("|| Repeated Sequence || Spacing || 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |\n");
    /* 22 | 12 | 4 */

    while(true){
        bool has_space = false;
        for(int i=0; i<3; i++){
            cur_group[i] = ctext[index+i];
            if(cur_group[i] == ' ' || cur_group[i] == '\n' || cur_group[i] == '\t'){
                has_space = true;
            }
        }
        if(cur_group[2] == '\0'){
            break;
        }
        if(has_space){
            ++index;
            continue;
        }
        is_repeated = isRepeated(prev_group, cur_group, index);
        if(is_repeated){
            displayFrequency(cur_group, is_repeated);;
        }
        for(int i=0; i<3; i++){
            prev_group[index][i] = cur_group[i];
        }
        ++index;
    }

    int key;
    printf("Input key size guess: ");
    scanf(" %d", &key);
    return key;
}

int isRepeated(int pgroup[MAXTXT][3], int cgroup[3], int i){
    int repeated_index = -1;
    for(int u=0; u<i; u++){
        bool is_same = true;
        for(int j=0; j<3; j++){
            if (pgroup[u][j] != cgroup[j]){
                is_same = false;
            }
        }
        if(is_same){
            repeated_index = u;
        }
    }
    if(repeated_index >= 0){
        return i-repeated_index;
    }
    return 0;
}

char x(int distance, int num){
        if(distance%num == 0){
            return 'x';
        }
        return ' ';
    }

void displayFrequency(int cgroup[3], int d){
    printf("%11c%c%c%16d%7c%4c%4c%4c%4c%4c%4c%4c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c\n", cgroup[0], cgroup[1], cgroup[2], d, x(d, 2), x(d, 3), x(d, 4), x(d, 5), x(d, 6), x(d, 7), x(d, 8), x(d, 9), x(d, 10), x(d, 11), x(d, 12), x(d, 13), x(d, 14), x(d, 15), x(d, 16), x(d, 17), x(d, 18), x(d, 19), x(d, 20));
}

void keyletters(char cText[MAXTXT], float fList[26], int guessedKeySize){

    float cipher_text_frequency[26];

    //Creates array for possible key, fills it with the '_' character, as the guess is initially empty
    char key[20];
    for(int i=0; i<20; i++){
        key[i] = '_';
    }

    char letters[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    //Integers for current shift in frequency displays and current starting key for analysis
    int cur_shift = 0;
    int cur_key = 0;

    /*Finds frequencies of characters in ciphertext,
    considering user's guess for how long the key is*/
    findFrequency(cText, cipher_text_frequency, guessedKeySize, cur_key);


    while(key[guessedKeySize-1] == '_'){
        system("cls");
        printf("\nCIPHER KEY\n");

        for(int i=0; i<guessedKeySize; i++){
            printf(" %c ", key[i]);
        }

        /*Prints both the expected frequency given the language of plaintext message
        as well as the found frequency in ciphertext*/

        printf("\n\nLETTER FREQUENCY\n");
        printFrequency(fList, 0);

        printf("\n\nCIPHER LETTER FREQUENCY\n");
        printFrequency(cipher_text_frequency, cur_shift);

        //Takes user input
        printf("\nPress 'a' to offset frequencies to the left,\n'd' to offset frequencies to the right\nor 'k' to confirm choice.\n\nThen, press 'ENTER'\n");
        char shift;
        scanf(" %c", &shift);

        if(shift=='a'){
            cur_shift++;
        } else if(shift=='d'){
            cur_shift--;
        } else if(shift=='k'){
            /*If user confirms choice, fills exactly one index of the key guess array
            with whichever character would cause that shift.
            For this, uses the 'letters' array, which is filled backwards because of how the offset works*/
            key[cur_key++] = letters[cur_shift];

            //Resets offset then runs findFrequency again, starting one letter later
            cur_shift = 0;
            findFrequency(cText, cipher_text_frequency, guessedKeySize, cur_key);
        }

        if(cur_shift==26){
            cur_shift = 0;
        } else if(cur_shift==-1){
            cur_shift = 25;
        }
    }
    system("cls");
    printf("Possible key: ");
    for(int i=0; i<guessedKeySize; i++){
        printf("%c", key[i]);
    }

    reverseVigenere(cText, key, guessedKeySize);
}

/*Goes through expected frequency array,
printing the frequencies in decreasing order*/
void printFrequency(float frequency[26], int start){
    for(int i=15; i>=0; i--){
        for(int u=start; u<26; u++){
            if (frequency[u] >= i){
                printf(" --- ");
            } else{
                printf("     ");
            }
        }
        for(int u=0; u<start; u++){
            if (frequency[u] >= i){
                printf(" --- ");
            } else{
                printf("     ");
            }
        }
        printf("\n ");
    }
    for(int i=start; i<26; i++){
        printf("%4.1f ", frequency[i]);
    }
    for(int i=0; i<start; i++){
        printf("%4.1f ", frequency[i]);
    }
}

void findFrequency(char cText[MAXTXT], float cFrequency[26], int keySize, int keyIndex){
    int counter = 0;

    /*Increments counter until it reaches the end of the ciphertext string, i.e.,
      calculates the size of the ciphertext message*/
    while(cText[counter] != '\0'){
        counter++;
    }

    //Clears array with frequency of each character
    for(int i=0; i<26; i++){
        cFrequency[i] = 0;
    }

    /*Runs through ciphertext array starting at keyIndex,
    skipping however many characters the user guessed the key size is,
    and counts how many of each character in the alphabet it encounters,
    putting them in the cFrequency array*/
    while(keyIndex < counter){
        if(isalpha(cText[keyIndex])){
            cFrequency[tolower(cText[keyIndex]) - 'a'] += 1;
        }
        keyIndex += keySize;
    }

    //Counts how many characters it has gone through in loop of line 373
    int total = 0;
    for(int i=0; i<26; i++){
        total += cFrequency[i];
    }

    //Substitutes character count in each cFrequency array element by it's percentage of total characters
    for(int i=0; i<26; i++){
        cFrequency[i] *= 100;
        cFrequency[i] /= total;
    }
}
void attack()
{
    while(!isUserSatisfied)
    {
        system("cls");
        printf("You are now attacking the cipher.\n");
        //Opens and reads file named ciphertext.txt
        FILE *inputFilePointer;
        char inputFileName[] = "ciphertext.txt";
        inputFilePointer = fopen(inputFileName, "r");

        //If there is no ciphertext file, reports error and ends execution
        if(inputFilePointer==NULL)
        {
            printf("ERROR: Ciphertext file not found.");
            isUserSatisfied = true;
        }
        //Scans ciphertext file until it finds '\n'
        fscanf(inputFilePointer, " %[^\n]", cipherTextMessage);
        printf("\nCiphertext message (shifted lowercase):\n%s\n\n", cipherTextMessage);

        //Runs keysize function and saves value the user inputted (a guess to the true key size)
        int key_size = keysize(cipherTextMessage);

        printf("Choose language: [p, e] ");
        char language;
        scanf(" %c", &language);
        if(language == 'p'){
                keyletters(cipherTextMessage, pt_frequency_list, key_size);
            } else if(language == 'e'){
                keyletters(cipherTextMessage, en_frequency_list, key_size);
            }
        printf("Are you satisfied with this result?\n[Type 'y' for YES or a different key for NO, then press ENTER]\n");
        char userInput;
        scanf(" %c", &userInput);
        fclose(inputFilePointer);
        if(userInput == 'y')
        {
            isUserSatisfied = true;
        }
    }
}

int main()
{
    while(!userIsDone)
    {
        system("cls");
        printf("You are now in the main menu.\n\n");
        printf("What do you wish to do?\n1. Encrypt using the Vigenere cipher\n2. Attack the Vigenere cipher\n0. EXIT\n");
        int userInput;
        scanf(" %d", &userInput);
        if(userInput == 0)
            userIsDone = true;
        else if(userInput == 1)
            vigenere();
        else if(userInput == 2)
            attack();
    }
    return 0;
}
