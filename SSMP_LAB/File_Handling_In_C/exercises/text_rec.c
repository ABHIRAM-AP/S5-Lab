#include <stdio.h>
#include <string.h>

int main()
{
    char objcodes[][30] = {"A", "B", "C", "D", "E", "B", "F", "G", "H", "E", "N", "Q"};
    char textRec[200] = "";
    int textLen = 0, maxLen = 30;

    int recStart = 0x1000;

    for (int i = 0; i < 13; i++)
    {
        if (textLen + 3 > maxLen)
        { // 3 bytes per code
            printf("T^%06X^%02X%s\n", recStart, textLen, textRec);
            textRec[0] = '\0';
            textLen = 0;
            recStart += textLen;
        }
        strcat(textRec, "^");
        strcat(textRec, objcodes[i]);
        textLen += 3;
    }
    if (textLen > 0)
        printf("T^%06X^%02X%s\n", recStart, textLen, textRec);
    return 0;
}
