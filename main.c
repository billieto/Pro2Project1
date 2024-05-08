// Name: Sakellariou-Kyrou Vasileios
// AM: 2022202300037
// E-mail: dit23037@go.uop.gr
//
// Name: Milonas Nikolaos
// AM: 2022202300
// E-mail: dit23128@go.uop.gr

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Where x and y the cordinates of the character/ obstacle (x,y) in the 2D array
typedef struct stormtrooper
{
    int x;
    int y;
    int direction; // Horizontal or vertical movement
    int bounds; // 0 for right or down, 1 for left or up
    int alive; // 0 for dead, 1 for alive
}stroop;

typedef struct obstacle
{
    int x;
    int y;
}obs;

typedef struct princess_leia
{
    int x;
    int y;
    int injured; // 0 for not injured (Not killed a stormtrooper), 1 for injured (Killed a stormtrooper)
    int moves; // Number of moves made by the princess
}princess;

typedef struct darth_vader
{
    int x;
    int y;
}darth;

typedef struct r2d2
{
    int x;
    int y;
    int found; // 0 for not found, 1 for found
}r2d2;

void check_malloc(void *p); // if malloc returns null something went wrong, exiting the program
int move_leia(char ***ship, princess *leia, r2d2 *r2, char *moveset, int n, int m, int offset_moveset); // function to move leia, return 1 if leia cant do the move, returns 0 if she can
int move_vader(char ***ship, darth *vader, int leia_x, int leia_y); // function to move vader, return 1 if vader got leia, returns 0 if vader didnt got leia
void move_stormtroopers(char ***ship, stroop **army, int n, int m, int i, int storm, int *injured, int leia_x, int leia_y);
void generate_stormtroopers(char ***ship, stroop **army, int n, int m, int storm); // every level generates the new stormtroopers
void generate_vader(char ***ship, darth *vader, int n, int m); // every level generates the new vader
void generate_leia(char ***ship, princess *leia, int n, int m); // every level generates new leia
void generate_r2d2(char ***ship, r2d2 *r2, int n, int m); // every level generates new r2d2
void generate_obsticles(char ***ship, obs **objects, int n, int m, int obstacles); // every level generates new obsticles
void generate_level_dependant(int level, int n, int m, int *storm, int *obstacles); // every level generates the amount of stormtroopers and obsticles based on the difficulty the player chose 
int using_force(char ***ship, char *token, char *token2, obs **objects, int obstacles, int n, int m); // function to impliment the force mechanic, returns 1 if something went wrong, returns 0 if everything did go well
void print_board(char **ship, int n, int m); // function to pint the board
void fill_board(char ***ship, int n, int m, r2d2 r2, stroop *army, int storm); // function so the move function work fine, basicly it fills the board with stormtroopers and r2d2
void inisialize_board(char ***ship, int n, int m); // malloc for the 2d board
void free_all(char ***ship, stroop **army, obs **objects, int n, int obstacles); // all free into 1 function, just to look good in main
void cover_board(char ***ship, int n, int m, princess leia, darth vader, r2d2 r2, stroop *army, int storm, obs *objects, int obstacles, int help); // function to cover the board so the player can play normally
char read_input(char **token, char **token2, char **moveset, int *size, int *force_limit); // function to read the user input and determen what he wants to do 
int random_number(int n); // generates a random number between 0 and n - 1, took the idea from mr. Trifonopoulos slides
int read_text(char str[], int size, int flag); // from mr. Tselika's book, it return the size and checks if the input got through just fine
void move_enemies(char ***ship, stroop **army, int storm, darth *vader, int *captured, princess *leia, int n, int m);


