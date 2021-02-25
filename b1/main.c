#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * author: Nguyen Minh Dung 20184079
 */

typedef struct WordObject
{
    char content[100];
    int count;
    int line[100];
} WordObject;

WordObject *wordList;

int listSize;


void sortResultAlFaBet()
{
    int i, j;
    WordObject temp;
    
    for (i = 0; i < listSize; i++)
    {
        for (j = 0; j < (listSize-i); j++)
        {
            if (strcmp(wordList[j].content, wordList[j + 1].content) > 0) 
            {
                // swap 2 item
                temp = wordList[j];
                wordList[j] = wordList[j + 1];
                wordList[j + 1] = temp;
            } 
        }
    }
}

void printResultToFile()
{
    FILE* resFile = fopen("resultAlice.txt", "w+");
    int j;
    for (int i = 0; i < listSize; i++)
    {
        j = wordList[i].count;
        fprintf(resFile, "%s %d", wordList[i].content, wordList[i].count);
        for (int k = 0; k < j; k++)
        {
            fprintf(resFile, ", %d", wordList[i].line[k]);
        }
        fprintf(resFile, "\n");
    }
    fclose(resFile);
}


void printResult()
{
    int j;
    for (int i = 0; i < listSize; i++)
    {
        j = wordList[i].count;
        printf("%s %d", wordList[i].content, wordList[i].count);
        for (int k = 0; k < j; k++)
        {
            printf(", %d", wordList[i].line[k]);
        }
        printf("\n");
    }
}

void toLowerCase(char *word)
{
    for (int i = 0; i < strlen(word); i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
            word[i] += 32;
    }
}

// check duplicate
int checkDuplicate(int num, char *token)
{
    for (int i = 0; i < num; i++)
    {
        if (strcmp(wordList[i].content, token) == 0)
            return 1;
    }
    return 0;
}

// check stopw contain word or not
int check_stopw(WordObject *stopw, int n, char *word)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(stopw[i].content, word) == 0)
            return 1;
    }
    return 0;
}

// check
int checkWordIsInvalidString(char* word){
    for (int i = 0; i < strlen(word); i++)
    {
        if((word[i] < 65 || word[i] > 90) && (word[i] < 97 || word[i] > 122)) {
            return 0;
        }
    }
    return 1;
}

WordObject *save(WordObject *wordList, int num, char *word, int lineIndex)
{
    WordObject *tmp = malloc((num + 1) * sizeof(WordObject));
    if (num != 0)
    {
        for (int i = 0; i < num; i++)
        {
            tmp[i] = wordList[i];
        }
        free(wordList);
    }
    strcpy(tmp[num].content, word);
    tmp[num].count = 1;
    tmp[num].line[0] = lineIndex;
    return tmp;
}

int getWords(FILE *f, WordObject *stopw, int n, int num)
{
    int lineIndex = 1;
    char currentChar;
    char tmpWord[100];
    currentChar = fgetc(f);
    int i = 0;
    int j;
    while (currentChar != EOF)
    {
        if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z'))
        {
            tmpWord[i++] = currentChar;
        }
        else
        {
            tmpWord[i] = '\0';
            toLowerCase(tmpWord);
            if (i != 0)
            {
                if (!check_stopw(stopw, n, tmpWord))
                {
                    if (!checkDuplicate(num, tmpWord))
                    {
                        if(checkWordIsInvalidString(tmpWord)){
                            //printf("%s\n", tmpWord);
                            wordList = save(wordList, num, tmpWord, lineIndex);
                            num++;
                        }
                        
                    } else {
                        for (int i = 0; i < num; i++)
                        {
                            if (strcmp(wordList[i].content, tmpWord) == 0)
                            {
                                j = wordList[i].count;
                                wordList[i].line[j] = lineIndex;
                                wordList[i].count++;
                            }
                        }
                    }
                }
            }
            i = 0;
            if (currentChar == '\n')
                lineIndex++;
        }
        currentChar = fgetc(f);
    }
    return num;
}

int main()
{
    FILE *fText, *fStopWord;
    listSize = 0;
    fText = fopen("vanban.txt", "r");
    fStopWord = fopen("stopw.txt", "r");

    int count = 0;
    char tmp[80];

    // count the size of list stop word
    while (!feof(fStopWord))
    {
        fscanf(fStopWord, "%s", tmp);
        count++;
    }

    WordObject *stopw = malloc(count * sizeof(WordObject));
    rewind(fStopWord);
    count = 0;
    while (!feof(fStopWord))
    {
        fscanf(fStopWord, "%s", stopw[count].content);
        count++;
    }
    listSize = getWords(fText, stopw, count, listSize);

    sortResultAlFaBet();
    //printResultToFile();
    printResult();

    fclose(fText);
    fclose(fStopWord);
}