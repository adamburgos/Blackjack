#ifndef HAND_H
#define HAND_H
#include "card.h"

#define HANDSIZE 12 //more than max cards we could have without a BUST

typedef struct Hand{
    Card cards[10];
    int handSize;
} Hand;

void AddCard(Hand *h, Card c);
void PrintHand(Hand hand);
Hand MakeHand();
int CalculatePoints(Hand *H);


#endif