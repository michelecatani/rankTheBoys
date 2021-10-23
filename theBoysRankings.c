#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Custom Data Types

typedef enum STATUS {
    p1, p2, ongoing
} STATUS;

// make a struct player

typedef struct player {
    char username[30];
    int score;
    int games;
    int wins; 
    int losses;
    double rating;
    int id;
} PLAYER;

// game struct

typedef struct game {
    int id;
    PLAYER * player1;
    PLAYER * player2;
    STATUS status;
} GAME;

// function to create a player

int createPlayer(PLAYER playerList[], int *lastPlayerID);

// function to create a game

void createGame(PLAYER playerList[], int playerListSize, GAME gameList[], int *lastGameID);

// print the players 

void printPlayers(PLAYER playerList[], int playerListSize);

void leaderBoard(PLAYER playerList[], int playerListSize);

// check name 

int validUsername(char *username);

// check if an ID is in the list 

int inList(PLAYER playerList[], int ID, int playerListSize);

// calculate player rating

void calculateEloAmount(PLAYER * winner, PLAYER * loser);

int main(void) {

    printf("%s", "\n**********\nWELCOME\n**********\n\n");

    int playerListSize = 5;
    int gameListSize = 10;
    int lastGameID = 0;
    int lastPlayerID = 0;

    // initialize playerList and gameList arrays... make them pointers so we can do DMA

    PLAYER *playerList = (PLAYER*) malloc(playerListSize*sizeof(PLAYER));
    GAME *gameList = (GAME*) malloc(gameListSize*sizeof(GAME));

    for(int i = 0; i < playerListSize; i++) {
        PLAYER newPlayer;
        newPlayer.id = 0;
        playerList[i] = newPlayer;
    }

    // give all values in gameList an id of -1... will be useful in other functions...

    for(int i = 0; i < gameListSize; i++) {
        GAME newGame;
        newGame.id = 0;
        gameList[i] = newGame;
    }

    while (1) {

        // list options

        puts("P to add a new player");
        puts("L to see the leaderboard");
        puts("G to start a new game");
        puts("M to look at old games");
        puts("C to see the list of players");
        puts("Q to quit");

        char input;

        // flush input 

        fflush(stdin);

        while(scanf(" %c", &input) != 1 || (input != 'P' && input != 'L' && input != 'G' && input != 'M' && input != 'C')){
            
            // if input is 'Q' then we quit

            if (input == 'Q'){
                puts("\nYou have quit.\n");
                return 0;
            }

            fflush(stdin);

            puts("This input is invalid.  Please enter one of the options listed above (case sensitive)");
        }

        if (input == 'P'){
            // add the function and update the lastPlayerID...
            lastPlayerID = createPlayer(playerList, &lastPlayerID);
        } else if (input == 'L') {
            leaderBoard(playerList, playerListSize);
        } else if (input == 'G'){
            createGame(playerList, playerListSize, gameList, &lastGameID);
        } else if (input == 'M'){
            // TODO make a history function
        } else if (input == 'C'){
            printPlayers(playerList, playerListSize);
        }
    }

}

// function to create a player

int createPlayer(PLAYER playerList[], int *lastPlayerID) {

    // TODO pass lastPlayerID correctly.

    PLAYER newPlayer;

    fflush(stdin);

    printf("%s", "\nHello player! Select your username. (25 char max)\n");
    while(scanf("%25s", newPlayer.username) != 1 || validUsername(newPlayer.username) != 1){
        fflush(stdin);
        puts("Please enter a valid username");
    }

    fflush(stdin);

    // set other members 

    newPlayer.games = 0;
    newPlayer.losses = 0;
    newPlayer.rating = 1000;
    newPlayer.wins = 0;
    newPlayer.id = *lastPlayerID + 1;

    printf("\nAlright, %s is now a player!  Base rating is 1000.\n\n", newPlayer.username);
    playerList[*lastPlayerID] = newPlayer;
    return ++*lastPlayerID;

    // TODO dma so that the array increases... 
}

// function to create a game

