/*  Header Comment
    Daniela Cruz Falquez & Jack Bailey
    COP 3223H - 0204
    11/19/18
    Labyrinth
    This program creates a labyrinth program for the user to play.
 */

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

//constants
#define MAXLEN 24

///structure definitions
typedef struct{
    int optimalPath;
    int startX;
    int startY;
    int mstartX;
    int mstartY;
    int endX;
    int endY;
    int width;
    int height;
    int map[MAXLEN][MAXLEN];
} maze;

typedef struct{
    int xpos;
    int ypos;
} entity;

typedef struct{
    char name[MAXLEN];
    int score;
} score;

//function prototypes
void readMaze(maze *lab, FILE *ifp, int index);
void printMaze(maze lab, entity player, entity minotaur);
int menu(score *leaderboard);
void minotaurMove(maze lab, entity *minotaur);
int canMove(maze lab, entity ent, char wasd);
int playerStatus(maze lab, entity *ent, char wasd);
void move(entity *ent, char wasd);
//void endGame(score *result, maze lab, FILE *output);
//int scoreCalculator(score *result, maze lab);
//void scoreSorter(FILE *output);

//main function
int main(void) {
    int results, check = 0, test = 0;
    int *flag = 1;
    char wasd;
    maze labs[3]; //3 mazes: easy/medium/hard
    score leaderboard;
    entity player, minotaur;

    FILE *if1 = fopen("labyrinthE1.txt", "r");
    FILE *if2 = fopen("labyrinthM1.txt", "r");
    FILE *if3 = fopen("labyrinthH1.txt", "r");
    FILE *ofp = fopen("leaderboard.txt","w");

    readMaze(&labs, if1, 0); //takes in lab, file, and index since we can't call lab[0]
    readMaze(&labs, if2, 1);
    readMaze(&labs, if3, 2);

    fclose(if1);
    fclose(if2);
    fclose(if3);

    while(1){
        //prints the menu and returns an int representing the user's choice
        results = menu(&leaderboard);
        leaderboard.score = 0;

       //depending on the maze type, the corresponding player information will be set.
        if(results != 4){
            player.xpos = labs[results-1].startX;
            player.ypos = labs[results-1].startY;
            minotaur.xpos = labs[results-1].mstartX;
            minotaur.ypos = labs[results-1].mstartY;

            while(flag){
                printMaze(labs[results-1], player, minotaur);

                while(1){
                    printf("\nWhere do you want to move? (WASD)\n");
                    scanf(" %c", &wasd);

                    check = canMove(labs[results-1], player, wasd);

                    //move(labs[results-1], &minotaur, wasd);

                    if(check == 1){
                        move(&player, wasd);
                        leaderboard.score++;
                        break;
                    }
                    if(check == 2){
                        playerStatus(labs[results-1], &player, wasd);
                        leaderboard.score++;
                        break;
                    }
                    if(check == 3){
                        test = 1;
                        break;
                    }
                    else
                        printf("This was an invalid move, please enter again. (WASD) \n");
                }

                minotaurMove(labs[results-1], &minotaur);

                if(test){
                    test = 0;
                    system("cls");
                    printf("Congratulations %s!", leaderboard.name);
                    printf("\nYou reached the end of the maze and saved the princess!\n");
                    printf("\n\nYour score is %d!\n",leaderboard.score); //call score()

                    break;
                }
            }
        }

        if(results == 4){
            //score stuff
        }
    }

    fclose(ofp);

    return 0;
}


//function definitions

//precondition: there is a valid file to read from and that the maze structure was created properly
//postcondition: none
void readMaze(maze *lab, FILE *ifp, int index){
    int i, j, num;

    fscanf(ifp, "%d %d %d %d %d %d %d %d %d", &lab[index].width, &lab[index].height, &lab[index].optimalPath, &lab[index].startX, &lab[index].startY, &lab[index].endX, &lab[index].endY, &lab[index].mstartX, &lab[index].mstartY);

    for(i = 0; i < lab[index].width; i++){
        for(j = 0; j < lab[index].height; j++){
            fscanf(ifp, "%d", &num);
            lab[index].map[i][j] = num;
        }
    }

    return;
}

