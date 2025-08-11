#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char opcode[10], operand[10], label[10];
char code[10], mnemonic[10], sym_label[10];
int loc;
char sym_addr[10];

void passTwo()
{
    FILE *inter, *optab, *symtab, *output;
    inter = fopen("intermediate.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    output = fopen("output.txt", "w");

    fscanf(inter, "%s%s%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(inter, "%d%s%s%s", &loc, label, opcode, operand);
    }

    while (strcmp(opcode, "END") != 0)
    {
        int found = 0;
        rewind(optab);
        while (fscanf(optab, "%s%s", code, mnemonic) != EOF)
        {
            if (strcmp(opcode, code) == 0)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            rewind(symtab);
            while (fscanf(symtab, "%s%s", sym_label, sym_addr) != EOF)
            {
                if (strcmp(operand, sym_label) == 0)
                {
                    fprintf(output, "%d\t%s\t%s\t%s\t%s%s\n", loc, label, opcode, operand, mnemonic, sym_addr);
                    break;
                }
            }
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            int val = atoi(operand);
            fprintf(output, "%d\t%s\t%s\t%s\t%06X\n", loc, label, opcode, operand, val);
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if (operand[0] == 'C')
            {
                char hex[20] = "";
                for (int i = 2; i < strlen(operand) - 1; i++)
                {
                    char temp[3];
                    sprintf(temp, "%X", operand[i]);
                    strcat(hex, temp);
                }
                fprintf(output, "%d\t%s\t%s\t%s\t%s\n", loc, label, opcode, operand, hex);
            }
        }
        else
        {
            // RESW, RESB, etc.
            fprintf(output, "%d\t%s\t%s\t%s\t\n", loc, label, opcode, operand);
        }

        fscanf(inter, "%d%s%s%s", &loc, label, opcode, operand);
    }

    fprintf(output, "%d\t%s\t%s\t%s\n", loc, label, opcode, operand);

    fclose(inter);
    fclose(optab);
    fclose(symtab);
    fclose(output);

    printf("Pass Two completed. Output written to output.txt\n");
}

int main()
{
    passTwo();
    return 0;
}
