#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main(void)
{
    char *c = "AA11";
    int a;

    a = atoi(c + 2); // douleuei

    printf("%d\n", a);

}
