// HEADER FILES USED
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <windows.h>                                                    //  header file for gotoxy
#include <stdio.h>                                                      //header file for standard input output
#include <dos.h>
#include <time.h>
using namespace std;

COORD coord={0,0};                               //center of axis is set to the top left cornor of the screen
void gotoxy(int x,int y)                                                // gotoxy function
{
 coord.X=x;
 coord.Y=y;
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }
int getRandom()                                                             // random function
{
    int random;
    srand(time(NULL));                                                      // giving time as input
    random = rand()%7;                                                      // for random number b/w 0-6
    return random;                                                          // return random number
}
// VARIABLES DECLARATION
int block[][4]={{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 }};
int stage[32][21]={};
int field[32][21]={};

int x = 8, y = 0;                                               // coordinate
bool gameover = false;                                          // check whether game is over or not
long long int GAMESPEED = 25000;                                // determines game speed
int score = 0,old_score=0;                                      // calculate score
int high_score = 0;                                      // to maintain high score

int block_list[7][4][4]=    // blocks
{
    {{ 0, 1, 0, 0 },
     { 0, 1, 0, 0 },
     { 0, 1, 0, 0 },
     { 0, 1, 0, 0 }},   // line

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 0, 0 },
     { 0, 1, 0, 0 }},   // inverted L

    {{ 0, 0, 1, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 0, 0 },
     { 0, 0, 0, 0 }},   // rotated Z

    {{ 0, 1, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 1, 0 },
     { 0, 0, 0, 0 }},   // rotated S

    {{ 0, 0, 0, 0 },
     { 0, 1, 0, 0 },
     { 1, 1, 1, 0 },
     { 0, 0, 0, 0 }},   // inverted T

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 0, 0 }},   // block

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 1, 0 },
     { 0, 0, 1, 0 }}    // mirrored of inverted L
};
// FUNCTIONS USED
int gameOver();                                                     // displays game over
void title();                                                       // displays title tetris
void gameLoop();                                                    // runs game
void display();                                                     // display game board
bool makeBlocks();                                                  // make blocks
void initGame();                                                    // initializes game
void moveBlock(int, int);                                           // move blocks
void collidable();                                                  // fix the block
bool isCollide(int, int);                                           // check collisions
void userInput();                                                   // takes input from user
bool rotateBlock();                                                 // rotates the block
void spawnBlock();                                                  // moves block down
void speed();                                                       // increases the speed
// MAIN FUNCTION
int main()                                                          // main function
{
    title();                                                        // calls title function
    gameLoop();                                                     // calls gameLoop function
    return 0;
}
void title()                                                        // title function
{
    system("cls");                                                  // clears screen
    cout << "================================================================================\n\n\n\n\n\n"
            "\t\t####### ####### ####### ######    ###    #####\n"    // C++ merges separate constant strings automatically
            "\t\t   #    #          #    #     #    #    #     #\n"
            "\t\t   #    #          #    #     #    #    #\n"
            "\t\t   #    #####      #    ######     #     #####\n"
            "\t\t   #    #          #    #   #      #          #\n"
            "\t\t   #    #          #    #    #     #    #     #\n"
      "########\t   #    #######    #    #     #   ###    #####\t\t########\n\n\n"
            "\n\n\n\n"
           "================================================================================\n";
           getch();                                                  // displays tetris till user press any key

}
int gameOver()                                                          // gameOver function
{
    system("cls");                                                      // clears screen
    cout << "\n\n\n\n"
            "\t #####     #    #     # ####### ####### #     # ####### ######\n"
            "\t#     #   # #   ##   ## #       #     # #     # #       #     #\n"
            "\t#        #   #  # # # # #       #     # #     # #       #     #\n"
            "\t#  #### #     # #  #  # #####   #     # #     # #####   ######\n"
            "\t#     # ####### #     # #       #     #  #   #  #       #   #\n"
            "\t#     # #     # #     # #       #     #   # #   #       #    #\n"
            "\t #####  #     # #     # ####### #######    #    ####### #     #\n"
            "\n\n\n\n";
            cout<<"\t\t\t\tYour score is : "<<score;                    // displays score
            cout<<"\n\n\n";
            char ch;
            cout<<"\t\t\tWant to start new game {y/n}: ";
            cin>>ch;
            if(ch=='y'){                                                // starts new game
                gameover = false;
                score = 0;
                GAMESPEED = 25000;
                old_score=0;
                gameLoop();
            }
}
void gameLoop()                                                         // gameLoop function
{
    int time = 0;
    initGame();                                                         // initializes game
    while(!gameover)                                // check whether game is over or not
    {
        if (kbhit())                                // function to determine if key is pressed or not
            userInput();                            // calls function to take user input
        if (time < GAMESPEED)                       // for by default down
            time++;
        else
        {
            spawnBlock();                                               // move the block down
            time = 0;
        }
    }
}
void initGame()                                                         // initGame function
{
    for(int i=0;i<=30;i++)                                              // loop for number of rows
    {
        for(int j=0;j<=19;j++)                                          // loop for number of columns
        {
            if((j==0)||(j==19)||(i==30))                                // check for border and base
                field[i][j] = stage[i][j] = 9;
            else                                                        // remaining part of board
                field[i][j] = stage[i][j] = 0;
        }
    }
    makeBlocks();                                                       // calls makesBlocks function
    display();                                                          // calls display function
}
void display()                                                          // display function
{
    system("cls");                                                      // clears the screen
    cout<<"\n\n\n\n\t\t Your Score : "<<score<<"\t\t Highest Score : "<<high_score;// displays score & high_score
    int a=10;
    for (int i = 0; i < 31; i++)                                        // loop for rows
    {
            gotoxy(32,a);                                               // for starting position of row
            a++;                                                        // updates row
        for (int j = 0; j < 20; j++)                                    // loop for column
        {
            switch (field[i][j])
            {
            case 0: cout<<" "<<flush;                                   // for empty space
                    break;
            case 9: cout<<"X"<<flush;                                   // for border and base
                    break;
            default: cout<<"#"<<flush;                                  // for filled part
                     break;
            }
        }
        cout<<endl;
    }
    cout << "\n\n\n\t\tA: left\tS: down\tD: right \t Rotation[Space]";      // for instructions
    if (gameover)                                                           // check for game over
        gameOver();                                                         // calls gameOver function
}
bool makeBlocks()                                                           // makeBlocks function
{
    x = 8;                                                                  // for middle of game
    y = 0;                                                                  // for random block
    int blockType = getRandom();
    for (int i = 0; i < 4; i++)                                             // loop for block
    {
        for (int j = 0; j < 4; j++)
        {
            block[i][j] = 0;
            block[i][j] = block_list[blockType][i][j];
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            field[i][j + x] = stage[i][j + x] + block[i][j];                 // copying block to field

            if (field[i][j + x] > 1)                                        // collision occurs
            {
                gameover = true;                                            // denotes game over
                return true;
            }
        }
    }
    return false;
}
void moveBlock(int x2, int y2)                                              // moveBlock function
{
    for (int i = 0; i < 4; i++)                                             // remove block
        for (int j = 0; j < 4; j++)
            field[y + i][x + j] -= block[i][j];
    x = x2;                                                                 // update coordinates
    y = y2;
    for (int i = 0; i < 4; i++)                                      // assign a block with the updated value
        for (int j = 0; j < 4; j++)
            field[y + i][x + j] += block[i][j];
    display();                                                              // calls display function
}
void collidable()                                                           // collidable function
{
    int c=1,k=29;
    for(int i=29;i>=0;i--)                                                  // for each row
    {
        c=1;
        for(int j=0;j<20;j++)                                               // for each column
        {
            if(field[i][j]==0)                                          // check for the empty space in a line
            {
                c=0;
                break;
            }
        }
        if(c==0)                                   // empty space found then copy as it is else don't do anything
        {
            for(int j=0;j<20;j++)
                stage[k][j] = field[i][j];          // copying after eliminating filled lines
            k--;
        }
    }
    for (int i = 0; i<31; i++)
        for (int j = 0; j<20; j++)
            field[i][j] = stage[i][j];                                  // copying final field then
    score += 10;                                                         // increase of 5 for fixing each block
    score += (k+1)*20;                                                  // increase of 10 for removal of each line
    if(k>=1)                                                            // if lines removed are more than one
        score += 20;                                                    // then increase the score by 20
    if(score> high_score)                                               // updates high score if possible
        high_score = score;
    speed();                                                            // calls speed function
 }
