#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main(void)
{
    char c[] = "Z";
    int a;

    // c[0] = c[0] - 'A' + '0';
    // c[1] = c[1] - 'A' + '0';
    //a = atoi(c + 2); // douleuei
    a = c[0] -'A';  //strtol(c - '0', NULL, 10); // den douleuei
    printf("%d\n", a);

}