void createGame(PLAYER playerList[], int playerListSize, GAME gameList[], int *lastGameID) {

    // TODO pass lastGameID correctly

    GAME newGame;

    fflush(stdin);

    newGame.id = *lastGameID;

    int player1ID, player2ID;
    PLAYER * player1;
    PLAYER * player2;

    // print the players to the console so we can see who to pick...
    printPlayers(playerList, playerListSize);

    // flush the input

    fflush(stdin);

    // check if it is in the list
    // ENSURE BOTH OF THESE ARENT 0, BECAUSE THOSE TECHNICALLY EXIST IN THE ARRAY.

    puts("Please select the first player (enter their ID, players are shown above ^):");
    while(scanf("%d", &player1ID) != 1 || inList(playerList, player1ID, playerListSize) == 0 || player1ID == 0){
        fflush(stdin);
        puts("Input must be an integer and must be a player ID from the list.");
    }

    // check if p2 id is in the list

    puts("Please select the second player (enter their ID, players are shown above ^):");
    while(scanf("%d", &player2ID) != 1 || inList(playerList, player2ID, playerListSize) == 0 || player2ID == player1ID || player2ID == 0){
        fflush(stdin);
        puts("Input must be an integer and must be a player ID from the list (ensure it's not the same as p1).");
    }

    for(int i = 0; i < playerListSize; i++){
        if (playerList[i].id == player1ID){
            player1 = &playerList[i];
        } else if (playerList[i].id == player2ID){
            player2 = &playerList[i];
        }
    }

    puts("game ongoing...");
    puts("Has somebody won? Enter their ID or 0 to exit and save:");

    int input;

    while(scanf("%d", &input) != 1 || (input != player1ID && input != player2ID)){
        if (input == 0){
            // TODO save the game... 
            puts("Game saved!");
            return;
        }
        fflush(stdin);
        puts("Input must be an integer.  Please let me know which ID won or save the game");
    }

    if (input == player1ID){
        printf("%s has won!\n\n", player1->username);
        // TODO ensure that player1 wins and everything that is supposed to happen happens...
        calculateEloAmount(player1, player2);
        return;
    } else if (input == player2ID){
        printf("%s has won!\n\n", player2->username);
        // TODO ensure that player2 winds and everything that is supposed to happen happens....
        calculateEloAmount(player2, player1);
        return;
    }

    // TODO FINISH THIS FUNCTION 



}

// print the players 

void printPlayers(PLAYER playerList[], int playerListSize) {
    // make a count variable to count positive ids.

    printf("%s", "\n********************\nPLAYERLIST\n********************\n");

    int count = 0;

    for(int i = 0; i < playerListSize; i++){
        if(playerList[i].id > 0){
            printf("\n%s:\n", playerList[i].username);
            printf("Games played: %d\n", playerList[i].games);
            printf("Games won: %d\n", playerList[i].wins);
            printf("Games lost: %d\n", playerList[i].losses);
            printf("RATING: %f\n", playerList[i].rating);
            printf("ID: %d\n", playerList[i].id);
            count++;
        }
    }

    // if count = 0, have to print something to let them know playerlist is empty

    if (count == 0) {
        printf("%s", "\nYou haven't created any players yet!\n\n");
    }
}

void leaderBoard(PLAYER playerList[], int playerListSize){
    return;
}

// check name 

int validUsername(char *username){
    return 1;
}

int inList(PLAYER playerList[], int ID, int playerListSize){

    // if the ID is in the list, return 1

    for(int i = 0; i < playerListSize; i++){
        if(ID == playerList[i].id){
            return 1;
            break;
        }
    }

    // if not, return 0

    return 0;
}

void calculateEloAmount(PLAYER * winner, PLAYER * loser){

    double amount;
    winner->wins++;
    winner->games++;
    loser->losses++;
    loser->games++;

    // initialize K-factor

    double kFactor = 32;
    double winnerExpectedScore = 1 / (1 + pow(10, (loser->rating - winner->rating)/400));
    double loserExpectedScore = 1 / (1 + pow(10, (winner->rating - loser->rating)/400));

    printf("Winner expected score %f\n", winnerExpectedScore);
    printf("Loser expected score %f\n", loserExpectedScore);

    // TODO this has to be actual elo.... 
    
    winner->rating = winner->rating + kFactor*(1 - winnerExpectedScore);
    loser->rating = loser->rating + kFactor*(0 - loserExpectedScore);
}