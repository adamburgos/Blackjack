#include <stdio.h>

#include "hand.h"
#include "card.h"

Hand MakeHand() {
    Hand hand;
    hand.handSize = 0;
    return hand;
}

void AddCard(Hand *hand, Card c) {
    if (hand->handSize < HANDSIZE) {
        hand->cards[hand->handSize] = c;
        hand->handSize++;
    } else {
        printf("Hand is full, cannot add more cards.\n");
    }
}

void PrintHand(Hand hand){
    for(int i = 0; i<hand.handSize; i++){
        PrintCard(hand.cards[i]);
    }
    printf("\n");
}

int CalculatePoints(Hand *hand){
    int total = 0;
    for(int i = 0; i<hand->handSize; i++){
        total += GetCardPoints(hand->cards[i]);
    }
    return total;
}