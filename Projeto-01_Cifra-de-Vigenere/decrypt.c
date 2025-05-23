#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXTXT 500

int keysize();
int isRepeated();
void displayFrequency();

void keyletters();
void printFrequency();
void findFrequency();

int main(){

    char cipherTextMessage[MAXTXT];
    float pt_frequency_list[26] = {14.6, 1.0, 3.9, 5.0, 12.6, 1.0, 1.3, 1.3, 6.2, 0.4, 0.0, 2.8, 4.7, 5.0, 10.7, 2.5, 1.2, 6.5, 7.8, 4.3, 4.6, 1.7, 0.0, 0.2, 0.0, 0.5};
    float en_frequency_list[26] = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.2, 0.8, 4.0, 2.4, 6.7, 7.5, 1.9, 0.1, 6.0, 6.3, 9.1, 2.8, 1.0, 2.4, 0.2, 2.0, 0.1};

    FILE *inputFilePointer;
    char inputFileName[] = "ciphertext.txt";
    inputFilePointer = fopen(inputFileName, "r");

    if(inputFilePointer==NULL)
    {
        printf("ERROR: Input file not found.");
        exit(1);
    }
    fscanf(inputFilePointer, "%[^\n]", cipherTextMessage);


    int key_size = keysize(cipherTextMessage);

    printf("Choose language: [p, e] ");
    char language;
    scanf(" %c", &language);
    if(language == 'p'){
        keyletters(cipherTextMessage, pt_frequency_list, key_size);
    } else if(language == 'e'){
        keyletters(cipherTextMessage, en_frequency_list, key_size);
    }
    
    exit(0);
    return 0;
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
    printf("Selecione o tamanho da senha: ");
    scanf("%d", &key);
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

void keyletters(char cText[MAXTXT], float fList[26], int keySize){

    float cipher_text_frequency[26];

    char key[20];
    for(int i=0; i<20; i++){
        key[i] = '_';
    }
    char letters[26] = {'A', 'Z', 'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B'};

    int cur_shift = 0;
    int cur_key = 0;
    findFrequency(cText, cipher_text_frequency, keySize, cur_key);
    while(key[keySize-1] == '_'){
        printf("\nCIPHER KEY\n");
        for(int i=0; i<keySize; i++){
            printf(" %c ", key[i]);
        }
        printf("\n\nLETTER FREQUENCY\n");
        printFrequency(fList, 0);
        printf("\n\nCIPHER LETTER FREQUENCY\n");
        printFrequency(cipher_text_frequency, cur_shift);
        printf("\n'a' para mover para a esquerda. 'd' para mover para a direita. 'k' para confirmar.\n");
        char shift;
        scanf(" %c", &shift);
        if(shift=='a'){
            cur_shift++;
        } else if(shift=='d'){
            cur_shift--;
        } else if(shift=='k'){
            key[cur_key++] = letters[cur_shift];
            cur_shift = 0;
            findFrequency(cText, cipher_text_frequency, keySize, cur_key);
        }
        if(cur_shift==26){
            cur_shift = 0;
        } else if(cur_shift==-1){
            cur_shift = 25;
        }
    }

    printf("Chave da Cifra: ");
    for(int i=0; i<keySize; i++){
        printf("%c", key[i]);
    }
}

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
    while(cText[counter] != '\0'){
        counter++;
    }
    for(int i=0; i<26; i++){
        cFrequency[i] = 0;
    }
    while(keyIndex < counter){
        if(isalpha(cText[keyIndex])){
            cFrequency[tolower(cText[keyIndex]) - 'a'] += 1;
        }
        keyIndex += keySize;
    }
    int total = 0;
    for(int i=0; i<26; i++){
        total += cFrequency[i];
    }
    for(int i=0; i<26; i++){
        cFrequency[i] *= 100;
        cFrequency[i] /= total;
    }
}