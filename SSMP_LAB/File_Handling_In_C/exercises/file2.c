#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handleByte(char operand[], char objCode[])
{
    if (operand[0] == 'C')
    {
        int len = strlen(operand) - 3;
        printf("%d\n", len);
        for (int i = 0; i < len; i++)
        {
            char hex[3];
            sprintf(hex, "%02X", operand[i + 2]);
            strcat(objCode, hex);
        }
    }
    else if (operand[0] == 'X')
    {
        strncpy(objCode, operand + 2, strlen(operand) - 3);
    }
}

int main()
{
    char objcode[50] = "";
    handleByte("C'1F4B'", objcode);
    printf("BYTE C'1F4B' -> %s\n", objcode);

    strcpy(objcode, "");
    handleByte("X'454F46'", objcode);
    printf("BYTE X'454F46' -> %s\n", objcode);
    return 0;
}