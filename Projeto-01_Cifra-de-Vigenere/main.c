#include <stdio.h>
#include <stdlib.h>

int main()
{
    char input[];

    FILE *fp;
    char inputFileName[] = "input.txt";
    fp = fopen(inputFileName, "r");

    if(fp==NULL)
    {
        printf("ERROR: Input file not found.");
        exit(1);
    }


    fclose(fp);
}
