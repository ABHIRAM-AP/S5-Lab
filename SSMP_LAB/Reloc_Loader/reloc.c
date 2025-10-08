#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char bit[1000]; // to store expanded bitmask
char bitmask[100];

void bitmask_convert(char mask[])
{
    int len = strlen(mask);
    strcpy(bit, "");
    for (int i = 0; i < len; i++)
    {
        switch (mask[i])
        {
        case '0':
            strcat(bit, "0000");
            break;
        case '1':
            strcat(bit, "0001");
            break;
        case '2':
            strcat(bit, "0010");
            break;
        case '3':
            strcat(bit, "0011");
            break;
        case '4':
            strcat(bit, "0100");
            break;
        case '5':
            strcat(bit, "0101");
            break;
        case '6':
            strcat(bit, "0110");
            break;
        case '7':
            strcat(bit, "0111");
            break;
        case '8':
            strcat(bit, "1000");
            break;
        case '9':
            strcat(bit, "1001");
            break;
        case 'A':
        case 'a':
            strcat(bit, "1010");
            break;
        case 'B':
        case 'b':
            strcat(bit, "1011");
            break;
        case 'C':
        case 'c':
            strcat(bit, "1100");
            break;
        case 'D':
        case 'd':
            strcat(bit, "1101");
            break;
        case 'E':
        case 'e':
            strcat(bit, "1110");
            break;
        case 'F':
        case 'f':
            strcat(bit, "1111");
            break;
        }
    }
}

int main()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        printf("Cannot open input.txt\n");
        return 1;
    }

    char rec[20], name[20], objcode[10];
    int start, add, len;
    int bit_index = 0;

    printf("ENTER THE STARTING ADDRESS OF THE PROGRAM: ");
    scanf("%x", &start);

    fscanf(fp, "%s", rec);
    if (strcmp(rec, "H") != 0)
    {
        printf("Invalid object file format\n");
        return 1;
    }

    fscanf(fp, "%s %x %x", name, &add, &len);
    printf("\nPROGRAM NAME = %s\n\n", name);
    printf("ADDRESS      OBJECT CODE\n");
    printf("----------------------------\n");

    fscanf(fp, "%s", rec);
    while (strcmp(rec, "E") != 0)
    {
        if (strcmp(rec, "T") == 0)
        {
            int taddr, tlen;
            fscanf(fp, "%x %x %s", &taddr, &tlen, bitmask);
            taddr += start;
            bit_index = 0;
            bitmask_convert(bitmask);

            // Read instructions until next T/E record
            fscanf(fp, "%s", rec);
            while (rec[0] != 'T' && rec[0] != 'E')
            {
                char opcode[3], addr_str[5];
                strncpy(opcode, rec, 2);
                opcode[2] = '\0';
                strncpy(addr_str, rec + 2, 4);
                addr_str[4] = '\0';

                int operand = strtol(addr_str, NULL, 16);
                if (bit[bit_index] == '1') // relocatable
                    operand += start;

                printf("%X\t%s%04X\n", taddr, opcode, operand);

                taddr += 3; // each SIC instruction is 3 bytes
                bit_index++;
                fscanf(fp, "%s", rec);
            }
        }
        else
        {
            fscanf(fp, "%s", rec); // skip anything else
        }
    }

    fclose(fp);
    return 0;
}
