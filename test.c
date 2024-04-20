#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main(void)
{
    char c[] = "BB11";
    int a;

    c[0] = c[0] - 'A' + '0';
    c[1] = c[1] - 'A' + '0';
    //a = atoi(c + 2); // douleuei
    a = atoi(c); // den douleuei
    printf("%d\n", a);

}
