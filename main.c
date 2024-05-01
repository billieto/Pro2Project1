#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Where x and y the cordinates of the caracter/ obstacle (x,y) in the 2D array
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

void check_malloc(void *p);
int move_leia(char ***ship, princess *leia, r2d2 *r2, char *moveset, int n, int m, int offset_moveset);
int move_vader(char ***ship, darth *vader, int leia_x, int leia_y);
void move_stormtroopers(char ***ship, stroop **army, int n, int m, int storm, int *injured, int leia_x, int leia_y);
void generate_stormtroopers(char ***ship, stroop **army, int n, int m, int storm); 
void generate_vader(char ***ship, darth *vader, int n, int m);
void generate_leia(char ***ship, princess *leia, int n, int m);
void generate_r2d2(char ***ship, r2d2 *r2, int n, int m); 
void generate_obsticles(char ***ship, obs **objects, int n, int m, int obstacles);
void generate_level_dependant(int level, int n, int m, int *storm, int *obstacles);
int using_force(char ***ship, char *token, char *token2, obs **objects, int obstacles, int n, int m);
void print_board(char **ship, int n, int m);
void fill_board(char ***ship, int n, int m, r2d2 r2, stroop *army, int storm);
void inisialize_board(char ***ship, int n, int m);
void free_all(char ***ship, stroop **army, obs **objects, char **moveset, int n);
void cover_board(char ***ship, int n, int m, princess leia, darth vader, r2d2 r2, stroop *army, int storm, obs *objects, int obstacles, int help);
char read_input(char **token, char **token2, char **moveset, int *size, int *force_limit);
int random_number(int n);
int read_text(char str[], int size, int flag); // from mr. Tselika's book


