/*
*
*
Hangman game created by Roelof Willems.
This for study purpose only. I'm sure there are better codes out there :-)
If you like it, feel free to let me know!
github.com/r-willems
linkedin.com/in/roelof-willems-047b87117
https://www.instagram.com/rudiwillems/
*
*
*/


#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 64
#define M 16

char* get_word(void);
char* ask_input(void);
void print_word(char*, char*);
void print_art(int);
int count_unique_char(char*);

char player = 'A';
char *word = NULL;

int main(void)
{
    system("clear");
    printf("\n       # HANGMAN #       \n");
    printf("Welcome to Hangman.\nPlease select opponent:\nPress (1) for Computer | Press (2) for Player\n");

    //We ask the user(s) for a choice to play against either the computer or another player.
    int select;
    scanf(" %1d", &select);
    if (select == 1)
    {
        word = get_word();
        if (word == NULL)
        {
            return 1;
        }
    }
    else if (select == 2)
    {
        word = ask_input();
        player++;
    }
    else
    {
        printf("Invalid input.");
        return 1;
    }
    system("clear");

        /*
    Letters only need to be guessed correctly once, so we set an integer, pass it the number of unique
    characters, and decrement it with every correct guess.
        */
    int unique = count_unique_char(word);

    int i = 0;
    char tries[MAX] = {0};
    char guess = 0;
    int lives = 6;
    bool guessed = false;

    while (lives != 0 && guessed == false)
    {
        print_word(word, tries);
        printf("You have %i lives left\n", lives);

        print_art(lives);
        printf("       # HANGMAN #       \n");
        if (tries)
        {
            printf("Previously guessed: %s\n", tries); 
        }
        else
        {
            printf("\n");
        }

        //Ask the player for a character
        do
        {
            printf("Player %c, guess a character: ... \n", player);
            scanf(" %c", &guess);
        } while (!isalpha(guess));
        guess = toupper(guess);

        system("clear");

        //Check if a letter has already been guessed, if so, subtract a life and skip to next iteriation.
        if (strchr(tries, guess) != NULL)
        {
            lives--;
            printf("The letter %c has already been tried!\n", guess);
            
            continue;
        }

        //Check for a correct guess
        if (strchr(word, guess) != NULL)
        {
            printf("The letter %c is right!\n", guess);
            unique--;
        }
        else
        {
            printf("Too bad!\n");
            lives--;
        }
        //Assign the guessed character to the array <tries>, so we save the previously guessed characters
        tries[i] = guess;
        i++;

        //If unique is down to zero, we know all characters have been guessed and we set the boolean value to true.
        if (unique == 0)
        {
            guessed = true;
        }

    }

    //If the lives have run out, we print the message that the player has lost.
    if (lives == 0)
    {
        system("clear");

        printf("\n    ! H A N G M A N !    \n");
        print_art(lives);
        printf("You have been hanged. The word was \n'%s'\n\n", word);
    }

    //Message that the player has won.
    if (guessed == true)
    {
        printf("X#X#X#X#X#X#X#X#X#X#X#X#X");
        system("clear");
        printf("\n     !   HANGMAN   !     \n");
        print_art(lives);
        printf("Congratulations, the word was \n'%s'\n\n", word);
    }

    free(word);

    return 0;
}

char* get_word(void)
{
    //Get a random word from file words.txt
    word = malloc(sizeof(char)*MAX);
    if (!word)
    {
        printf("Error in memory");
        return NULL;
    }

    FILE *input = fopen("words.txt", "r");
    if (!input)
    {
        printf("File not found!\n");
        free(word);
        return NULL;
    }
 
    srand(time(0));
    fseek(input, 0-(sizeof(char)*M), SEEK_END);
    int r = rand() % ftell(input);

    fseek(input, 0+r, SEEK_SET);   
    char c;
    do
    {
        fseek(input, 0, SEEK_CUR);  
    } while ((c = getc(input)) != '\n'); 

    int i; char d; 
    for (i = 0; (d = fgetc(input)) != '\n'; i++)
    {
        word[i] = toupper(d); 
    }
    word[i] = '\0';

    fclose(input);
    
    return word;
}

char* ask_input(void)
{
    word = malloc(sizeof(char)*MAX);
    if (!word)
    {
        printf("Error in memory");
        return 0;
    }

    printf("Player %c enter a word: ...\n", player);
    scanf(" %s", word);
    word[strlen(word)] = '\0';
    for (int i = 0; word[i] != '\0'; i++)
    {   
        if (!isalpha(word[i]))
        {   
            printf("Input error. Only use alphabetic characters!\n");
            free(word);
            ask_input();
        }
        word[i] = toupper(word[i]);
    }

    return word;
} 

int count_unique_char(char* str)
{
        /*
    Iteriate through the string and use the ASCII value of each to get an index number, ex 'A' is 65.
    Then set value of hash at that index to 1. On repetion, it will just set the value to 1 again, so
    eventually hash will be an array of 1's for letters that have occured, and empty values. Lastly we
    iteriathe through that array to pass each 1 to a counter, resulting in an int of the total of 
    unique characters
        */
    int hash[128] = { 0 };
    int i, count = 0, l = strlen(str);

    for (i = 0; i < l; i++)
    {
        int idx = str[i];
        hash[idx] = 1;
    }

    for (i = 0; i < 128; i++)
    {
        count += hash[i];
    }

    return count;
}

void print_word(char *word, char *tries)
{
    //Print the word, show underscore if letter has not been guessed yet and reveal the letter once it has been correctly guessed
    printf("Your word is: ");
    for (int i = 0; i < strlen(word); i++)
    {
        if (strchr(tries, word[i]) != NULL)
        {
            printf("%c", word[i]);
        }
        else
        {
            printf("_");
        }
        printf(" ");
    }
    printf("\n");
}

void print_art(int l)
{
    //ASCII-art to print the actual Hangman
    printf(":::::::::::::::::::::\n");
    switch (l)
    {
        case 5:
            printf(":::::::...o...:::::::\n");
            printf(":::::::.......:::::::\n");
            printf(":::::::.......:::::::\n");
            break;
        case 4:
            printf(":::::::.\\.o...:::::::\n");
            printf(":::::::.......:::::::\n");
            printf(":::::::.......:::::::\n");
            break;
        case 3:
            printf(":::::::.\\.o./.:::::::\n");
            printf(":::::::.......:::::::\n");
            printf(":::::::.......:::::::\n");
            break;
        case 2:
            printf(":::::::.\\.o./.:::::::\n");
            printf(":::::::...|...:::::::\n");
            printf(":::::::.......:::::::\n");
            break;
        case 1:
            printf(":::::::.\\.o./.:::::::\n");
            printf(":::::::...|...:::::::\n");
            printf(":::::::./.....:::::::\n");
            break;
        case 0:
            printf("!:::::X.\\.o./.X:::::!\n");
            printf("!:::::X...|...X:::::!\n");
            printf("!:::::X../.\\..X:::::!\n");
            break;
        default:
            printf(":::::::.......:::::::\n");
            printf(":::::::.......:::::::\n");
            printf(":::::::.......:::::::\n");
    }
    printf(":::::::.......:::::::\n");
}