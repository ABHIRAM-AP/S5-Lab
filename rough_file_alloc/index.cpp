#include <stdio.h>
#define MAX 100

struct file
{
    int file_no;
    int index_block;
    int blocks[20];
    int block_count;
    int allocated;
};
typedef struct file FILES;

int memory[MAX];

void initializeMemory()
{
    for (int i = 0; i < MAX; i++)
        memory[i] = -1;
}

int isFree(int block)
{
    return memory[block] == -1;
}

void allocateFiles(FILES files[], int total)
{
    for (int i = 0; i < total; i++)
    {
        printf("\nEnter File Number: ");
        scanf("%d", &files[i].file_no);

        printf("Enter Index Block: ");
        scanf("%d", &files[i].index_block);

        if (!isFree(files[i].index_block))
        {
            printf("Index Block %d already allocated. File %d can't be allocated.\n", files[i].index_block, files[i].file_no);
            files[i].allocated = 0;
            continue;
        }

        printf("Enter number of blocks: ");
        scanf("%d", &files[i].block_count);

        printf("Enter %d block numbers: ", files[i].block_count);
        int valid = 1;
        for (int j = 0; j < files[i].block_count; j++)
        {
            scanf("%d", &files[i].blocks[j]);
            if (!isFree(files[i].blocks[j]))
                valid = 0;
        }

        if (!valid)
        {
            printf("One or more data blocks already allocated. File %d can't be stored.\n", files[i].file_no);
            files[i].allocated = 0;
        }
        else
        {
            memory[files[i].index_block] = files[i].file_no;
            for (int j = 0; j < files[i].block_count; j++)
                memory[files[i].blocks[j]] = files[i].file_no;
            files[i].allocated = 1;
            printf("File %d allocated successfully.\n", files[i].file_no);
        }
    }
}

void displayFiles(FILES files[], int total)
{
    printf("\nFile\tIndex\tBlocks\n");
    for (int i = 0; i < total; i++)
    {
        if (files[i].allocated)
        {
            printf("%d\t%d\t", files[i].file_no, files[i].index_block);
            for (int j = 0; j < files[i].block_count; j++)
                printf("%d ", files[i].blocks[j]);
            printf("\n");
        }
    }
}

int main()
{
    int total;
    FILES files[20];

    initializeMemory();

    printf("Enter number of files: ");
    scanf("%d", &total);

    allocateFiles(files, total);
    displayFiles(files, total);

    return 0;
}
