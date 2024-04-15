#include "deck.h"
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()
#include <stdio.h>

Deck MakeDeck() {
    Deck deck;
    deck.top = 0;

    for (int i = 0; i < 13; i++) {
        deck.cards[i] = MakeCard(i+1, HEARTS);
    }
    for (int i = 0; i < 13; i++) {
        deck.cards[i+13] = MakeCard(i+1, SPADES);
    }
    for (int i = 0; i < 13; i++) {
        deck.cards[i+26] = MakeCard(i+1, CLUBS);
    }
    for (int i = 0; i < 13; i++) {
        deck.cards[i+39] = MakeCard(i+1, DIAMONDS);
    }
    
    ShuffleDeck(&deck);
    return deck;
}

void ShuffleDeck(Deck* deck) {
    srand(time(NULL));
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card DrawCard(Deck* deck) {
    if (deck->top >= DECK_SIZE) {
        // Handle error: deck is empty
        return (Card){-1}; 
    }
    Card drawnCard = deck->cards[deck->top];
    deck->top += 1;
    return drawnCard;
}
