#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_PLAYERS 5

// defining my struct so that it can have an array of chars (for the name) and the number of guesses
typedef struct Players
{
    char name[100];
    int guesses;
}player;

/*
This is just a linear sorting algorithm
*/
void sortList(player list[], int user){
    for(int i = 0; i < user - 1; i++){
        for(int j = i + 1; j < user; j++){
            if(list[j].guesses < list[i].guesses){
                player temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}

/*
This is my basic loop function all it does is request the name, and store it into the list
tehn it gets the amount of guesses to get a random sqrt value and stores it in the list, 
and then it gets ready to printout all the values in the list, and it increments the user so that we can
make sure there isn't more than 5 people in the list/file at one time
*/
void PlayGame(player list[], int *user) {
    printf("Please enter your name to start: ");
    scanf(" %99s", list[*user].name);

    int randVal = rand() % (100 - 10) + 10;
    int guess = -1;
    int num_tries = 0;

    printf("%f is the square root of what number?\n", sqrt(randVal));

    do {
        printf("Guess a value between 10 and 100: ");
        scanf("%d", &guess);
        num_tries++;

        if(guess == randVal){
            printf("You got it, baby!\n");
            printf("You made %d guesses.\n", num_tries);
            list[*user].guesses = num_tries;
        } else if(guess > randVal){
            printf("Too High, guess again:\n");
        } else {
            printf("Too Low, guess again:\n");
        }
    } while(guess != randVal);

    (*user)++;   

    printf("Here are the current leaders:\n");
    sortList(list, *user);
}

/*
This is my code to write my list to the file. we open the file and make sure that there is actually something in it.
Then we make sure that we arent running over the max number of people (5) using the terniary operator. Then we are writing to the file
in this format:
Josh 5
Jack 6 ...
Then we set user to the MAX_PLAYER only if it is greater, then we make sure that user is always <= MAX_PLAYER for the rest of the code
*/
void writeToFile(player list[], int* user){
    FILE* addToLeaderBoard = fopen("scores.txt", "w");
    if (!addToLeaderBoard) {
        perror("Error opening file");
        return;
    }

    sortList(list, *user);

    int limit = (*user < MAX_PLAYERS) ? *user : MAX_PLAYERS;
    for (int i = 0; i < limit; i++) {
        fprintf(addToLeaderBoard, "%s %d\n", list[i].name, list[i].guesses);
    }

    fclose(addToLeaderBoard);

    if (*user > MAX_PLAYERS) {
        *user = MAX_PLAYERS;
    }
}

/*
This just reads through the file and then prints out all the values. The while statement works by making sure that when we are
reading through the file, we only get 2 values, if we get more then it breaks out of the while loop and we are done because the only way
we are writing to the file is in that format
*/
void readFileAndPrintFile(player list[]){
    FILE* currentLeaders = fopen("scores.txt", "r");
    int i = 0;
    while (i < MAX_PLAYERS && fscanf(currentLeaders, "%s %d", list[i].name, &list[i].guesses) == 2) {
        i++;
    }
    fclose(currentLeaders);

    for(int j = 0; j < i; j++){
        printf("%s made %d guesses\n", list[j].name, list[j].guesses);
    }
}

/*
This is the main function, where we call all of our functions, the first thing that we do is we check the file,
if there is something in the file, we go into the if statement. Then we record all the values to the list array of players
Then we start into our while loop, which starts our game
*/
int main(){
    char quit;
    player list[MAX_PLAYERS];
    FILE* currentLeaderBoard = fopen("scores.txt", "r");
    int user = 0;
    if(currentLeaderBoard != NULL){    
        while (user < MAX_PLAYERS && fscanf(currentLeaderBoard, "%s %d", list[user].name, &list[user].guesses) == 2) {
            user++;
        }
        fclose(currentLeaderBoard);
    }

    printf("Welcome! Press 'q' to quit or any other key to continue: ");
    scanf(" %c", &quit);

    while(quit != 'q'){
        PlayGame(list, &user);
        writeToFile(list, &user);
        readFileAndPrintFile(list);
        printf("Press 'q' to quit or any other key to continue: ");
        scanf(" %c", &quit);
    }

    printf("Bye Bye!\n");
    return 0;
}
