#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define MID 50

void decode(char line[], char label[], char opcode[], char operand[])
{
    char temp[3][MID];
    int count = sscanf(line, "%s %s %s", temp[0], temp[1], temp[2]);

    if (count == 1)
    {
        strcpy(label, "");
        strcpy(opcode, temp[0]);
        strcpy(operand, "");
    }
    else if (count == 2 && strcmp(temp[1], "MACRO") == 0)
    {
        strcpy(label, temp[0]);
        strcpy(opcode, temp[1]);
        strcpy(operand, "");
    }
    else if (count == 2)
    {
        strcpy(label, "");
        strcpy(opcode, temp[0]);
        strcpy(operand, temp[1]);
    }
    else if (count == 3)
    {
        strcpy(label, temp[0]);
        strcpy(opcode, temp[1]);
        strcpy(operand, temp[2]);
    }
    else
    {
        strcpy(label, "");
        strcpy(opcode, "");
        strcpy(operand, "");
    }
}

void defineMacro(FILE *fin, FILE *fnamtab, FILE *fdeftab, char label[], char operand[])
{
    fprintf(fnamtab, "%s 0 80\n", label); // simple dummy NAMTAB entry
    fprintf(fdeftab, "%s %s\n", label, operand);

    char line[MAX], l[MID], o[MID], op[MID];

    // Loop until MEND
    while (fgets(line, sizeof(line), fin))
    {
        decode(line, l, o, op);
        if (strcmp(l, "") == 0 && strcmp(o, "MEND") == 0)
        {
            fprintf(fdeftab, "MEND\n");
            break;
        }
        // Replace & arguments with ?1, ?2 etc.
        for (int i = 0; i < strlen(op); i++)
        {
            if (op[i] == '&')
                op[i] = '?';
        }
        fprintf(fdeftab, "%s %s\n", o, op);
    }
}

void expandMacro(FILE *fdeftab, FILE *fout, char label[], char operand[])
{
    FILE *fargtab = fopen("files/argtab.txt", "w+");
    char operands[100];
    strcpy(operands, operand);

    char *token = strtok(operands, ",");
    int argno = 1;
    while (token != NULL)
    {
        fprintf(fargtab, "?%d %s\n", argno++, token);
        token = strtok(NULL, ",");
    }
    rewind(fargtab);

    char line[MAX], l[MID], o[MID];
    int inside = 0;

    rewind(fdeftab);
    while (fgets(line, sizeof(line), fdeftab))
    {
        decode(line, l, o, operand);
        if (strcmp(l, label) == 0)
        {
            inside = 1;
            continue;
        }
        if (inside)
        {
            if (strcmp(l, "MEND") == 0)
                break;

            char temp[MID];
            strcpy(temp, o);

            // replace ?1, ?2 with actual args
            rewind(fargtab);
            char a1[MID], a2[MID];
            while (fscanf(fargtab, "%s %s", a1, a2) != EOF)
            {
                if (strcmp(temp, a1) == 0)
                    strcpy(temp, a2);
            }

            fprintf(fout, "    %s %s\n", l, temp);
        }
    }
    fclose(fargtab);
}

void processLine(FILE *fin, FILE *fnamtab, FILE *fdeftab, FILE *fout,
                 char label[], char opcode[], char operand[])
{
    if (strcmp(opcode, "MACRO") == 0)
    {
        defineMacro(fin, fnamtab, fdeftab, label, operand);
    }
    else
    {
        FILE *check = fopen("files/namtab.txt", "r");
        char nlabel[MID];
        int found = 0;
        while (fscanf(check, "%s", nlabel) != EOF)
        {
            if (strcmp(opcode, nlabel) == 0)
            {
                found = 1;
                break;
            }
        }
        fclose(check);

        if (found)
        {
            fprintf(fout, ".   %s %s\n", opcode, operand);
            expandMacro(fdeftab, fout, opcode, operand);
        }
        else
        {
            fprintf(fout, "%s %s %s\n", label, opcode, operand);
        }
    }
}

int main()
{
    FILE *fin = fopen("files/input.txt", "r");
    FILE *fdeftab = fopen("files/deftab.txt", "w+");
    FILE *fnamtab = fopen("files/namtab.txt", "w+");
    FILE *fout = fopen("files/output.txt", "w");

    if (!fin || !fdeftab || !fnamtab || !fout)
    {
        printf("Error opening files.\n");
        return 1;
    }

    char line[MAX], label[MID], opcode[MID], operand[MID];

    while (fgets(line, sizeof(line), fin))
    {
        decode(line, label, opcode, operand);
        processLine(fin, fnamtab, fdeftab, fout, label, opcode, operand);
        if (strcmp(opcode, "END") == 0)
            break;
    }

    fclose(fin);
    fclose(fdeftab);
    fclose(fnamtab);
    fclose(fout);

    printf("✅ Macro Processing Complete. Check output.txt, deftab.txt, namtab.txt.\n");
    return 0;
}
