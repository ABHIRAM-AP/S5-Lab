#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void passOne()
{
    char label[20], opcode[20], operand[20];
    char code[20], mnemonic[5];
    char line[100];
    int locctr = 0, start = 0;

    FILE *fp1 = fopen("input.txt", "r");
    FILE *fp2 = fopen("optab.txt", "r");
    FILE *fp3 = fopen("symtab.txt", "w");
    FILE *fp4 = fopen("intermediate.txt", "w");
    FILE *fp5 = fopen("length.txt", "w");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5)
    {
        printf("Error opening one or more files.\n");
        exit(1);
    }

    // Read first line
    if (fgets(line, sizeof(line), fp1))
    {
        int fields = sscanf(line, "%s%s%s", label, opcode, operand);

        // If only opcode & operand are present, shift variables
        if (fields == 2)
        {
            strcpy(operand, opcode);
            strcpy(opcode, label);
            strcpy(label, "");
        }
        else if (fields == 1)
        {
            strcpy(opcode, label);
            strcpy(label, "");
            strcpy(operand, "");
        }

        if (strcmp(opcode, "START") == 0)
        {
            start = atoi(operand);
            locctr = start;
            fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        }
        else
        {
            locctr = 0;
            fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        }
    }

    // Process remaining lines
    while (fgets(line, sizeof(line), fp1))
    {
        int fields = sscanf(line, "%s%s%s", label, opcode, operand);

        // Handle missing label
        if (fields == 2)
        {
            strcpy(operand, opcode);
            strcpy(opcode, label);
            strcpy(label, "");
        }
        else if (fields == 1)
        {
            strcpy(opcode, label);
            strcpy(label, "");
            strcpy(operand, "");
        }

        if (strcmp(opcode, "END") == 0)
        {
            fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
            break;
        }

        // Write to intermediate file
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // If label exists, add to symbol table
        if (strlen(label) > 0)
        {
            fprintf(fp3, "%s\t%d\n", label, locctr);
        }

        // Reset optab pointer
        rewind(fp2);
        int found = 0;
        while (fscanf(fp2, "%s%s", code, mnemonic) == 2)
        {
            if (strcmp(opcode, code) == 0)
            {
                locctr += 3;
                found = 1;
                break;
            }
        }

        // If not found in OPTAB, check directives
        if (!found)
        {
            if (strcmp(opcode, "WORD") == 0)
                locctr += 3;
            else if (strcmp(opcode, "RESW") == 0)
                locctr += 3 * atoi(operand);
            else if (strcmp(opcode, "BYTE") == 0)
                locctr += 1;
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
            else
                printf("Warning: Unknown opcode %s\n", opcode);
        }
    }

    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    int length = locctr - start;
    fprintf(fp5, "%d", length);
    fclose(fp5);

    printf("\nThe length of the code: %d\n", length);
}

int main()
{
    passOne();
    return 0;
}
