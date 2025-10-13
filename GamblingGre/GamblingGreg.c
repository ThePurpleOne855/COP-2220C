#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct {
    char name[10];
    int payoutMax;
    int payoutMin;
    float oddOfWinning;
} Dog;


typedef struct {
    int moneyBet;
    char dogBetOn[10];
    int moneyWon;
    int startBalance;
    int finalBalance;
} GameDetails;


void gamble(Dog dogList[], int *ptrToBalance, int gameNumber, int *ptrBalance, GameDetails *ptrDetails, int *ptrGameTracker);

void banking(int *ptrBalanceBet, int *ptrBalancePersonalAccount);

void raceResult(GameDetails *ptrDetails, int VarGameTracker);

int main(){
    
    srand(time(0));

    Dog racingDogs[9] = {
        {"Coco", 2, 1, 0.40},
        {"Banana", 5, 1, 0.10},
        {"Lemonade", 10, 1, 0.08},
        {"Wendy", 15, 1, 0.06},
        {"Tarzan", 50, 1, 0.01},
        {"MaxSteel", 20, 1, 0.04},
        {"Goku", 10, 1, 0.08},
        {"Vegeta", 5, 1, 0.10},
        {"Mike", 3, 1, 0.13},
    };
    GameDetails record[10];
    int gameTracker = 0;
    int gregBalance = 50;
    GameDetails details[10];
    int gregPersonalBankAccountBalance = 100;

    int on = 1;
    


    while(on == 1){
        char menuSelection;
        printf("=======================\n=======================\n");
        printf("[G]amble\n");
        printf("[B]anking\n");
        printf("[R]esult of each race that Greg wagered\n");
        printf("[L]eave the dog track\n");

        scanf(" %c", &menuSelection);

        switch (menuSelection){
            case 'G':
            case 'g':
                gamble(racingDogs, &gregBalance, gameTracker, &gregBalance, details, &gameTracker);
                gameTracker += 1;
                break;

            case 'B':
            case 'b':
                banking(&gregBalance, &gregPersonalBankAccountBalance);
                break;

            case 'R':
            case 'r':
                raceResult(details, gameTracker);
                break;

            case 'L':
            case 'l':
                on = 0;
                break;
        }

        if(gameTracker == 10){
            printf("Game Limit has been reached... You must left the dog track.");
            return 0;
        }
    };

    return 0;
}



void gamble(Dog dogList[], int *ptrToBalance, int gameNumber, int *ptrBalance, GameDetails *ptrDetails, int *ptrGameTracker)
{   

    int gregStartingBalance = *ptrBalance;
    int gregDogSelection;
    int gregBet;
    int moneyWon = 0;

    for(int i = 0; i < 9; i++){
        printf("%d) This is %s, Payout: %d to %d, Winning Chances: %.2f %%\n", i + 1 , dogList[i].name, dogList[i].payoutMin, dogList[i].payoutMax, (dogList[i].oddOfWinning * 100));
        
    }
    printf("\n");

    printf("From 1 to 9, Place your bet on one of these racing dogs\n");


    scanf("%d", &gregDogSelection);


    if(gregDogSelection < 1 || gregDogSelection > 9){
        do{
            printf("Sorry, That is not a valid input. Input a number between 1 and 9:\n");
            scanf("%d", &gregDogSelection);
        } while (gregDogSelection < 1 || gregDogSelection > 9);
    }
    printf("Your balance is: %d\n", *ptrBalance);

    printf("How much do you want to bet on %s?\n", dogList[gregDogSelection - 1].name);

    scanf("%d", &gregBet);

    if(gregBet <= 0){
        printf("Your bet cannot be a value that is less than zero\n");

        do {
            scanf("%d", &gregBet);
        } while (gregBet <= 0);
    }

    if(gregBet > *ptrBalance){
        printf("Your bet is greater than your current balance. Balance: %d\n", *ptrBalance);

        do{
            scanf("%d", &gregBet);
        } while (gregBet > *ptrBalance);
    }

    *ptrBalance = *ptrBalance - gregBet;


    gregDogSelection = gregDogSelection - 1;
    
    printf("You selected %s, Payout: %d to %d, Winning Chances: %.2f%%\n\n", dogList[gregDogSelection].name,
                                                                              dogList[gregDogSelection].payoutMin,
                                                                              dogList[gregDogSelection].payoutMax,
                                                                              dogList[gregDogSelection].oddOfWinning * 100);

    int payOutRatio = (rand() % dogList[gregDogSelection].payoutMax) + dogList[gregDogSelection].payoutMin;
    printf("=======================\n=======================\n");
    printf("Racing...\n");
    int racingDogWinner = (rand() % 9);
    printf("The winner is...\n");
    

    if (racingDogWinner == gregDogSelection) {
        printf("%s is the winner!\n", dogList[gregDogSelection].name);
    
        int winnings = gregBet * (payOutRatio - 1);
        moneyWon = winnings;
    
        *ptrBalance = *ptrBalance + gregBet + winnings;
    
        printf("You won $%d! (Bet: $%d, Payout: %d:1)\n", winnings, gregBet, payOutRatio);
    } else {
        printf("The winner is %s!\n", dogList[racingDogWinner].name);
    
        moneyWon = -gregBet;
    
        printf("You lost $%d\n", gregBet);
    }

    printf("New balance: $%d\n", *ptrBalance);

    printf("=======================\n=======================\n");

    
    ptrDetails[*ptrGameTracker].moneyBet = gregBet;
    strcpy(ptrDetails[*ptrGameTracker].dogBetOn, dogList[gregDogSelection].name);
    ptrDetails[*ptrGameTracker].moneyWon = moneyWon;
    ptrDetails[*ptrGameTracker].startBalance = gregStartingBalance;
    ptrDetails[*ptrGameTracker].finalBalance = *ptrBalance;


}