int main(void)
{
    int i, n, m, diff = 2, storm = 2, obstacles, level = 1, len = 0, flag_l = 0, captured = 0, help = 0, force_limit = 0, flag_f = 0;
    int starting_m, staring_n;
    // "storm" is the amount of stormtroopers there are on the board, "obstacles" does the same job as "storm", i enter values to veriables to be sure
    char **ship; // this is the 2D array used to play the game
    char *moveset = NULL; // the moveset leia will perform in the game
    char *cords1, *cords2; // cords1 for the object the player wants to move and cords2 for the destination
    char choice, play_again = 'y', c; // c is for resetting buffer
    stroop *army;
    obs *objects; 
    darth vader;
    princess leia; 
    r2d2 r2; 

    r2.found = 1;

    //srand(13); // My debuggind was done in this seed
    srand(time(NULL)); 
    //A small manual for a more friendly experiance to the user <3
    puts("\n\nWelcome to a galaxy far, far away...\n");
    puts("Before you start your jurney you have to know some keen things.\n");
    puts("1) Leia can move upwards, downwards, right and left (u/U, d/D, r/R, l/L), she cant move into obstacles, Stormtroopers and Darth Vader.");
    puts("2) The player can input multiple moves as one so he cannot waste time, but in the first move she cant do Leia will stop the moveset");
    puts("3) The first time Leia gets caught by a Stormtrooper she will kill him. If she is caught again she will die. This resets every level.");
    puts("4) Darthvader moves every 2 moves of Leia and always goes after her, also he does diagnal moves.");
    puts("5) To proceed to the next level leia must find R2D2, basicly to be in the same tile as him.");
    puts("6) Stormtroopers do horizontal or vertical movement only, if for some reason they cant proceed their move they change direction.");
    puts("7) Leia can use her powers to move obstacles (f/F), she can only do this 2 times per level and cannot place an obstacles on top of another, a Stormtrooper, Darth Vader or R2D2");
    puts("8) Leia can use the help of Master Yoda to reveal the board for one move (h/H)");
    puts("9) Every command counts as a move");
    puts("10) If the player dosent want to play anymore he can input 'x' or 'X'");
    puts("11) For the player to win the game he must get to the minium board with is 10 x 10");
    puts("12) 'L' is Leia, '@' are the Stormtroopers, 'X' are the obstacles, 'D' is Darth Vader and 'R' is R2D2\n\n\n");

    // Just a note, i put curly brackets in the if statements to make the code more readable 
    //and if someone need to fix something he dosent have to put them and make his life harder

    printf("Enter the number of cols (Minumun is 30 and Maximum is 99): ");       
    scanf("%d", &n);
    while(n < 30 || n > 99) 
    {
        printf("Invalid number of cols. Please enter a number between 30 and 99: ");
        scanf("%d", &n); 
    } 

    printf("Enter the number of rows (Minumun is 25 and Maximum is 58): ");
    scanf("%d", &m); 
    while(m < 25 || m > 58)
    {
        printf("Invalid number of rows. Please enter a number between 25 and 58: ");
        scanf("%d", &m);
    }

    starting_m = m;
    staring_n = n;

    puts("\n\nThe difficulties are:\n1 for Easy\n2 for Medium\n3 for Hard\n4 for Imposible\n");
    printf("Enter the difficulty level (1-4): ");
    scanf("%d", &diff); 
    while(diff < 1 || diff > 4)
    {
        printf("Invalid difficulty level. Please enter a number between 1 and 4: ");
        scanf("%d", &diff);
    }

    while((c = getchar()) != '\n' && c != EOF); // clear buffer

    while(1)
    {
        flag_l = 0;
        flag_f = 0;

        if(r2.found == 1 || play_again == 'y')
        {
            captured = 0;
            force_limit = 0;
            play_again = EOF;

            inisialize_board(&ship, n, m);

            generate_level_dependant(diff, n, m, &storm, &obstacles);

            generate_vader(&ship, &vader, n, m);

            generate_stormtroopers(&ship, &army, n, m, storm);

            if(obstacles != 0)
            {
                generate_obsticles(&ship, &objects, n, m, obstacles);
            }

            generate_r2d2(&ship, &r2, n, m);

            generate_leia(&ship, &leia, n, m);
        }

        cover_board(&ship, n, m, leia, vader, r2, army, storm, objects, obstacles, help);

        printf("Level: %d, ", level);
        if(leia.injured == 1)
        {
            printf("Leia status: Injured!!!");
        }
        else
        {
            printf("Leia status: Not Injured");
        }
        printf(", Moves: %d, ", leia.moves);
        printf("Force uses: %d", 2 - force_limit);

        print_board(ship, n, m); // the real print table

        fill_board(&ship, n, m, r2, army, storm);
        //print_board(ship, n, m); // its throught out the code i have it for debugging perpuses 
        choice = read_input(&cords1, &cords2, &moveset, &len, &force_limit);

        help = 0;
  
        if(choice == 'x')
        {
            free_all(&ship, &army, &objects, n, obstacles);
            break;
        }
        else if(choice == 'm')
        {
            for(i = 0; i < len; i++)
            {
                flag_l = move_leia(&ship, &leia, &r2, moveset, n, m, i);
                if(flag_l) // flag is if leias moves are invalid
                {
                    puts("\n\nBecause you impoted a moveset that cannot be done, leia performed the moves she can do until the move she cant do is reached\n");
                    break;
                }

                if(leia.x == r2.x && leia.y == r2.y)
                {
                    r2.found = 1;
                    break;
                }

               move_enemies(&ship, &army, storm, &vader, &captured, &leia, n, m);

               if(captured || leia.injured > 1)
               {
                   break;
               }
            }

            free(moveset);
        }
        else if(choice == 'f')
        {
            if(obstacles == 0)
            {
                puts("\nThere arent any obstacles to move, please enter a valid move!\n");
                continue;
            }

            if(force_limit > 2)
            {
                puts("\nYou have reached the force limit for this game!\n");
                force_limit--;
                continue;
            }

            flag_f = using_force(&ship, cords1, cords2, &objects, obstacles, n, m);
            if(flag_f) // This flag is is the force move cant be done 
            {
                force_limit--;
                continue;
            }

            move_enemies(&ship, &army, storm, &vader, &captured, &leia, n, m);
        }
        else if(choice == 'h')
        {
            help = 1;

            move_enemies(&ship, &army, storm, &vader, &captured, &leia, n, m);    
        }

        if(captured || leia.injured > 1)
        {
            printf("\nDo you want to play again? (y/n): ");
            play_again = getchar();
            while(play_again != 'y' && play_again !='Y' && play_again != 'n' && play_again != 'N')
            {
                printf("\nInvalid choice. Please enter y or n: ");
                play_again = getchar();
            }

            if(isupper(play_again))
            {
                play_again = tolower(play_again);
            }

            free_all(&ship, &army, &objects, n, obstacles);

            if(play_again == 'y')
            {
                n = staring_n;
                m = starting_m;
                level = 1;
            }
            else
            {
                break;
            }

            r2.found = 1;
        }
        else if(r2.found)
        {
            free_all(&ship, &army, &objects, n, obstacles);

            if(n > 10)
            {
                n--;
            }

            if(m > 10)
            {
                m--;
            }

            if(n == 10 && m == 10)
            {
                puts("\n\nCongratulations! You have completed the game!\n");
                break;
            }

            level++;

        }
    }

    puts("\n\nThank you for playing!, we hope you liked the game!!");
    puts("Until next time traveler!\n\n");

    return 0;
}// end of main

