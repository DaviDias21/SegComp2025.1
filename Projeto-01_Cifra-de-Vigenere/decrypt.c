#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXTXT 500

int keysize();
int isRepeated();
void displayFrequency();

void keyletters();
void printFrequency();

int main(){

    char cipherTextMessage[MAXTXT];
    float frequency_list[26] = {14.6, 1.0, 3.9, 5.0, 12.6, 1.0, 1.3, 1.3, 6.2, 0.4, 0.0, 2.8, 4.7, 5.0, 10.7, 2.5, 1.2, 6.5, 7.8, 4.3, 4.6, 1.7, 0.0, 0.2, 0.0, 0.5};

    FILE *inputFilePointer;
    char inputFileName[] = "input.txt";
    inputFilePointer = fopen(inputFileName, "r");

    if(inputFilePointer==NULL)
    {
        printf("ERROR: Input file not found.");
        exit(1);
    }
    fscanf(inputFilePointer, "%[^\n]", cipherTextMessage);


    int key_size = keysize(cipherTextMessage);
    keyletters(cipherTextMessage, frequency_list, key_size);
    
    exit(0);
    return 0;
}

int keysize(char ctext[MAXTXT]){

    int prev_group[MAXTXT][3];
    int cur_group[3];
    int index = 0;
    bool end_reached = false;
    int is_repeated = 0;

    printf("POSSIBLE KEYSIZE\n");
    printf("----------------\n");
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

    printf("LETTER FREQUENCY\n");
    printFrequency(fList, 0);

    int cur_shift = 0;
    while(true){
        char shift;
        scanf("%c", &shift);
        if(shift=='a'){
            cur_shift++;
        } else if(shift=='d'){
            cur_shift--;
        }
        if(cur_shift==26){
            cur_shift = 0;
        } else if(cur_shift==-1){
            cur_shift = 25;
        }
        printFrequency(fList, cur_shift);
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