int main(void)
{
    int i, n, m, diff, storm = 2, obstacles, level = 0, len, flag_l = 0, captured = 0, help = 0, force_limit = 0, flag_f = 0;
    char **ship; // this is the 2D array used to play the game
    char *moveset; // the moveset leia will perform in the game
    char *cords1, *cords2; // cords1 for the object the player wants to move and cords2 for the destination
    char choice, play_again = 'y', c;
    stroop *army;
    obs *objects;
    darth vader;
    princess leia;
    r2d2 r2;

    //srand(13);
    srand(time(NULL)); 

    puts("\n\nWelcome to a galaxy far, far away...\n");

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

    printf("\n\nThe difficulties are:\n1 for Easy\n2 for Medium\n3 for Hard\n4 for Imposible\n\n");
    printf("Enter the difficulty level (1-4): ");
    scanf("%d", &diff); 
    while(diff < 1 || diff > 4)
    {
        printf("Invalid difficulty level. Please enter a number between 1 and 4: ");
        scanf("%d", &diff);
    }

    while((c = getchar()) != '\n' && c != EOF);

    while(n > 10 || m > 10)
    {
        flag_l = 0;
        flag_f = 0;

        if(r2.found == 1 || play_again == 'y' || play_again == 'Y')
        {
            captured = 0;
            play_again = 0;
            force_limit = 0;
            level++;

            if(level > 1)
            {
                if(n > 10)
                {
                    n--;
                }
                
                if(m > 10)
                {
                    m--;
                }

                free_all(&ship, &army, &objects, &moveset, n);
            }

            inisialize_board(&ship, n, m);

            generate_level_dependant(diff, n, m, &storm, &obstacles);

            generate_vader(&ship, &vader, n, m);

            generate_stormtroopers(&ship, &army, n, m, storm);

            generate_obsticles(&ship, &objects, n, m, obstacles);

            generate_r2d2(&ship, &r2, n, m);

            generate_leia(&ship, &leia, n, m);
        }

        cover_board(&ship, n, m, leia, vader, r2, army, storm, objects, obstacles, help);
        print_board(ship, n, m); // the real print table

        fill_board(&ship, n, m, r2, army, storm);
        //print_board(ship, n, m);
        choice = read_input(&cords1, &cords2, &moveset, &len, &force_limit);

        help = 0;
        // ta choice na prospathisw na ta kanw me switch case
        
        if(choice == 'x')
        {
            free_all(&ship, &army, &objects, &moveset, n);
            break;
        }
        else if(moveset != NULL) // choice == 'm'
        {
            for(i = 0; i < len; i++)
            {
                flag_l = move_leia(&ship, &leia, &r2, moveset, n, m, i);
                if(flag_l)
                {
                    printf("\nBecause you impoted a moveset that cannot be done, leia performed the moves she can do until the move she cant do is reached\n");
                    break; // flag is if leias moves are invalid
                }

                if(leia.x == r2.x && leia.y == r2.y)
                {
                    r2.found = 1;
                    break;
                }

                leia.moves++;
                
                if(leia.moves % 2 == 0)
                {
                    captured = move_vader(&ship, &vader, leia.x, leia.y);
                }

                if(captured)
                {
                    break;
                }

                move_stormtroopers(&ship, &army, n, m, storm, &leia.injured, leia.x, leia.y);
                if(leia.injured > 1)
                {
                    break;
                }

                // cover_board(&ship, n, m, leia, vader, r2, army, storm, objects, obstacles, help);
                //fill_board(&ship, n, m, r2, army, storm);
            }
        }
        else if(choice == 'f')
        {
            if(force_limit > 2)
            {
                puts("You have reached the force limit for this game!");
                continue;
            }

            flag_f = using_force(&ship, cords1, cords2, &objects, obstacles, n, m);
            if(flag_f)
            {
                force_limit--;
                continue;
            }

            leia.moves++;
            
            if(leia.moves % 2 == 0)
            {
                captured = move_vader(&ship, &vader, leia.x, leia.y);
            }

            move_stormtroopers(&ship, &army, n, m, storm, &leia.injured, leia.x, leia.y);
        }
        else if(choice == 'h')
        {
            help = 1;
            leia.moves++;

            if(leia.moves % 2 == 0)
            {
                captured = move_vader(&ship, &vader, leia.x, leia.y);
            }

            move_stormtroopers(&ship, &army, n, m, storm, &leia.injured, leia.x, leia.y);    
        }

        if(captured || leia.injured > 1)
        {
            printf("Do you want to play again? (y/n): ");
            play_again = getchar();
            while(play_again != 'y' && play_again !='Y' && play_again != 'n' && play_again != 'N')
            {
                printf("Invalid choice. Please enter y or n: ");
                play_again = getchar();
            }

            if(play_again == 'n' || play_again == 'N')
            {
                break;
            }
            else
            {
                free_all(&ship, &army, &objects, &moveset, n);
            }
        }
    }

    if(n == 10 && m == 10)
    {
        puts("Congratulations! You have completed the game!\n");
    }

    puts("Thank you for playing!, we hope you liked the game!!");
    puts("Until next time traveler!");

    return 0;
}// end of main

void free_all(char ***ship, stroop **army, obs **objects, char **moveset, int n)
{
    int i;

    for(i = 0; i < n; i++)
    {
        free((*ship)[i]);
    }

    free(*ship);
    free(*army);
    free(*objects);
    //free(*moveset);
}

void print_board(char **ship, int n, int m)
{
    int i, j;
    char c;

    puts("\n");
    printf("\t     ");
    for (c = 'A'; c < 'A' + m; c++) 
    {
        if (c > 'Z') {
            printf("%c%c ", (c - 'A') / 26 + 'A' - 1, (c - 'A') % 26 + 'A');
        } else {
            printf("%c  ", c);
        }
    }
    putchar('\n');
    printf("\t    ");
    for (c = 'A'; c < 'A' + m; c++)
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
    if (p == NULL)
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

    *ship = (char **)malloc(n * sizeof(char *)); 

    check_malloc(*ship);

    for(i = 0; i < n; i++)
    {
        (*ship)[i] = (char *)malloc(m * sizeof(char)); 
        check_malloc((*ship)[i]);
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            (*ship)[i][j] = '#';
        }
    }// i know curly brackets are not needed but i want to have them if my partner wants to add something and save him time    
}

