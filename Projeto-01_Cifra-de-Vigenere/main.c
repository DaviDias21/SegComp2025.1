#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

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
char generateCharFromVigenere(char plainTextChar, char cipherChar)
{
    char resultChar;
    if(isCharInTheAlphabet(plainTextChar))
    {
        resultChar = getCharFromNumber((getNumberFromChar(plainTextChar) + getNumberFromChar(cipherChar)) % (25));
    }
    else
    {
        resultChar = plainTextChar;
    }
    return resultChar;
}

int main()
{
    //Variable declaration
    char plainTextMessage[500];
    char lc_plainTextMessage[500];

    char vigenereKey[20];
    char lc_vigenereKey[20];
    int keySize;
    int keyIndex = 0;

    char cipherTextMessage[500];

    //Takes plaintext from file named "input.txt" in the same directory as .c file
    FILE *inputFilePointer;
    char inputFileName[] = "input.txt";
    inputFilePointer = fopen(inputFileName, "r");

    //ERROR if the file does not exist
    if(inputFilePointer==NULL)
    {
        printf("ERROR: Input file not found.");
        exit(1);
    }
    fscanf(inputFilePointer, "%[^\n]", plainTextMessage);

    //Sets the plaintext to lowercase, for future character frequency analysis
    for(int i = 0; i < strlen(plainTextMessage); i++)
    {
        lc_plainTextMessage[i] = tolower(plainTextMessage[i]);
    }
    lc_plainTextMessage[strlen(plainTextMessage)]='\0';

    //User inputs desired Vigenere key
    printf("Please input desired key (max 20 characters): ");
    scanf("%[^\n]", vigenereKey);
    keySize = strlen(vigenereKey);

    //Sets the inputted key to lowercase, to avoid errors
    for(int i = 0; i < keySize; i++)
    {
        lc_vigenereKey[i] = tolower(vigenereKey[i]);
    }
    lc_vigenereKey[keySize]='\0';

    //Ciphers plaintext (lowercase) one character at a time
    for(int j=0;j<strlen(lc_plainTextMessage);j++)
    {
        cipherTextMessage[j] = generateCharFromVigenere(lc_plainTextMessage[j], lc_vigenereKey[keyIndex]);
        //Key wrap-around
        if(keyIndex + 1 >= keySize){keyIndex = 0;}
        else{keyIndex++;}
    }
    cipherTextMessage[strlen(plainTextMessage)]='\0';

    //Closes "input.txt" file
    fclose(inputFilePointer);

    printf("Plaintext message:\n\n%s", plainTextMessage);
    printf("\n\nCiphertext message (shifted lowercase):\n\n%s\n\n", cipherTextMessage);

    return 0;
}
