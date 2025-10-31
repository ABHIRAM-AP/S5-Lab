#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200
#define MIN 20

void decode(char line[], char loc[], char opcode[], char operand[])
{
    char strings[4][MIN];
    int count = sscanf(line, "%s %s %s %s", strings[0], strings[1], strings[2], strings[3]);

    strcpy(loc, strings[0]);
    if (count == 2)
    {
        strcpy(opcode, strings[1]);
        strcpy(operand, "");
    }
    else if (count == 3)
    {
        strcpy(opcode, strings[1]);
        strcpy(operand, strings[2]);
    }
    else
    {
        strcpy(opcode, strings[2]);
        strcpy(operand, strings[3]);
    }
}
int searchSymtab(FILE *fsymtab, char label[], char symAddr[])
{
    char symbol[MIN], line[MAX];
    rewind(fsymtab);
    fgets(line, sizeof(line), fsymtab);
    while (fscanf(fsymtab, "%s %s", symbol, symAddr) != EOF)
    {
        if (strcmp(symbol, label) == 0)
            return 1;
    }
    return 0;
}
int searchOptab(FILE *foptab, char opcode[], char hexCode[])
{
    char mnemonic[MIN], line[MAX];
    rewind(foptab);
    fgets(line, sizeof(line), foptab);
    while (fscanf(foptab, "%s %s", mnemonic, hexCode) != EOF)
    {
        if (strcmp(mnemonic, opcode) == 0)
            return 1;
    }
    return 0;
}
int main()
{
    FILE *fin = fopen("intermediate.txt", "r");
    FILE *foptab = fopen("optab.txt", "r");
    FILE *fsym = fopen("symtab.txt", "r");
    FILE *flen = fopen("length.txt", "r");

    FILE *flisting = fopen("listing.txt", "w");
    FILE *fout = fopen("record.txt", "w");

    char line[MAX], loc[MAX], opcode[MAX], operand[MAX], symAddr[MAX], hexCode[MAX],
        name[MAX], length[MAX], text[MAX], objCode[MAX], start[MAX], startObj[MAX];

    fgets(line, sizeof(line), fin);
    line[strcspn(line, "\n")] = '\0';
    fprintf(flisting, "%s%-12s\n", line, "Object Code");
    fscanf(flen, "%s", length);

    fscanf(fin, "%s %s %s %s\n", start, name, opcode, operand);
    fprintf(flisting, "%-6s%-10s%-10s%-10s\n", start, name, opcode, operand);
    fprintf(fout, "H^%-6s^%06X^%06X\n", name, (int)strtol(start, NULL, 16), (int)strtol(length, NULL, 16));

    fgets(line, sizeof(line), fin);
    decode(line, loc, opcode, operand);

    int count = 0, newText = 0, found;
    strcpy(startObj, start);
    while (strcmp(opcode, "END") != 0)
    {
        strcpy(objCode, "-1");
        found = searchOptab(foptab, opcode, hexCode);
        if (found)
        {
            strcpy(objCode, hexCode);
            if (strcmp(operand, "") != 0)
            {
                if (searchSymtab(fsym, operand, symAddr))
                {
                    strcat(objCode, symAddr);
                }
                else
                {
                    printf("no symbol");
                    exit(1);
                }
            }
            else
            {
                sprintf(objCode + 2, "%04X", 0);
            }
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            sprintf(objCode, "%06X", atoi(operand));
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if (operand[0] == 'C')
            {
                int j = 0;
                for (int i = 2; i < strlen(operand) - 1; i++)
                {
                    sprintf(objCode + j, "%02X", operand[i]);
                    j += 2;
                }
            }
            else
            {
                for (int i = 2; i < strlen(operand) - 1; i++)
                {
                    sprintf(objCode + i - 2, "%c", operand[i]);
                }
            }
        }
        else
        {
            newText = 1;
        }
        if (strcmp(objCode, "-1") != 0)
        {
            line[strcspn(line, "\n")] = '\0';
            fprintf(flisting, "%s%-12s\n", line, objCode);
            if (strlen(text) + strlen(objCode) - count > 60 || newText)
            {
                text[strlen(text) - 1] = '\0';
                fprintf(fout, "T^%06X^%02X^%s\n", (int)strtol(startObj, NULL, 16), ((strlen(operand) - count + 1)) / 2, text);
                newText = 0;
                count = 0;
                strcpy(text, "");
                strcpy(startObj, loc);
            }
            if (count == 0)
                strcpy(text, objCode);
            else
                strcat(text, objCode);
            count++;
            strcat(text, "^");
        }
        else
        {
            fprintf(flisting, "%s", line);
        }
        fgets(line, sizeof(line), fin);
        decode(line, loc, opcode, operand);
    }
    fprintf(flisting, "%s", line);
    if (count != 0)
    {
        text[strlen(text) - 1] = '\0';
        fprintf(fout, "T^%06X^%02X^%s\n", (int)strtol(startObj, NULL, 16), ((strlen(operand) - count + 1)) / 2, text);
    }
    if (strcmp(operand, "") != 0)
    {
        if (searchSymtab(fsym, operand, symAddr))
        {
            strcpy(start, symAddr);
        }
        else
        {
            strcpy(start, operand);
        }
    }
    fprintf(fout, "E^%06X", (int)strtol(start, NULL, 16));
}