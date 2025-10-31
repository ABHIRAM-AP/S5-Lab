#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *searchOptab(char opo[])
{
    FILE *foptab = fopen("optab.txt", "r");
    char opcode[20];
    static char value[20];
    if (!foptab)
    {
        printf("Error opening optab.txt\n");
        exit(1);
    }
    while (fscanf(foptab, "%s %s", opcode, value) != EOF)
    {
        if (strcmp(opcode, opo) == 0)
        {
            fclose(foptab);
            return value;
        }
    }
    fclose(foptab);
    return NULL;
}

void split(char line[])
{

    FILE *fout = fopen("split1.txt", "a+");
    if (!fout)
    {
        printf("Error opening split1.txt\n");
        exit(1);
    }

    char label[20], opcode[20], operand[20];
    int n = sscanf(line, "%s %s %s", label, opcode, operand);

    if (n == 3)
    {
        char *code = searchOptab(opcode);
        if (code != NULL)
            fprintf(fout, "Label=%-8s Opcode=%-8s Operand=%-8s Code=%s\n", label, opcode, operand, code);
        else
            fprintf(fout, "Label=%-8s Opcode=%-8s Operand=%-8s Code=--\n", label, opcode, operand);
    }
    else if (n == 2)
    {
        char *code = searchOptab(label);
        if (code != NULL)
            fprintf(fout, "Opcode=%-8s Operand=%-8s Code=%s\n", label, opcode, code);
        else
            fprintf(fout, "Opcode=%-8s Operand=%-8s Code=--\n", label, opcode);
    }
    else if (n == 1)
    {
        char *code = searchOptab(label);
        if (code != NULL)
            fprintf(fout, "Operand=%-8s Code=%s\n", label, code);
        else
            fprintf(fout, "Operand=%-8s Code=--\n", label);
    }

    fclose(fout);
}

void read(FILE *fin)
{
    char line[200];
    while (fgets(line, sizeof(line), fin))
    {
        printf("%s", line);
        split(line);
    }
}

int main()
{
    FILE *fin = fopen("asm.txt", "r");
    if (!fin)
    {
        printf("Error\n");
        exit(1);
    }
    read(fin);
    fclose(fin);

    return 0;
}