//precondition: that the maze structure was created properly
//postcondition: none
void printMaze(maze lab, entity player, entity minotaur){
    int i, j;

    system("cls");

    if(lab.width == 12)
        printf("Watch out for the Minotaur!!!\n");

    if(lab.width == 17 || lab.width==22)
        printf("Be careful that you don't fall into a trap and watch out for the Minotaur!!!\n");

    printf("\n");

    //We are experimenting with color on the console when we print the maze
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // Save current attributes
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    lab.map[player.xpos][player.ypos] = 5;
    lab.map[minotaur.xpos][minotaur.ypos] = 7;

    for(i = 0; i < lab.width; i++){
        if(i != lab.startX && j != lab.startY)
            printf("\t ");
        else{
            printf("Start -->");
        }

        for(j = 0; j < lab.height; j++){
            if(lab.map[i][j]==1){
                //SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
                printf("   ");
                //SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else if(lab.map[i][j]==0)
                printf("|||");
            else if(lab.map[i][j]==2){
                SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
                printf(" TB");
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else if(lab.map[i][j]==3){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                printf(" TE");
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else if(lab.map[i][j]==5){
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf(" H ");
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }

            if(lab.map[i][j]==7){
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf(" M ");
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }

            if(i == lab.endX && j == lab.endY-1)
                printf("<--- END");
        }

        printf("\n");
    }

    return;
}

//precondition: none
//postcondition: returns number of user selection
int menu(score *leaderboard){
    int results = 0;
    char name[MAXLEN];

    while(results != 1 && results != 2 && results != 3){
        printf("Please make a selection from the following:\n");
        printf("\t1 - Save the Princess\n");
        printf("\t2 - View Scores\n");
        printf("\t3 - Quit\n");

        scanf("%d", &results);
    }

    if(results == 2){
        return 4;
    }

    else if(results == 3){
        exit(0);
    }

    else if(results == 1){
        printf("What is your name Hero?\n");
        scanf("%s", leaderboard->name);

        results = 0;

        while(results != 1 && results != 2 && results != 3){
            printf("%s make a selection from the following:\n", leaderboard->name);
            printf("\t1 - Easy Labyrinth\n");
            printf("\t2 - Medium Labyrinth\n");
            printf("\t3 - Hard Labyrinth\n");

            scanf("%d", &results);
        }
    }

    return results;
}

//precondition: a character was entered
//postcondition: returns 1 if move selection was valid, 0 if not
int canMove(maze lab, entity ent, char wasd){
    char movDir = toupper(wasd);

    if(movDir == 'A'){
        if(lab.map[ent.xpos][ent.ypos-1]==1 && ent.ypos-1 != lab.startY-1 || lab.map[ent.xpos][ent.ypos-1]==3) //checks upper to see if wall
            return 1;
        if(lab.map[ent.xpos][ent.ypos-1]==2)
            return 2;
    }
    else if(movDir == 'W'){
        if(lab.map[ent.xpos-1][ent.ypos]==1 || lab.map[ent.xpos-1][ent.ypos]==3)
            return 1;
        if(lab.map[ent.xpos-1][ent.ypos]==2)
            return 2;

    }
    else if(movDir == 'D'){
        if(ent.ypos+1 == lab.endY && ent.xpos == lab.endX){
            return 3;
        }
        if(lab.map[ent.xpos][ent.ypos+1]==1 || lab.map[ent.xpos][ent.ypos+1]==3)
            return 1;
        if(lab.map[ent.xpos][ent.ypos+1]==2)
            return 2;
    }
    else if(movDir == 'S'){
        if(lab.map[ent.xpos+1][ent.ypos]==1 || lab.map[ent.xpos+1][ent.ypos]==3)
            return 1;
        if(lab.map[ent.xpos+1][ent.ypos]==2)
            return 2;
    }

    return 0;
}

//This function handles the traps and also checks to see if minotaur and player are adjacent
//precondition: none
//postcondition:
int playerStatus(maze lab, entity *ent, char wasd){
   char movDir = toupper(wasd);
   int i, j, x, y;

   for(i=0; i<lab.width; i++){
        for(j=0; j<lab.width; j++){
            if(lab.map[i][j] == 3){
                x = i;
                y = j;
            }
        }
   }

    if(movDir == 'A' && lab.map[ent->xpos][ent->ypos-1]==2){
        ent->xpos = x;
        ent->ypos = y;
        return 0;
    }
    else if(movDir == 'W' && lab.map[ent->xpos-1][ent->ypos]==2){
        ent->xpos = x;
        ent->ypos = y;
        return 0;
    }
    else if(movDir == 'D' && lab.map[ent->xpos][ent->ypos+1]==2){
        ent->xpos = x;
        ent->ypos = y;
        return 0;
    }
    else if(movDir == 'S' && lab.map[ent->xpos+1][ent->ypos]==2){
        ent->xpos = x;
        ent->ypos = y;
        return 0;
    }

    //if minotaur is adjacent
    return 1;
}

//precondition:
//postcondition:
void move(entity *ent, char wasd){
    char movDir = toupper(wasd);

    if(movDir == 'A')
        ent->ypos = ent->ypos-1;

    else if(movDir == 'W')
        ent->xpos = ent->xpos-1;

    else if(movDir == 'D')
        ent->ypos = ent->ypos+1;

    else if(movDir == 'S')
        ent->xpos = ent->xpos+1;

    return;
}

//precondition:
//postcondition:
void minotaurMove(maze lab, entity *minotaur){
    srand(time(NULL));

    char movDir;
    int result;
    int randomNum;

    while(1){
        randomNum = rand()%4 + 1 ;

        if(randomNum == 1)
            movDir = 'W';

        else if(randomNum == 2)
           movDir = 'A';

        else if(randomNum == 3)
            movDir = 'S';

        else if(randomNum == 4)
            movDir = 'D';

        result = canMove(lab, *minotaur, movDir);

        if(result == 1 || result == 2){
            move(minotaur, movDir);
            break;
        }
    }
}

/*
//precondition:
//postcondition:
int scoreCalculator(score *result, maze lab){

}

//precondition:
//postcondition:
void scoreSorter(FILE *output){

}
*/
