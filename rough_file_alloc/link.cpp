#include <stdio.h>
#include <stdlib.h>

struct block
{
    int block_no;
    struct block *next;
};

typedef struct block BLOCK;

struct file
{
    int file_no;
    BLOCK *start;
};
typedef struct file FILES;

BLOCK *createBlock(int block_no)
{
    BLOCK *newBlock = (BLOCK *)malloc(sizeof(BLOCK));
    newBlock->block_no = block_no;
    newBlock->next = NULL;
    return newBlock;
}

void readDetails(FILES f[], int total)
{
    for (int i = 0; i < total; i++)
    {
        printf("Enter file number:\n");
        scanf("%d", &f[i].file_no);
        f[i].start = NULL;

        int blockCount;
        printf("Enter number of blocks for File %d: ", f[i].file_no);
        scanf("%d", &blockCount);

        printf("Enter %d block numbers:\n", blockCount);
        BLOCK *head = NULL, *tail = NULL;
        for (int j = 0; j < blockCount; j++)
        {
            int block_no;
            scanf("%d", &block_no);

            BLOCK *newNode = createBlock(block_no);

            if (head == NULL)
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }
        f[i].start = head;
    }
}

void searchFile(FILES f[], int total, int fileNum)
{
    int found = 0;
    for (int i = 0; i < total; i++)
    {
        if (f[i].file_no == fileNum)
        {
            found = 1;
            printf("\nFile %d Found!\nBlocks Occupied:\n", f[i].file_no);
            BLOCK *current = f[i].start;
            while (current != NULL)
            {
                printf("%3d", current->block_no);
                if (current->next != NULL)
                {
                    printf(" -> ");
                }
                else
                    printf("-> NULL");
                current = current->next;
            }
        }
    }
    if (!found)
    {
        printf("\nFile %d not found.\n", fileNum);
    }
}
void freeMemory(FILES files[], int total)
{
    for (int i = 0; i < total; i++)
    {
        BLOCK *curr = files[i].start;
        while (curr != NULL)
        {
            BLOCK *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}
int main()
{
    int total = 0, fileNum = 0;
    FILES files[100];
    printf("Enter number of files: ");
    scanf("%d", &total);
    readDetails(files, total);
    printf("Enter file to be searched:\n");
    scanf("%d", &fileNum);
    searchFile(files, total, fileNum);
    freeMemory(files, total);
}