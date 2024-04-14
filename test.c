#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main(void)
{
    int i, j;
    char c, ship[100][100];
    int n = 99, m = 58;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            ship[i][j] = '#';
        }
    }// i know curly brackets are not needed but i want to have them if my partner wants to add something and save him time

    puts("\n");
    printf("\t     ");
    for (c = 'A'; c < 'A' + m; c++) 
    {
        if (c > 'Z') {
            printf("%c%c ", (c - 'A') / 26 + 'A' - 1, (c - 'A') % 26 + 'A');
        } else {
            printf("%c  ", c);
        }
    }
    putchar('\n');
    printf("\t    ");
    for (c = 'A'; c < 'A' + m; c++)
    {
        printf("___");
    }
    putchar('\n');
    
    for(i = 0; i < n; i++)
    {
        printf("\t%2d |", i + 1); 

        for(j = 0; j < m; j++)
        {
            printf(" %c ", ship[i][j]); 
        }

        putchar('\n');
    }

    
}
