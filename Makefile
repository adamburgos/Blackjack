blackjack: main.c card.c hand.c deck.c     
	gcc -o blackjack.out main.c card.c hand.c deck.c

clean:     
	rm *.out *.txt