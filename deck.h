#ifndef DECK_H
#define DECK_H

#include "card.h"

#define DECK_SIZE 52

typedef struct Deck {
    Card cards[DECK_SIZE];
    int top; // Index of the top card
} Deck;

Deck MakeDeck();
void ShuffleDeck(Deck* deck);
Card DrawCard(Deck* deck);


#endif