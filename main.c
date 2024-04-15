#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "deck.h"
#include "card.h"
#include "hand.h"

typedef struct Game{
    Hand playerHand;
    Hand dealerHand;
    int playerChips;
    int bet;
    Deck deck;
}Game;

int RunGame(Game* r, int bet);
void SaveGameToFile(Game game);
Game LoadGameFromFile();

int main(int argc, char *argv[]){

    Game game;
    game.playerHand = MakeHand();
    game.dealerHand = MakeHand();
    game.playerChips = 1000;
    game.deck = MakeDeck();

    int loaded = argc > 1;

  while(game.playerChips > 0){
    
    if(loaded == 0){
        printf("\nYou have %d chips\n",game.playerChips);
        printf("How much would you like to bet?\n");
        scanf("%d",&game.bet);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { } //clear input buffer to avoid issues
        if(game.bet == 0){
            break; 
        }
        
        //deal cards
        AddCard(&game.playerHand, DrawCard(&game.deck));
        AddCard(&game.dealerHand, DrawCard(&game.deck));

        AddCard(&game.playerHand, DrawCard(&game.deck));
        AddCard(&game.dealerHand, DrawCard(&game.deck));
    }else{
        loaded = 0;
        game = LoadGameFromFile();
    }


    if(CalculatePoints(&game.playerHand) == 21){
        printf("You were dealt a 21! ");
        PrintHand(game.playerHand);
        game.playerChips += (int)(double)game.bet/2.0;
        continue;
    }

    game.playerChips += RunGame(&game, game.bet);
    game.playerHand = MakeHand();
    game.dealerHand = MakeHand();
    game.deck = MakeDeck();
  }

  printf("Game over.\n");
  printf("You left with %d chips.\n", game.playerChips);
}

//main game loop
int RunGame(Game* r, int bet){ 

    printf("\n"); 

    const char* suitName[] = {
        "HEARTS",
        "DIAMONDS",
        "CLUBS",
        "SPADES"
    };

    //get the player points, if greater than 21 print out u busted and return -bet
    int playerPoints = CalculatePoints(&r->playerHand);
    if (playerPoints > 21){
        printf("you busted ");
        return -bet;
    }

    //print out info on dealer hand and player hand
    printf("Dealer Score: %d + ???\n", r->dealerHand.cards[0].value);
    printf("Dealer Hand: %d of", r->dealerHand.cards[0].value);
    printf(" %s, ??? of ???\n", suitName[r->dealerHand.cards[0].suit]);
    

    printf("Player Score: %d\n", CalculatePoints(&r->playerHand));
    printf("Player Hand: ");
    PrintHand(r->playerHand);
    
    //prompt user to hit 'h' stay 's' or take a break 'b'
    char playerAction;
    printf("Will you hit 'h' or stay 's' or break 'b'?\n");

    //read character from terminal
    scanf("%c", &playerAction);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    while (playerAction != 'h' && playerAction != 's' && playerAction != 'b'){
        printf("input not recognized, try again\n");
        scanf("%c", &playerAction);
    
        while ((c = getchar()) != '\n' && c != EOF) { }
    }    

    //if 'h', add a card to player hand, recursive call
    if (playerAction == 'h'){
        AddCard(&r->playerHand, DrawCard(&r->deck));
        return RunGame(r,bet);
    }
    
    //if 'b', save game to file, exit program with exit(0);
    if (playerAction == 'b'){
        SaveGameToFile(*r); 
        exit(0);
    }

    //if 's'' resolve the game 
    if (playerAction == 's'){
        while (CalculatePoints(&r->dealerHand) <= 16){
            AddCard(&r->dealerHand, DrawCard(&r->deck));
        }
        int playerScore = CalculatePoints(&r->playerHand);
        int dealerScore = CalculatePoints(&r->dealerHand);
        
        if ((playerScore > 21 && dealerScore > 21) || (playerScore == dealerScore)){
            printf("\ntie\n");
            printf("your score: %d\n", playerScore);
            PrintHand(r->playerHand);
            printf("dealer score: %d\n",dealerScore);
            PrintHand(r->dealerHand);
            return 0; // tie
        }
        else if (playerScore > 21){
            printf("\nbust\n");
            printf("your score: %d\n", playerScore);
            PrintHand(r->playerHand);
            printf("dealer score: %d\n",dealerScore);
            PrintHand(r->dealerHand);
            return -bet; // loss
        }    
        else if (playerScore > dealerScore || dealerScore > 21){
            printf("\nwin\n");
            printf("your score: %d\n", playerScore);
            PrintHand(r->playerHand);
            printf("dealer score: %d\n",dealerScore);
            PrintHand(r->dealerHand);
            return bet*2; //win
        }
        else {
            printf("\nlost\n");
            printf("your score: %d\n", playerScore);
            PrintHand(r->playerHand);
            printf("dealer score: %d\n",dealerScore);
            PrintHand(r->dealerHand);
            return -bet; //loss
        }
    }

    return 0;
    
}

//Saving the game
void SaveCardToFile(Card card, FILE *file){
    fprintf(file, "%d %d\n", card.suit, card.value);
}

void SaveHandToFile(Hand hand, FILE *file){
    fprintf(file, "%d\n", hand.handSize);
    for (int i = 0; i < hand.handSize; i++){
        SaveCardToFile(hand.cards[i], file);
    }
}

void SaveDeckToFile(Deck deck, FILE *file){
    fprintf(file, "%d\n", deck.top);
    for (int i = 0; i < DECK_SIZE; i++){
        SaveCardToFile(deck.cards[i], file);
    }
}

void SaveGameToFile(Game game){
    FILE* saveFile = fopen("save.txt", "w");   
    if (saveFile != NULL){
        printf("saving game...\n");
        fprintf(saveFile, "%d\n", game.playerChips);
        fprintf(saveFile, "%d\n", game.bet);

        SaveHandToFile(game.playerHand, saveFile);
        SaveHandToFile(game.dealerHand, saveFile);
        SaveDeckToFile(game.deck, saveFile);

        fclose(saveFile);
        printf("game saved successfully\n");
    } else {
        printf("failed to save game");
    }   
}

//loading the game
Card LoadCardFromFile(FILE *file){
    Card card; 
    fscanf(file, "%d %d", &card.suit, &card.value);
    return card;
}

Hand LoadHandFromFile(FILE *file){
    Hand hand;
    fscanf(file, "%d", &hand.handSize);
    for (int i = 0; i < hand.handSize; i++){
        hand.cards[i] = LoadCardFromFile(file);
    }
    return hand;
}

Deck LoadDeckFromFile(FILE *file){
    Deck deck; 
    fscanf(file, "%d", &deck.top);
    for (int i = 0; i < DECK_SIZE; i++){
        deck.cards[i] = LoadCardFromFile(file);
    }
    return deck; 
}

Game LoadGameFromFile(){ 
    FILE *loadFile = fopen("save.txt", "r");
    Game game = {0};
    if (loadFile != NULL){
        fscanf(loadFile, "%d", &game.playerChips);
        fscanf(loadFile, "%d", &game.bet);

        game.playerHand = LoadHandFromFile(loadFile);
        game.dealerHand = LoadHandFromFile(loadFile);
        game.deck = LoadDeckFromFile(loadFile);

        fclose(loadFile);
        printf("game loaded successfully\n");
    } else {
        printf("failed to load game\n");
    }
    return game; 
}
