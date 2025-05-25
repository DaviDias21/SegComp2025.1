#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED

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
char generateCharFromVigenere(char plainTextChar, char keyChar)
{
    char notAccentedChar;
    char resultChar;

    if (plainTextChar == 'á' || plainTextChar == 'à' || plainTextChar == 'â' || plainTextChar == 'ã') {notAccentedChar = 'a';}
    else if (plainTextChar == 'é' || plainTextChar == 'ê' )                                           {notAccentedChar = 'e';}
    else if (plainTextChar == 'í')                                                                    {notAccentedChar = 'i';}
    else if (plainTextChar == 'ó' || plainTextChar == 'ô' || plainTextChar == 'õ')                    {notAccentedChar = 'o';}
    else if (plainTextChar == 'ú' || plainTextChar == 'ü')                                            {notAccentedChar = 'u';}
    else if (plainTextChar == 'ç')                                                                    {notAccentedChar = 'c';}
    else {notAccentedChar = plainTextChar;}

    if(isCharInTheAlphabet(notAccentedChar))
    {
        resultChar = getCharFromNumber(((getNumberFromChar(notAccentedChar) + getNumberFromChar(keyChar)) + 26) % (26));
    }
    else
    {
        resultChar = notAccentedChar;
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


#endif // ENCRYPT_H_INCLUDED
