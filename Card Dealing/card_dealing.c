#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void dealCards (char *deck[], char *arrPlayer1[], char *arrPlayer2[], char *arrPlayer3[]);
void shuffleArray(char *arr[], int n);
void showDeals(char *arrPlayer1Hand[], char *arrPlayer2Hand[], char *arrPlayer3Hand[], int size);

int main(){
    char *suits[] = {"Clubs", "Hearts", "Spades", "Diamonds"}; // Creates the suits
    char *ranks[] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"}; // Creates the ranks

    char *deck[52];
    int index = 0;


    int length_of_suits_array = sizeof(suits) / sizeof(suits[0]);
    int length_of_ranks_array = sizeof(ranks) / sizeof(ranks[0]);
    for(int x = 0; x < length_of_suits_array; x++){ // Creates the deck by combining suits and ranks
        for(int y = 0; y < length_of_ranks_array; y++){
            char combined[70];

            strcpy(combined, suits[x]);
            strcat(combined, " Of ");
            strcat(combined, ranks[y]);

            deck[index] = strdup(combined); // Creates a duplicate of combined and appends it to the current index
            index++;
        }
    }


    int n = sizeof(deck) / sizeof(deck[0]);

    shuffleArray(deck, n);

    char *player1_hand[5];
    char *palyer2_hand[5];
    char *player3_hand[5];


    dealCards(deck, player1_hand, palyer2_hand, player3_hand);

    int size = sizeof(player1_hand) / sizeof(palyer2_hand[0]);

    showDeals(player1_hand, palyer2_hand, player3_hand, size);
}

void dealCards(char *deck[], char *arrPlayer1[], char *arrPlayer2[], char *arrPlayer3[])
{
    int size = 52; // Will be used to move from the end of the array to the beginning of it.
    int i = 0;
    for(int number_of_deals_for_player_1 = 5; number_of_deals_for_player_1 != 0; number_of_deals_for_player_1--){
        char *popped = deck[size -1];
        arrPlayer1[i] = popped;
        size--;
        i++;
        
    }
    i = 0;

    for(int number_of_deals_for_player_2 = 5; number_of_deals_for_player_2 != 0; number_of_deals_for_player_2--){
        char *popped = deck[size - 1];
        arrPlayer2[i] = popped;
        size--;
        i++;
    }
    i = 0;

    for(int number_of_deals_for_player_3 = 5; number_of_deals_for_player_3 != 0; number_of_deals_for_player_3--){
        char *popped = deck[size - 1];
        arrPlayer3[i] = popped;
        size--;
        i++;
    }

}


void shuffleArray(char *arr[], int n)
{
    srand(time(NULL)); // Applied Fisher-Yates shuffling algorithm

    for (int i = n - 1; i > 0; i--){
        int j = rand() % (i + 1);
        

        char *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void showDeals(char *arrPlayer1Hand[], char *arrPlayer2Hand[], char *arrPlayer3Hand[], int size) // Prints Players Hand
{

    printf("Player 1 Hand:\n");


    for(int i = 0; i < size; i++){
        printf("%s\n", arrPlayer1Hand[i]);
    }
    printf("\n\n\n");

    printf("Player 2 Hand:\n");

    for(int i = 0; i < size; i++){
        printf("%s\n", arrPlayer2Hand[i]);
    }
    printf("\n\n\n");

    printf("Player 3 Hand:\n");

    for(int i = 0; i < size; i++){
        printf("%s\n", arrPlayer3Hand[i]);
    }

}
