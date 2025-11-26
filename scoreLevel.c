#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char getScoreLevel_switch(int score)
{
    switch (score / 10)
    {
    case 10:
    case 9:
        return 'A';
    case 8:
        return 'B';
    case 7:
        return 'C';
    case 6:
        return 'D';
    default:
        return 'E';
    }
}

char getScoreLevel_ASCII(int score)
{
    if (score == 100)
    {
        return 'A';
    }
    if (score < 60)
    {
        return 'E';
    }

    int offset = 10 - (score / 10) - 1; // 100时为@,90区间为A
    return 'A' + offset;
}

long getFileSize(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    return size;
}

int getFileRows(FILE *fp)
{
    int totalRows = 0;
    char buf[1024];
    while (fgets(buf, 1024, fp) != NULL)
    {
        totalRows++;
    }
    rewind(fp);
    return totalRows;
}

int getFileSocreList(FILE *fp, int *scoreList)
{
    long fileSize = getFileSize(fp);
    char buf[32];
    int scoreCount = 0;
    while (fgets(buf, fileSize, fp) != NULL)
    {
        // user1,90
        strtok(buf, ",");
        scoreList[scoreCount] = (int)strtol(strtok(NULL, ","), NULL, 10);
        scoreCount++;
    }
    rewind(fp);
    return scoreCount;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Press Ctrl + C to escape.\n");
        while (true)
        {
            int input = 0;
            printf("Input a score(0-100):");
            scanf("%d", &input);
            if (input > 100 || input < 0)
            {
                printf("WTF??? Are you blind???\n"
                       "The fucking score is in the range of 0 to 100\n");
                continue;
            }
            char scoreLevel = getScoreLevel_ASCII(input);
            printf("Level: %c, Score: %d\n", scoreLevel, input);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            char *endptr = NULL;
            long l = strtol(argv[i], &endptr, 10);
            char scoreLevel = '\0';
            if (*endptr == '\0')
            {
                // 纯数字
                printf("pure num");
                scoreLevel = getScoreLevel_ASCII(l);
                if (l > 100 || l < 0)
                {
                    scoreLevel = '!';
                }
                printf("Level: %c, Score: %ld\n", scoreLevel, l);
            }
            else
            {
                // 不是纯数字，按文件名处理
                FILE *fp = fopen(argv[i], "r");
                if (fp == NULL)
                {
                    printf("Open file failed.");
                    return -1;
                }
                int *scoreList = malloc(sizeof(int) * getFileRows(fp));
                int scoreCount = getFileSocreList(fp, scoreList);
                for (int i = 0; i < scoreCount; i++)
                {
                    scoreLevel = getScoreLevel_ASCII(scoreList[i]);
                    printf("Level: %c, Score: %d\n", scoreLevel, scoreList[i]);
                }

                free(scoreList);
            }
        }
    }
}