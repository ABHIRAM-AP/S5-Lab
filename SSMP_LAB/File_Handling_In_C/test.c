#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200
#define MIN 20

void decode(char line[], char label[], char opcode[], char operand[])
{
    char strings[3][MIN];
    int count = sscanf(line, "%s %s %s", strings[0], strings[1], strings[2]);

    if (count == 1)
    {
        strcpy(label, "");
        strcpy(opcode, strings[0]);
        strcpy(operand, "");
    }
    else if (count == 2)
    {
        strcpy(label, "");
        strcpy(opcode, strings[0]);
        strcpy(operand, strings[1]);
    }
    else
    {
        strcpy(label, strings[0]);
        strcpy(opcode, strings[1]);
        strcpy(operand, strings[2]);
    }
}
int searchSymtab(FILE *fsymtab, char label[], char symAddr[])
{
    rewind(fsymtab);
    char symbol[MAX], line[MAX];
    fgets(line, sizeof(line), fsymtab);
    while (fscanf(fsymtab, "%s %s", symbol, symAddr) != EOF)
    {
        if (strcmp(label, symbol) == 0)
            return 1;
    }
    return 0;
}

int searchOptab(FILE *foptab, char opcode[], char hexCode[])
{
    rewind(foptab);
    char mnemonic[MAX], line[MAX];
    fgets(line, sizeof(line), foptab);
    while (fscanf(foptab, "%s %s", mnemonic, hexCode) != EOF)
    {
        if (strcmp(opcode, mnemonic) == 0)
            return 1;
    }
    return 0;
}
int main()
{
    FILE *fin = fopen("input.txt", "r");
    FILE *fout = fopen("symtab.txt", "w");
    FILE *fsym = fopen("symtab.txt", "r");
    FILE *foptab = fopen("optab.txt", "r");

    FILE *finter = fopen("intermediate.txt", "w");
    FILE *flen = fopen("length.txt", "w");

    fprintf(fout, "%-8s%s\n", "Label", "LOC");
    fprintf(finter, "%-6s%-10s%-10s%-10s\n", "LOC", "Label", "Opcode", "Operand");
    char line[MAX], label[MAX], opcode[MAX], operand[MAX], symAddr[MAX], hexCode[MAX];
    int locctr = 0x0, start = 0x0, found;
    fgets(line, sizeof(line), fin);
    fgets(line, sizeof(line), fin);
    decode(line, label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        locctr = start = (int)strtol(operand, NULL, 16);
        fprintf(finter, "%04X\t%-10s%-10s%-10s\n", locctr, label, opcode, operand);
        fgets(line, sizeof(line), fin);
        decode(line, label, opcode, operand);
    }
    while (strcmp(opcode, "END") != 0)
    {
        if (strcmp(label, "") != 0)
        {
            if (searchSymtab(fsym, label, symAddr))
            {
                printf("dupl");
                exit(1);
            }
            fprintf(fout, "%-8s%04X\n", label, locctr);
        }
        found = searchOptab(foptab, opcode, hexCode);
        fprintf(finter, "%04X\t%-10s%-10s%-10s\n", locctr, label, opcode, operand);
        if (found || strcmp(opcode, "WORD") == 0)
            locctr += 3;
        else if (strcmp(opcode, "RESW") == 0)
            locctr += atoi(operand) * 3;
        else if (strcmp(opcode, "RESB") == 0)
            locctr += atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if (operand[0] == 'C')
                locctr += strlen(operand) - 3;
            else
                locctr += (strlen(operand) - 3) / 2;
        }
        else
        {
            printf("invalid\n");
            exit(1);
        }

        fgets(line, sizeof(line), fin);
        decode(line, label, opcode, operand);
    }
    fprintf(finter, "%04X\t%-10s%-10s%-10s\n", locctr, label, opcode, operand);
    fprintf(flen, "%X", locctr - start);
}