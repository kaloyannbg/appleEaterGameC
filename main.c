#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#define LEVEL_0_SPEED 100
#define LEVEL_1_SPEED 80
#define LEVEL_2_SPEED 60
#define LEVEL_3_SPEED 40

void hidecursor();

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void clearScreen(int currSpeed)
{
    Sleep(currSpeed);
}

#endif

#ifdef linux
#include <unistd.h>
#include <ncurses.h>
#define LEVEL_0_SPEED 1
#define LEVEL_1_SPEED 0.800
#define LEVEL_2_SPEED 0.600
#define LEVEL_3_SPEED 0.400

void clearScreen(int currSpeed)
{
    sleep(currSpeed);
}

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

#endif

#define LEVEL_1_SCORES 5
#define LEVEL_2_SCORES 25
#define LEVEL_3_SCORES 50

#define LEFT_LOWER 'a'
#define LEFT_UPPER 'A'

#define RIGHT_LOWER 'd'
#define RIGHT_UPPER 'D'

#define UP_LOWER 'w'
#define UP_UPPER 'W'

#define DOWN_LOWER 's'
#define DOWN_UPPER 'S'

typedef char **twoDArray;

twoDArray createBoard(int boardRows, int boardCols, int *currX, int *currY);

void resetBoard(twoDArray myArr, int boardRows, int boardCols, int currX, int currY);

void printBoard(twoDArray myArr, int boardRows, int boardCols, int currX, int currY);

void changeBoard(twoDArray myArr, int *currX, int *currY, int rowApple, int colApple);

void placeApple(twoDArray myArr, int boardRows, int boardCols, int *rowX, int *colY);

void printScores(int currScore);

void printCover();

void printLevelAndScore(int *getAdressOfLevelVar, int *isWinAdress, int currScore);

void setCirclePos(char c, int *adressCurrX, int *adressCurrY, int boardRows, int boardCols, int *hitWallAdress);

void changeSpeed(int level, int *currSpeed);

int main(int argc, char const *argv[])
{

    srand(time(0));

    int boardRows = 10, boardCols = 30, currX = 0, currY = 0, hitWall = 0;

    char **currBoard = createBoard(boardRows, boardCols, &currX, &currY);

    char c = RIGHT_UPPER;
    int rowApple = 0, colApple = 0, scores = 0, level = 0, isWin = 0, currSpeed = 100;

    placeApple(currBoard, boardRows, boardCols, &rowApple, &colApple);

    while (hitWall == 0 && isWin != 1)
    {
        system("clear");
        resetBoard(currBoard, boardRows, boardCols, currX, currY);
        changeBoard(currBoard, &currX, &currY, rowApple, colApple);
        if (currX == rowApple && currY == colApple)
        {
            ++scores;
            placeApple(currBoard, boardRows, boardCols, &rowApple, &colApple);
        }
        printCover();

        printLevelAndScore(&level, &isWin, scores);

        printf("\n\n");
        printBoard(currBoard, boardRows, boardCols, currX, currY);

        setCirclePos(c, &currX, &currY, boardRows, boardCols, &hitWall);

        if (kbhit())
        {
            c = getch();
            continue;
        }


        changeSpeed(level, &currSpeed);
        clearScreen(1);
    }

    system("clear");

    printCover();

    printLevelAndScore(&level, &isWin, scores);
    printf("\n\n");
    printBoard(currBoard, boardRows, boardCols, currX, currY);

    if (hitWall == 1)
    {
        printf("\n\n -- You lose, because hit wall!");
    }
    else if (isWin == 1)
    {
        printf("\n\n -- You win, with reaching max level 3!");
    }

   // endwin();

    return 0;
}

void changeSpeed(int level, int *currSpeed)
{
    switch (level)
    {
    case 0:
        (*currSpeed) = LEVEL_0_SPEED;
        break;
    case 1:
        (*currSpeed) = LEVEL_1_SPEED;
        break;
    case 2:
        (*currSpeed) = LEVEL_2_SPEED;
        break;
    case 3:
        (*currSpeed) = LEVEL_3_SPEED;
        break;
    default:
        break;
    }
}

void setCirclePos(char c, int *adressCurrX, int *adressCurrY, int boardRows, int boardCols, int *hitWallAdress)
{
    switch (c)
    {
    case RIGHT_LOWER:
    case RIGHT_UPPER:
        ++(*adressCurrY);
        if ((*adressCurrY) == boardCols)
        {
            *hitWallAdress = 1;
            break;
            // currY = 0;
        }
        break;
    case LEFT_LOWER:
    case LEFT_UPPER:
        --(*adressCurrY);
        if ((*adressCurrY) == 0)
        {
            // currY = boardCols;
            *hitWallAdress = 1;
            break;
        }
        break;
    case DOWN_LOWER:
    case DOWN_UPPER:
        ++(*adressCurrX);
        if ((*adressCurrX) == boardRows)
        {
            *hitWallAdress = 1;
            break;
            // currX = 0;
        }
        break;
    case UP_LOWER:
    case UP_UPPER:
        --(*adressCurrX);
        if ((*adressCurrX) == -1)
        {
            *hitWallAdress = 1;
            break;
            // currX = boardRows - 1;
        }
        break;
    default:
        break;
    }
}