void banking(int *ptrBalanceBet, int *ptrBalancePersonalAccount)
{   
    int menuSelection;
    printf("Current Balance at Gambling Account: %d\n", *ptrBalanceBet);
    printf("Current Balance at Personal Account: %d\n", *ptrBalancePersonalAccount);
    printf("1) Withdraw\n2) Deposit\n");

    scanf("%d", &menuSelection);

    int withdrawal_deposit_amount;


    if(menuSelection == 1){
        printf("How much do you want to withdraw?\n");
        scanf("%d", &withdrawal_deposit_amount);
        if (withdrawal_deposit_amount > *ptrBalanceBet){
            printf("You cannot withdraw more than your balance. Current Balance at Gamble Account: %d\n", *ptrBalanceBet);
            return;
        }
        *ptrBalancePersonalAccount = *ptrBalancePersonalAccount + withdrawal_deposit_amount;
        *ptrBalanceBet = *ptrBalanceBet - withdrawal_deposit_amount;
    } else if(menuSelection == 2){
        printf("How much do you want to deposit?\n");
        scanf("%d", &withdrawal_deposit_amount);

        if(withdrawal_deposit_amount > *ptrBalancePersonalAccount){
            printf("You cannot deposit more than your balance. Current Personal Account Balance: %d\n", *ptrBalancePersonalAccount);
            return;
        }

        *ptrBalanceBet += withdrawal_deposit_amount;
        *ptrBalancePersonalAccount -= withdrawal_deposit_amount;
    }

}

void raceResult(GameDetails *ptrDetails, int VarGameTracker)
{
     if (VarGameTracker == 0) {
        printf("\n=======================\n");
        printf("No races have been played yet!\n");
        printf("=======================\n\n");
        return;
    }

    printf("\n=======================\n");
    printf("RACE RESULTS SUMMARY\n");
    printf("=======================\n\n");

    for (int i = 0; i < VarGameTracker; i++) {
        printf("Race #%d:\n", i + 1);
        printf("  Dog Bet On: %s\n", ptrDetails[i].dogBetOn);
        printf("  Money Bet: $%d\n", ptrDetails[i].moneyBet);
        printf("  Money Won/Lost: $%d\n", ptrDetails[i].moneyWon);
        printf("  Starting Balance: $%d\n", ptrDetails[i].startBalance);
        printf("  Final Balance: $%d\n", ptrDetails[i].finalBalance);
        
        if (ptrDetails[i].moneyWon > 0) {
            printf("  Result: WON!\n");
        } else {
            printf("  Result: LOST\n");
        }
        printf("-----------------------\n");
    }

    printf("\n=======================\n\n");
}