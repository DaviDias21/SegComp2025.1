#ifndef ATTACK_H_INCLUDED
#define ATTACK_H_INCLUDED

#define MAXTXT 500

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

    //Counts how many characters it has gone through in loop of line 54
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

#endif // ATTACK_H_INCLUDED