void move_enemies(char ***ship, stroop **army, int storm, darth *vader, int *captured, princess *leia, int n, int m)
{
    (leia -> moves)++;

    if((leia -> moves) % 2 == 0)
    {
        *captured = move_vader(ship, vader, leia -> x, leia -> y);
    }

    move_stormtroopers(ship, army, n, m, 0, storm, &(leia -> injured), leia -> x, leia -> y);
}


void free_all(char ***ship, stroop **army, obs **objects, int n, int obstacles)
{
    int i;

    for(i = 0; i < n; i++)
    {
        free((*ship)[i]);
    }

    free(*ship);
    free(*army);
    if(obstacles != 0)
    {
        free(*objects);
    }
}

void print_board(char **ship, int n, int m)
{
    int i, j;
    char c;

    puts("\n");
    printf("\t     ");
    for(c = 'A'; c < 'A' + m; c++) 
    {
        if(c > 'Z')
        {
            printf("%c%c ", (c - 'A') / 26 + 'A' - 1, (c - 'A') % 26 + 'A');
        }
        else
        {
            printf("%c  ", c);
        }
    }
    putchar('\n');
    printf("\t    ");
    for(c = 'A'; c < 'A' + m; c++)
    {
        printf("___");
    }
    putchar('\n');
    
    for(i = 0; i < n; i++)
    {
        printf("\t%2d |", i + 1); 

        for(j = 0; j < m; j++)
        {
            printf(" %c ", ship[i][j]); 
        }

        putchar('\n');
    }

    
}

