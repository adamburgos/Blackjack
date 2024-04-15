#include "card.h"
#include <stdio.h>

void PrintCard(Card c) {

    switch (c.value) {
        case 1:
            printf("Ace of ");
            break;
        case 11:
            printf("Jack of ");
            break;
        case 12:
            printf("Queen of ");
            break;
        case 13:
            printf("King of ");
            break;
        default:
            printf("%d of ", c.value);
            break;
    }


    switch (c.suit) {
        case HEARTS:
            printf("Hearts, ");
            break;
        case DIAMONDS:
            printf("Diamonds, ");
            break;
        case CLUBS:
            printf("Clubs, ");
            break;
        case SPADES:
            printf("Spades, ");
            break;
        default:
            printf("Unknown, ");
            break;
    }

    
}

int GetCardPoints(Card c) {
    switch (c.value) {
        case 11: // Jack
        case 12: // Queen
        case 13: // King
            return 10;
        case 1: // Ace
            return 11; // For simplicity, we'll consider Ace as 11 points
        default:
            return c.value;
    }
}

Card MakeCard(int rank, enum Suit suit) {
    Card newCard;
    newCard.value = rank;
    newCard.suit = suit;
    return newCard;
}