bool isCollide(int x2, int y2)                                          // check for collisions
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (block[i][j] && stage[y2 + i][x2 + j] != 0)
                return true;                                            // if collision then return true
    return false;                                                       // else remove false
}
void spawnBlock()                                                       // spawn block function
{
    if (!isCollide(x, y + 1))                                           // check for collision
        moveBlock(x, y + 1);                                        // if no collision then move the block down
    else                                                                // else fix it
     {
        collidable();                                                   // fix the block
        makeBlocks();                                                   // make new block
        display();                                                      // display it
    }
}
void speed()                                                            // speed function
{
    if(score - old_score >=200 )
    {
        old_score += 200;
        GAMESPEED -= 5000;                                              // increases speed
    }
}
void userInput()                                                        // user input function
{
    char key;
    key = getch();
    switch (key)
    {
    case 'd':   if (!isCollide(x + 1, y))                               // move right if(no collision
                    moveBlock(x + 1, y);                                // move it
                break;
    case 'a':   if (!isCollide(x - 1, y))                               // move left if no collision
                    moveBlock(x - 1, y);                                // move it
                break;
    case 's':   if (!isCollide(x, y + 1))                               // move down if no collision
                    moveBlock(x, y + 1);                                // move it
                break;
    case ' ':   rotateBlock();                                          // rotate the block
    }
}
bool rotateBlock()                                                      // rotate block function
{
    int tmp[4][4];
    for (int i = 0; i < 4; i++)                                         // Save temporarily block
        for (int j = 0; j < 4; j++)
            tmp[i][j] = block[i][j];
    for (int i = 0; i < 4; i++)                                         // rotate
        for (int j = 0; j < 4; j++)
            block[i][j] = tmp[3 - j][i];                            // rotates clockwise
    if (isCollide(x, y))
    {                                                               // And stop if it overlaps not to be rotated
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                block[i][j] = tmp[i][j];
        return true;                                                    // rotation occurs
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            field[y + i][x + j] -= tmp[i][j];                           // remove original block
            field[y + i][x + j] += block[i][j];                         // adds rotated block
        }
    display();                                                          // calls display function
    return false;
}