void generate_level_dependant(int level, int n, int m, int *storm, int *obstacles)
{
    switch (level)
    {
        case 1:
            *storm = n * m * 2 / 100;

            if (*storm < 2)
            {
                *storm = 2;
            }
            
            *obstacles = n * m * 13 / 100;
         break;

        case 2:
            *storm = n * m * 5 / 100;

            if (*storm < 2)
            {
                *storm = 2;
            }
            
            *obstacles = n * m * 10 / 100;
         break;
    
        case 3:
            *storm = n * m * 10 / 100;

            if (*storm < 2)
            {
                *storm = 2;
            }
            
            *obstacles = n * m * 5 / 100;
         break;
        
        case 4:
            *storm = n * m * 15 / 100;
            
            if (*storm < 2)
            {
                *storm = 2;
            }
            
            *obstacles = 0; //evil option :(
         break;
    }
}

void generate_stormtroopers(char ***ship, stroop **army, int n, int m, int storm)
{
    int i, x, y;

    *army = (stroop *)malloc(storm * sizeof(stroop));
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

        switch(rand() % 2)
        {
            case 0:
                (*army)[i].direction = 0; // horimove_leiazontal movement for the stromtrooper
                (*army)[i].bounds = rand() % 2; // 0 for right, 1 for left
             break;

            case 1:
                (*army)[i].direction = 1; // vertical movement for the stormtrooper
                (*army)[i].bounds = rand() % 2; // 0 for down, 1 for up
             break;
        }
    }

    // print_board(*ship, n, m);
}