void check_malloc(void *p)
{
    if(p == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

int read_text(char str[], int size, int flag)
{
    int len;

    if(fgets(str, size, stdin) == NULL)
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    len = strlen(str);
    if(len > 0)
    {
        if(flag && str[len - 1] == '\n')
        {
            str[len - 1] = '\0';
            len--;
        }
        else
        {
            printf("Error: too few characters\n");
            exit(EXIT_FAILURE);
        }
        
        return len;
    }

    return EOF;
}

int random_number(int n)
{
    double d;

    d = (double) rand() / ( (double) RAND_MAX + 1);
    d = d * n;
    return (int) d;
}

void inisialize_board(char ***ship, int n, int m)
{
    int i, j; 

    *ship = (char**) malloc(n * sizeof(char*)); 

    check_malloc(*ship);

    for(i = 0; i < n; i++)
    {
        (*ship)[i] = (char*) malloc(m * sizeof(char)); 
        check_malloc((*ship)[i]);
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            (*ship)[i][j] = '#';
        }
    }    
}

void generate_level_dependant(int level, int n, int m, int *storm, int *obstacles)
{
    switch (level)
    {
        case 1:
            *storm = n * m * 2 / 100;

            *obstacles = n * m * 13 / 100;
         break;

        case 2:
            *storm = n * m * 5 / 100;

            *obstacles = n * m * 10 / 100;
         break;
    
        case 3:
            *storm = n * m * 10 / 100;

            *obstacles = n * m * 5 / 100;
         break;
        
        case 4:
            *storm = n * m * 15 / 100;

            *obstacles = 0; //evil option :(
         break;
    }

    if (*storm < 2)
    {
        *storm = 2;
    }
            
}

void generate_stormtroopers(char ***ship, stroop **army, int n, int m, int storm)
{
    int i, x, y;

    *army = (stroop*) malloc(storm * sizeof(stroop));
    check_malloc(*army);

    for(i = 0; i < storm; i++)
    {     
        x = random_number(n);
        y = random_number(m);

        while((*ship)[x][y] == 'X' || (*ship)[x][y] == 'D' || (*ship)[x][y] == '@')
        {
            x = random_number(n);
            y = random_number(m);
        }

        (*ship)[x][y] = '@';
        
        (*army)[i].x = x;
        (*army)[i].y = y;
        (*army)[i].alive = 1;

        (*army)[i].direction = rand() % 2; // 0 for horizontal movement, 1 for vertical movement
        (*army)[i].bounds = rand() % 2; // 0 for right, 1 for left or 0 for down, 1 for up
    }

    // print_board(*ship, n, m);
}

void generate_vader(char ***ship, darth *vader, int n, int m)
{
    int i;

    i = random_number(4);

    switch(i + 1)
    {
        case 1:
            vader -> x = 0;
            vader -> y = 0;

            (*ship)[0][0] = 'D';
         break;
        
        case 2:
            vader -> x = n - 1;
            vader -> y = 0;

            (*ship)[n - 1][0] = 'D';
         break;

        case 3:
            vader -> x = 0;
            vader -> y = m - 1;

            (*ship)[0][m - 1] = 'D';
         break;

        case 4:
            vader -> x = n - 1;
            vader -> y = m - 1;

            (*ship)[n - 1][m - 1] = 'D';
         break;
    }
}

void generate_obsticles(char ***ship, obs **objects, int n, int m, int obstacles)
{
    int i, x, y;

    *objects = (obs*) malloc(obstacles * sizeof(obs));
    check_malloc(*objects);

    for(i = 0; i < obstacles; i++)
    {     
        x = random_number(n);
        y = random_number(m);

        while((*ship)[x][y] == 'X' || (*ship)[x][y] == 'D' || (*ship)[x][y] == '@')
        {
            x = random_number(n);
            y = random_number(m);
        }

        (*ship)[x][y] = 'X';
        
        (*objects)[i].x = x;
        (*objects)[i].y = y;
    }

    //print_board(*ship, n, m);

}

void generate_leia(char ***ship, princess *leia, int n, int m)
{
    int x, y;

    x = random_number(n);
    y = random_number(m);

    while((*ship)[x][y] == 'X' || (*ship)[x][y] == 'D' || (*ship)[x][y] == '@' || (*ship)[x][y] == 'R')
    {
        x = random_number(n);
        y = random_number(m);
    }

    (*ship)[x][y] = 'L';
    
    leia -> x = x;
    leia -> y = y;

    leia -> moves = 0;
    leia -> injured = 0;
}

void generate_r2d2(char ***ship, r2d2 *r2, int n, int m)
{
    int x, y;

    x = random_number(n);
    y = random_number(m);

    while((*ship)[x][y] == 'X' || (*ship)[x][y] == 'D' || (*ship)[x][y] == '@')
    {
        x = random_number(n);
        y = random_number(m);
    }

    (*ship)[x][y] = 'R';
    
    r2 -> x = x;
    r2 -> y = y;
    r2 -> found = 0;
}

void move_stormtroopers(char ***ship, stroop **army, int n, int m, int i, int storm, int *injured, int leia_x, int leia_y)
{
    if(i >= storm || i < 0)
    {
        return;
    }

    if((*army)[i].alive == 0)
    {
        move_stormtroopers(ship, army, n, m, i + 1, storm, injured, leia_x, leia_y);
        return;
    }

    (*ship)[(*army)[i].x][(*army)[i].y] = '#';

    if((*army)[i].direction == 0 && (*army)[i].bounds == 0) // right movement
    {
        if((*army)[i].y + 1 <= m - 1 && ((*ship)[(*army)[i].x][(*army)[i].y + 1] == '#' || (*ship)[(*army)[i].x][(*army)[i].y + 1] == '.' || (*ship)[(*army)[i].x][(*army)[i].y + 1] == 'L'))
        {
            (*army)[i].y++;
        }
        else if((*army)[i].y - 1 >= 0 && ((*ship)[(*army)[i].x][(*army)[i].y - 1] == '#' || (*ship)[(*army)[i].x][(*army)[i].y - 1] == '.' || (*ship)[(*army)[i].x][(*army)[i].y - 1] == 'L'))
        {
            (*army)[i].bounds = 1;
            (*army)[i].y--;
        }
        else
        {
            (*ship)[(*army)[i].x][(*army)[i].y] = '@';
        }
    }
    else if((*army)[i].direction == 0 && (*army)[i].bounds == 1) // left movement
    {
        if((*army)[i].y - 1 >= 0 && ((*ship)[(*army)[i].x][(*army)[i].y - 1] == '#' || (*ship)[(*army)[i].x][(*army)[i].y - 1] == '.' || (*ship)[(*army)[i].x][(*army)[i].y - 1] == 'L'))
        {
            (*army)[i].y--;
        }
        else if((*army)[i].y + 1 <= m - 1 && ((*ship)[(*army)[i].x][(*army)[i].y + 1] == '#' || (*ship)[(*army)[i].x][(*army)[i].y + 1] == '.' || (*ship)[(*army)[i].x][(*army)[i].y + 1] == 'L'))
        {
            (*army)[i].bounds = 0;
            (*army)[i].y++;
        }
        else
        {
            (*ship)[(*army)[i].x][(*army)[i].y] = '@';
        }
    }
    else if((*army)[i].direction == 1 && (*army)[i].bounds == 0) // down movement
    { 
        if((*army)[i].x + 1 <= n - 1 && ((*ship)[(*army)[i].x + 1][(*army)[i].y] == '#' || (*ship)[(*army)[i].x + 1][(*army)[i].y] == '.' || (*ship)[(*army)[i].x + 1][(*army)[i].y] == 'L'))
        {
            (*army)[i].x++;
        }
        else if((*army)[i].x - 1 >= 0 && ((*ship)[(*army)[i].x - 1][(*army)[i].y] == '#' || (*ship)[(*army)[i].x - 1][(*army)[i].y] == '.' || (*ship)[(*army)[i].x - 1][(*army)[i].y] == 'L'))
        {
            (*army)[i].bounds = 1;
            (*army)[i].x--;
        }
        else
        {
            (*ship)[(*army)[i].x][(*army)[i].y] = '@';
        }
    }
    else if((*army)[i].direction == 1 && (*army)[i].bounds == 1) // up movement
    {
        if ((*army)[i].x - 1 >= 0 && ((*ship)[(*army)[i].x - 1][(*army)[i].y] == '#' || (*ship)[(*army)[i].x - 1][(*army)[i].y] == '.' || (*ship)[(*army)[i].x - 1][(*army)[i].y] == 'L'))
        {
            (*army)[i].x--;
        }
        else if((*army)[i].x + 1 <= n - 1 && ((*ship)[(*army)[i].x + 1][(*army)[i].y] == '#' || (*ship)[(*army)[i].x + 1][(*army)[i].y] == '.' || (*ship)[(*army)[i].x + 1][(*army)[i].y] == 'L'))
        {
            (*army)[i].bounds = 0;
            (*army)[i].x++;
        }
        else
        {
            (*ship)[(*army)[i].x][(*army)[i].y] = '@';
        }
    }

    if((*army)[i].x == leia_x && (*army)[i].y == leia_y)
    {
        (*injured)++;
        (*army)[i].alive = 0;
        (*army)[i].x = -1;
        (*army)[i].y = -1;

        if(*injured == 1)
        {
            puts("Leia was caught by a stormtrooper! But she managed to kill him!");
            puts("Now Leia is injured, next time she encounters a stormtrooper she will die!");
        }
        else
        {
            puts("Leia was caught by a stormtrooper and because she was injured she got captured!");
            return;
        }
    }

    if((*army)[i].alive == 1)
    {
        (*ship)[(*army)[i].x][(*army)[i].y] = '@';
    }

    move_stormtroopers(ship, army, n, m, i + 1, storm, injured, leia_x, leia_y);
}

int move_vader(char ***ship, darth *vader, int leia_x, int leia_y)
{
    (*ship)[vader -> x][vader -> y] = '#';
    // For this methond i did it in papper and it represents how far vader and leia are
    //based on the dx and dy it always does the best move it can to get closer to leia
    int dx = leia_x - vader -> x;
    int dy = leia_y - vader -> y;

    if(dx > 0)
    {
        if(dy > 0)
        {
            if((*ship)[vader -> x + 1][vader -> y + 1] != 'X' && (*ship)[vader -> x + 1][vader -> y + 1] != 'R' && (*ship)[vader -> x + 1][vader -> y + 1] != '@')
            {
                vader -> x++;
                vader -> y++;
            }
            else if((*ship)[vader -> x + 1][vader -> y] != 'X' && (*ship)[vader -> x + 1][vader -> y] != 'R' && (*ship)[vader -> x + 1][vader -> y] != '@')
            {
                vader -> x++;
            }
            else if((*ship)[vader -> x][vader -> y + 1] != 'X' && (*ship)[vader -> x][vader -> y + 1] != 'R' && (*ship)[vader -> x][vader -> y + 1] != '@')
            {
                vader -> y++;
            }
        }
        else if(dy < 0)
        {
            if((*ship)[vader -> x + 1][vader -> y - 1] != 'X' && (*ship)[vader -> x + 1][vader -> y - 1] != 'R' && (*ship)[vader -> x + 1][vader -> y - 1] != '@')
            {
                vader -> x++;
                vader -> y--;
            }
            else if((*ship)[vader -> x + 1][vader -> y] != 'X' && (*ship)[vader -> x + 1][vader -> y] != 'R' && (*ship)[vader -> x + 1][vader -> y] != '@')
            {
                vader -> x++;
            }
            else if((*ship)[vader -> x][vader -> y - 1] != 'X' && (*ship)[vader -> x][vader -> y - 1] != 'R' && (*ship)[vader -> x][vader -> y - 1] != '@')
            {
                vader -> y--;
            }
        }
        else
        {
            if((*ship)[vader -> x + 1][vader -> y] != 'X' && (*ship)[vader -> x + 1][vader -> y] != 'R' && (*ship)[vader -> x + 1][vader -> y] != '@')
            {
                vader -> x++;
            }
        }
    } 
    else if(dx < 0) 
    {
        if(dy > 0)
        {
            if((*ship)[vader -> x - 1][vader -> y + 1] != 'X' && (*ship)[vader -> x - 1][vader -> y + 1] != 'R' && (*ship)[vader -> x - 1][vader -> y + 1] != '@')
            {
                vader -> x--;
                vader -> y++;
            }
            else if ((*ship)[vader -> x - 1][vader -> y] != 'X' && (*ship)[vader -> x - 1][vader -> y] != 'R' && (*ship)[vader -> x - 1][vader -> y] != '@')
            {
                vader -> x--;
            }
            else if ((*ship)[vader -> x][vader -> y + 1] != 'X' && (*ship)[vader -> x][vader -> y + 1] != 'R' && (*ship)[vader -> x][vader -> y + 1] != '@')
            {
                vader -> y++;
            }
        }
        else if(dy < 0)
        {
            if((*ship)[vader -> x - 1][vader -> y - 1] != 'X' && (*ship)[vader -> x - 1][vader -> y - 1] != 'R' && (*ship)[vader -> x - 1][vader -> y - 1] != '@')
            {
                vader -> x--;
                vader -> y--;
            }
            else if((*ship)[vader -> x - 1][vader -> y] != 'X' && (*ship)[vader -> x - 1][vader -> y] != 'R' && (*ship)[vader -> x - 1][vader -> y] != '@')
            {
                vader -> x--;
            }
            else if((*ship)[vader -> x][vader -> y - 1] != 'X' && (*ship)[vader -> x][vader -> y - 1] != 'R' && (*ship)[vader -> x][vader -> y - 1] != '@')
            {
                vader -> y--;
            }
        }
        else
        {
            if((*ship)[vader -> x - 1][vader -> y] != 'X' && (*ship)[vader -> x - 1][vader -> y] != 'R' && (*ship)[vader -> x - 1][vader -> y] != '@')
            {
                vader -> x--;
            }
        }
    }
    else
    {
        if(dy > 0)
        {
            if((*ship)[vader -> x][vader -> y + 1] != 'X' && (*ship)[vader -> x][vader -> y + 1] != 'R' && (*ship)[vader -> x][vader -> y + 1] != '@')
            {
                vader -> y++;
            }
        }
        else if(dy < 0)
        {
            if((*ship)[vader -> x][vader -> y - 1] != 'X' && (*ship)[vader -> x][vader -> y - 1] != 'R' && (*ship)[vader -> x][vader -> y - 1] != '@')
            {
                vader -> y--;
            }
        }
    }
    
    if(vader -> x == leia_x && vader -> y == leia_y)
    {
        puts("Leia was caught by Darth Vader and was captured!!");
        return 1;
    }

    (*ship)[vader -> x][vader -> y] = 'D';

    return 0;
}

void cover_board(char ***ship, int n, int m, princess leia, darth vader, r2d2 r2, stroop *army, int storm, obs *objects, int obstacles, int help)
{
    int i, j;
    switch(help)
    {
        case 0:
            for(i = 0; i < n; i++)
            {
                for(j = 0; j < m; j++)
                {
                    if((*ship)[i][j] == '@' || (*ship)[i][j] == 'R' || (*ship)[i][j] == '.')
                    {
                        (*ship)[i][j] = '#';
                    }
                }
            }

            for(i = 0; i < n; i++)
            {
                if((i == r2.x && leia.y == r2.y))
                {
                    (*ship)[i][leia.y] = 'R';
                    continue;
                }

                for(j = 0; j < storm; j++)
                {
                    if(army[j].x == i && army[j].y == leia.y)
                    {
                        (*ship)[i][leia.y] = '@';
                    }
                }

                if((*ship)[i][leia.y] != '@' && (*ship)[i][leia.y] != 'D' && (*ship)[i][leia.y] != 'L' && (*ship)[i][leia.y] != 'R' && (*ship)[i][leia.y] != 'X')
                {
                    (*ship)[i][leia.y] = '.';
                }

            }

            for(i = 0; i < m; i++)
            {
                if((leia.x == r2.x && i == r2.y))
                {
                    (*ship)[leia.x][i] = 'R';
                    continue;
                }
            
                for(j = 0; j < storm; j++)
                {
                    if(army[j].x == leia.x && army[j].y == i)
                    {
                        (*ship)[leia.x][i] = '@';
                    }
                }

                if((*ship)[leia.x][i] != '@' && (*ship)[leia.x][i] != 'D' && (*ship)[leia.x][i] != 'L' && (*ship)[leia.x][i] != 'R' && (*ship)[leia.x][i] != 'X')
                {
                    (*ship)[leia.x][i] = '.';
                }
            }
         break;

        case 1:
        
            for(i =0; i < n; i++)
            {
                for(j = 0; j < m; j++)
                {
                    if((*ship)[i][j] == '#')
                    {
                        (*ship)[i][j] = '.';
                    }
                }
            }

         break;
    }
}

void fill_board(char ***ship, int n, int m, r2d2 r2, stroop *army, int storm) // i do this so the board can intercat with the move functions
{
    int i;

    for(i = 0; i < storm; i++)
    {
        if(army[i].alive)
        {
            (*ship)[army[i].x][army[i].y] = '@';
        }
    }

    (*ship)[r2.x][r2.y] = 'R';
}

char read_input(char **token, char **token2, char **moveset, int *size, int *force_limit)
{
    char choice;
    char str[100] = {0};
    char *test_token;
    int i, len;

    while(1)
    {
        printf("Enter your choice: "); 
        len = read_text(str, 100, 1);

        *token = strtok(str, ">"); // to see if the user inputed a force command
        if(*token != NULL)
        {
            *token2 = strtok(NULL, ">");
            if(*token2 != NULL)
            {
                test_token = strtok(NULL, ">");
                if(test_token != NULL)
                {
                    printf("Invalid choice. Please enter a valid move\n");
                    continue;
                }
                else
                {
                    (*force_limit)++;
                    return 'f';
                }
            }
        }

        choice = tolower(str[0]);

        if((choice == 'h' || choice == 'x') && len == 1) // checking for help or exit, syntax sesitive, but case insensitive
        {
            return choice;
        }
        else // else its moves for leia
        {
            for(i = 0; i < len; i++)
            {
                if(isupper(str[i]))
                {
                    str[i] = tolower(str[i]);
                }
    
                if(str[i] != 'u' && str[i] != 'd' && str[i] != 'l' && str[i] != 'r') // if its somethign diffrent its nothing so stop
                {
                    break;
                }
            }
    
            if(i == len) // checking if all went right
            {
                *moveset = (char *)malloc(len * sizeof(char)); 
                // i choose to do a malloc here so i can keep the dynamic theme and not send the static buffer
                check_malloc(*moveset);
    
                for(i = 0; i < len; i++)
                {
                    (*moveset)[i] = str[i];
                }
    
                *size = len;
                return 'm';
            }
        }
        
        puts("\nInvalid choice. Please enter a valid move");
    }
}

int using_force(char ***ship, char *token, char *token2, obs **objects, int obstacles, int n, int m)
{
    int i, x1, x2, y1, y2, dig1, dig2, let1, let2;
    int len1 = strlen(token), len2 = strlen(token2);;

    for(i = 0; i < len1; i++)
    {
        if(isalpha(token[i]))
        {
            token[i] = toupper(token[i]);
            token[i] = token[i] - 'A';
            let1++;
        }
        else
        {
            dig1++;
        }
    }

    for(i = 0; i < len2; i++)
    {
        if(isalpha(token2[i]))
        {
            token2[i] = toupper(token2[i]);
            token2[i] = token2[i] - 'A';
            let2++;
        }
        else
        {
            dig2++;
        }
    }

    if(!dig1 || !dig2 || !let1 || !let2)
    {
        puts("\nInvalid input. Please enter a valid move");
        return 1;
    }
    

    x1 = atoi(token + let1) - 1;
    if(x1 < 0 || x1 > n - 1)
    {
        puts("\nInvalid input. Please enter a valid move");
        return 1;
    }

    switch(let1)
    {
        case 1:
            y1 = token[0];
         break;
    
        case 2:
            y1 = token[1];
            switch(token[0]) // calculates double letter cord, maths done in papper
            {
                case 0:
                    y1 += 26;
                 break;

                case 1:
                    y1 += 51;
                 break;
            }
         break;
    }

    if(y1 < 0 || y1 > m - 1)
    {
        puts("\nInvalid input. Please enter a valid move");
        return 1;
    }

    x2 = atoi(token2 + let2) - 1;
    if(x2 < 0 || x2 > n - 1)
    {
        puts("\nInvalid input. Please enter a valid move");
        return 1;
    }

    switch(let2)
    {
        case 1:
            y2 = token2[0];
         break;
    
        case 2:
            y2 = token2[1];
            switch(token2[0]) // calculates double letter cord, maths done in papper
            {
                case 0:
                    y2 += 26;
                 break;

                case 1:
                    y2 += 51;
                 break;
            }
         break;
    }

    if(y2 < 0 || y2 > m - 1)
    {
        puts("\nInvalid input. Please enter a valid move");
        return 1;
    }

    if((*ship)[x2][y2] != '#' && (*ship)[x2][y2] != '.')
    {
        puts("\nCant move object on the tile you want because someone or something is on the destination :(");
        return 1;
    }
    else if((*ship)[x1][y1] != 'X')
    {
        puts("\nYou cant move an object that is not an obstacle");
        return 1;
    }

    for(i = 0; i < obstacles; i++)
    {
        if((*objects)[i].x == x1 && (*objects)[i].y == y1) // finding the right obstacle to move and to change its cords
        {
            (*ship)[(*objects)[i].x][(*objects)[i].y] = '#';
            (*objects)[i].x = x2;
            (*objects)[i].y = y2;
            (*ship)[x2][y2] = 'X';
            break;
        }
    }

    return 0;
}

int move_leia(char ***ship, princess *leia, r2d2 *r2, char *moveset, int n, int m, int offset_moveset)
{
    switch(moveset[offset_moveset])
    {
        case 'l':
            if(leia -> y - 1 < 0 || ((*ship)[leia -> x][leia -> y - 1] != '.' && (*ship)[leia -> x][leia -> y - 1] != '#' && (*ship)[leia -> x][leia -> y - 1] != 'R'))
            {
                return 1;
            }
            else
            {
                (*ship)[leia -> x][leia -> y] = '.';
                leia -> y--;
                (*ship)[leia -> x][leia -> y] = 'L';
            }
         break;

        case 'r':
            if(leia -> y + 1 > m - 1 || ((*ship)[leia -> x][leia -> y + 1] != '.' && (*ship)[leia -> x][leia -> y + 1] != '#' && (*ship)[leia -> x][leia -> y + 1] != 'R'))
            {
                return 1;
            }
            else
            {
                (*ship)[leia -> x][leia -> y] = '.';
                leia -> y++;
                (*ship)[leia -> x][leia -> y] = 'L';
            }
         break;
        
        case 'u':
            if(leia -> x - 1 < 0 || ((*ship)[leia -> x - 1][leia -> y] != '.' && (*ship)[leia -> x - 1][leia -> y] != '#' && (*ship)[leia -> x - 1][leia -> y] != 'R'))
            {
                return 1;
            }
            else
            {
                (*ship)[leia -> x][leia -> y] = '.';
                leia -> x--;
                (*ship)[leia -> x][leia -> y] = 'L';
            }
         break;

        case 'd':
            if(leia -> x + 1 > n - 1 || ((*ship)[leia -> x + 1][leia -> y] != '.'  && (*ship)[leia -> x + 1][leia -> y] != '#' && (*ship)[leia -> x + 1][leia -> y] != 'R'))
            {
                return 1;
            }
            else
            {
                (*ship)[leia -> x][leia -> y] = '.';
                leia -> x++;
                (*ship)[leia -> x][leia -> y] = 'L';
            }
         break;
    }

    return 0;
}