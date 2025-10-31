#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define MID 50

int searchNamtab(FILE *fnamtab, char opcode[])
{
    char name[MID];
    rewind(fnamtab);
    while (fgets(name, sizeof(name), fnamtab))
    {
        name[strcspn(name, "\n")] = '\0'; // remove newline
        if (strcmp(name, opcode) == 0)
            return 1; // found macro name
    }
    return 0;
}

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
void expandMacro(FILE *fnamtab, FILE *fdeftab, FILE *fout, char opcode[], char operand[])
{
    FILE *fargtab = fopen("files/argtab.txt", "w+");
    rewind(fdeftab);
    char line[MAX];

    // Print macro call header (.ABC P,Q)
    fprintf(fout, "    .%s    %s\n", opcode, operand);

    // Step 1: split operands (P,Q) into params[]
    char params[10][MID];
    int paramCount = 0;
    char *token = strtok(operand, ",");
    while (token != NULL)
    {
        strcpy(params[paramCount++], token);
        fprintf(fargtab, "%s\n", token);
        token = strtok(NULL, ",");
    }

    // Step 2: find macro body
    int inMacro = 0;
    while (fgets(line, sizeof(line), fdeftab))
    {
        char name[MID];
        sscanf(line, "%s", name);

        if (!inMacro)
        {
            if (strcmp(name, opcode) == 0)
                inMacro = 1;
            continue;
        }

        if (strstr(line, "MEND"))
            break;

        // Step 3: replace ?1, ?2 ... with actual params
        char expanded[MAX];
        strcpy(expanded, line);

        for (int i = 0; i < paramCount; i++)
        {
            char search[5], *pos;
            sprintf(search, "?%d", i + 1);
            while ((pos = strstr(expanded, search)) != NULL)
            {
                char temp[MAX];
                *pos = '\0';
                sprintf(temp, "%s%s%s", expanded, params[i], pos + strlen(search));
                strcpy(expanded, temp);
            }
        }

        // Write final expanded line
        fprintf(fout, "    %s", expanded);
    }
}

void processLine(FILE *fin, FILE *fnamtab, FILE *fdeftab, FILE *fout, char label[], char opcode[], char operand[])
{
    int found = searchNamtab(fnamtab, opcode);
    if (found)
    {
        expandMacro(fnamtab, fdeftab, fout, opcode, operand);
        return;
    }
    char line[MAX];
    if (strcmp(opcode, "MACRO") == 0)
    {
        fprintf(fnamtab, "%s\n", label);

        // ---- Step 1: store macro arguments ----
        char args[10][MID];
        int argCount = 0;
        char tmp[MID];
        strcpy(tmp, operand);
        char *token = strtok(tmp, ",");

        while (token != NULL)
        {
            strcpy(args[argCount++], token);
            token = strtok(NULL, ",");
        }

        // ---- Step 2: write macro name line in deftab ----
        fprintf(fdeftab, "%s ", label);
        for (int i = 0; i < argCount; i++)
        {
            fprintf(fdeftab, "%s", args[i]);
            if (i < argCount - 1)
                fprintf(fdeftab, ",");
        }
        fprintf(fdeftab, "\n");

        // ---- Step 3: process body until MEND ----
        while (fgets(line, sizeof(line), fin))
        {
            decode(line, label, opcode, operand);

            if (strcmp(opcode, "MEND") == 0)
            {
                fprintf(fdeftab, "MEND\n");
                break;
            }

            // ---- Step 4: replace &A,&B,... with ?1,?2,... ----
            for (int i = 0; i < argCount; i++)
            {
                char *pos = strstr(operand, args[i]);
                if (pos != NULL)
                {
                    char temp[MAX];
                    strcpy(temp, operand);
                    // strcpy(operand, temp); // reset just in case
                    sprintf(pos, "?%d", i + 1);
                }
            }

            fprintf(fdeftab, "%s %s\n", opcode, operand);
        }
    }
    else
    {
        fprintf(fout, "%s %s %s\n", label, opcode, operand);
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

    return 0;
}