void generate_vader(char ***ship, darth *vader, int n, int m)
{
    int i;

    //akra tou vader 0,0 / m-1,0 / 0,n-1 / n-1,m-1

    i = random_number(4);
    // i have to change the switch so i can function with n > m or m > n
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

    *objects =  (obs*) malloc(obstacles * sizeof(obs));
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

void move_stormtroopers(char ***ship, stroop **army, int n, int m, int storm, int *injured, int leia_x, int leia_y)
{
    int i;

    for(i = 0; i < storm; i++)
    {
        if(!(*army)[i].alive)
        {
            continue;
        }

        (*ship)[(*army)[i].x][(*army)[i].y] = '#';

        switch((*army)[i].direction)
        {
            case 0: // horizontal movment
                switch((*army)[i].bounds) 
                {
                    case 0: // 0 for right
                        if((*army)[i].y + 1 > m - 1 || ((*ship)[(*army)[i].x][(*army)[i].y + 1] != '#' && (*ship)[(*army)[i].x][(*army)[i].y + 1] != '.' && (*ship)[(*army)[i].x][(*army)[i].y + 1] != 'L'))
                        {
                            (*army)[i].bounds = 1;
                            (*army)[i].y--;
                        }
                        else if((*ship)[(*army)[i].x][(*army)[i].y + 1] != '@' && (*ship)[(*army)[i].x][(*army)[i].y + 1] != 'D' && (*ship)[(*army)[i].x][(*army)[i].y + 1] != 'R')
                        {
                            (*army)[i].y++;
                        }
                        else
                        {
                            continue;
                        }
                     break;

                    case 1: // 1 for left
                        if((*army)[i].y - 1  < 0 || ((*ship)[(*army)[i].x][(*army)[i].y - 1] != '#' && (*ship)[(*army)[i].x][(*army)[i].y - 1] != '.' && (*ship)[(*army)[i].x][(*army)[i].y - 1] != 'L'))
                        {
                            (*army)[i].bounds = 0;
                            (*army)[i].y++;
                        }
                        else if((*ship)[(*army)[i].x][(*army)[i].y - 1] != '@' && (*ship)[(*army)[i].x][(*army)[i].y - 1] != 'D' && (*ship)[(*army)[i].x][(*army)[i].y - 1] != 'R')
                        {
                            (*army)[i].y--;
                        }
                        else
                        {
                            continue;
                        }
                     break;
                }
             break;

            case 1: // vertical movement
                switch((*army)[i].bounds) // 0 for down
                {
                    case 0:
                        if((*army)[i].x + 1 > n - 1 || ((*ship)[(*army)[i].x + 1][(*army)[i].y] != '#' && (*ship)[(*army)[i].x + 1][(*army)[i].y] != '.' && (*ship)[(*army)[i].x + 1][(*army)[i].y] != 'L'))
                        {
                            (*army)[i].bounds = 1;
                            (*army)[i].x--;
                        }
                        else if((*ship)[(*army)[i].x + 1][(*army)[i].y] != '@' && (*ship)[(*army)[i].x + 1][(*army)[i].y] != 'D' && (*ship)[(*army)[i].x + 1][(*army)[i].y] != 'R')
                        {
                            (*army)[i].x++;
                        }
                        else
                        {
                            continue;
                        }
                     break;

                    case 1: // 1 for up
                        if((*army)[i].x - 1 < 0 || ((*ship)[(*army)[i].x - 1][(*army)[i].y] != '#' && (*ship)[(*army)[i].x - 1][(*army)[i].y] != '.' && (*ship)[(*army)[i].x - 1][(*army)[i].y] != 'L'))
                        {
                            (*army)[i].bounds = 0;
                            (*army)[i].x++;
                        }
                        else if((*ship)[(*army)[i].x - 1][(*army)[i].y] != '@' && (*ship)[(*army)[i].x - 1][(*army)[i].y] != 'D' && (*ship)[(*army)[i].x - 1][(*army)[i].y] != 'R')
                        {
                            (*army)[i].x--;
                        }
                        else
                        {
                            continue;
                        }
                     break;
                }
             break;
        }
        
        if((*army)[i].x == leia_x && (*army)[i].y == leia_y)
        {
            (*injured)++;
            (*army)[i].alive = 0;
            (*army)[i].x = -1;
            (*army)[i].y = -1;
            puts("Leia was caugh by a stormtrooper! But she managed to kill him!");
            puts("Now leia is injured, next time she will encounter a stormtrooper she will die!");
        }
        
        if((*injured) > 1)
        {
            break;
        }
        //print_board(*ship, n, m);
        if((*army)[i].alive)
        {
            (*ship)[(*army)[i].x][(*army)[i].y] = '@';
        }
    }
}

int move_vader(char ***ship, darth *vader, int leia_x, int leia_y)
{
    (*ship)[vader -> x][vader -> y] = '#';

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
            for(i = 0;i < n; i++)
            {
                for(j = 0;j < m; j++)
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
                    if(army[j].x == i && army[j].y == leia.y)
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

    *moveset = NULL;

    while(1)
    {
        printf("Enter your choice: "); 
        len = read_text(str, 100, 1);

        *token = strtok(str, ">");
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

        if(choice == 'h' || choice == 'x')
        {
            return choice;
        }

        for(i = 0; i < len; i++)
        {
            if(isupper(str[i]))
            {
                str[i] = tolower(str[i]);
            }

            if(str[i] != 'u' && str[i] != 'd' && str[i] != 'l' && str[i] != 'r')
            {
                break;
            }
        }

        if(i == len || i == 0)
        {
            *moveset = (char *)malloc(len * sizeof(char));
            *moveset = str;
            *size = len;
            return 'm';
        }

        printf("Invalid choice. Please enter a valid move\n");
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
        puts("Invalid input. Please enter a valid move");
        return 1;
    }
    

    x1 = atoi(token + let1) - 1;
    if(x1 < 0 || x1 > n - 1)
    {
        puts("Invalid input. Please enter a valid move");
        return 1;
    }

    switch(let1)
    {
        case 1:
            y1 = token[0];
         break;
    
        case 2:
            y1 = token[1];
            switch(token[0])
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
        puts("Invalid input. Please enter a valid move");
        return 1;
    }

    x2 = atoi(token2 + let2) - 1;
    if(x2 < 0 || x2 > n - 1)
    {
        puts("Invalid input. Please enter a valid move");
        return 1;
    }

    switch(let2)
    {
        case 1:
            y2 = token2[0];
         break;
    
        case 2:
            y2 = token2[1];
            switch(token2[0])
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
        puts("Invalid input. Please enter a valid move");
        return 1;
    }

    if((*ship)[x2][y2] != '#' && (*ship)[x2][y2] != '.')
    {
        puts("Cant move object on the tile you want because someone or something is on the destination :(");
        return 1;
    }
    else if((*ship)[x1][y1] != 'X')
    {
        puts("You cant move an object that is not an obstacle");
        return 1;
    }

    for(i = 0; i < obstacles; i++)
    {
        if((*objects)[i].x == x1 && (*objects)[i].y == y1)
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