void printLevelAndScore(int *getAdressOfLevelVar, int *isWinAdress, int currScore)
{
    if (currScore < LEVEL_1_SCORES)
    {
        *getAdressOfLevelVar = 0;
    }
    else if (currScore >= LEVEL_1_SCORES && currScore < LEVEL_2_SCORES)
    {
        *getAdressOfLevelVar = 1;
    }
    else if (currScore >= LEVEL_2_SCORES && currScore < LEVEL_3_SCORES)
    {
        *getAdressOfLevelVar = 2;
    }
    else if (currScore >= LEVEL_3_SCORES)
    {
        *getAdressOfLevelVar = 3;
        *isWinAdress = 1;
        return;
    }

    printf(" -- Level %d -- \n", *getAdressOfLevelVar);
    printf(" -- %d / ", currScore);

    switch (*getAdressOfLevelVar)
    {
    case 0:
        printf("%d -- ", LEVEL_1_SCORES);
        break;
    case 1:
        printf("%d -- ", LEVEL_2_SCORES);
        break;
    case 2:
        printf("%d -- ", LEVEL_3_SCORES);
        break;
    default:
        break;
    }
}

void printBoard(twoDArray myArr, int boardRows, int boardCols, int currX, int currY)
{
    if (myArr == NULL)
    {
        printf(" -- ARRAY IS NULL -- ");
        exit(1);
    }
    if (boardCols % 2 == 0)
    {
        printf("  ");
    }
    else
    {
        printf(" ");
    }
    for (int i = 0; i < boardCols / 2; i++)
    {
        printf("%c ", 176);
    }
    printf("\n");
    for (int i = 0; i < boardRows; i++)
    {
        for (int j = 0; j < boardCols; j++)
        {
            if (j == 0)
            {
                printf("%c", 176);
            }
            if (myArr[i][j] == 'o')
            {
                printf("\033[0;32m"); // Set the text to the color red.
                printf("%c", myArr[i][j]);
                printf("\033[0m"); // Resets the text to default color.
            }
            else if (myArr[i][j] == 'O')
            {
                printf("\033[0;31m"); // Set the text to the color red.
                printf("%c", myArr[i][j]);
                printf("\033[0m"); // Resets the text to default color.
            }
            else
            {
                printf("%c", myArr[i][j]);
            }

            if (j == boardCols - 1)
            {
                printf("%c", 176);
            }
        }
        printf("\n");
    }
    if (boardCols % 2 == 0)
    {
        printf("  ");
    }
    else
    {
        printf(" ");
    }
    for (int i = 0; i < boardCols / 2; i++)
    {
        printf("%c ", 176);
    }
    printf("\n");
}

twoDArray createBoard(int boardRows, int boardCols, int *currX, int *currY)
{
    twoDArray myArr = (twoDArray)malloc(boardRows * sizeof(char *)); // X - UP - DOWN
    if (myArr == NULL)
    {
        printf(" -- OUT OF MEMORY -- ");
        exit(1);
    }

    for (int i = 0; i < boardRows; i++)
    {
        myArr[i] = (char *)malloc(boardCols * sizeof(char)); // Y - LEFT - RIGHT

        for (int j = 0; j < boardCols; j++)
        {
            myArr[i][j] = ' ';
        }
    }
    return myArr;
}

void changeBoard(twoDArray myArr, int *currX, int *currY, int rowApple, int colApple)
{
    myArr[*currX][*currY] = 'o';
    myArr[rowApple][colApple] = 'O';
}

void resetBoard(twoDArray myArr, int boardRows, int boardCols, int currX, int currY)
{
    for (int i = 0; i < boardRows; i++)
    {
        for (int j = 0; j < boardCols; j++)
        {
            myArr[i][j] = ' ';
        }
    }
}

void placeApple(twoDArray myArr, int boardRows, int boardCols, int *rowX, int *colY)
{
    *rowX = rand() % (boardRows - 2);
    *colY = rand() % (boardCols - 2);
    if (*rowX == 0 || *rowX == 1)
    {
        *rowX = 2;
    }
    if (*colY == 0 || *colY == 1)
    {
        *colY = 2;
    }
}

void printCover()
{
    printf(" -- Noob DOS Apple Eater Game created, bcs my sleep issues is active XD -- \n\n");
}

void printScores(int currScore)
{
    printf(" -- Your scores: %d ", currScore);
}