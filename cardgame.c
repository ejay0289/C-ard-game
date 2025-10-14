#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PLAYER_COUNT 4
#define DECK_SIZE 52
#define MAX_HAND_SIZE 4

struct Card {
	int value;
	char suite;
};

struct Player{
	unsigned int player_id;
	struct Card player_hand[4];
    unsigned int hand_size;
};


const char suites[4] = {'H','C','D','S'};
const int values[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
char display_value[] = {'A','J','Q','K','N'};
struct Card *Card_create(int value,char suite,char display_value){
	struct Card *card = malloc(sizeof(*card));
	assert(card != NULL);

	card->value = value;
	card->suite = suite;
	//card->display_value = display_value;

	return card;
}


struct Player *player_container[MAX_PLAYER_COUNT] = {NULL,NULL,NULL,NULL};
int active_players = 0;
void create_player(){
	struct Player *player = malloc(sizeof(struct Player));
	assert(player != NULL);

	for (int i = 0; i < 4; i++) {
		player->player_hand[i] = (struct Card){0};
	}

	for(int i = 0; i<MAX_PLAYER_COUNT;i++){
	if(player_container[i] == NULL ){

		player->player_id = i+1;
		player_container[i] = player;
		break;		
		}
	}	

    active_players++;
}

void print_players(){
for(int i = 0;i<MAX_PLAYER_COUNT;i++){
	if(player_container[i] != NULL){
	printf("%d\t",player_container[i]->player_id);
		}
	}
printf("\n");
}

void print_hand(struct Player *player){
	printf("Player %d hand: ",player->player_id);
	for(int i = 0; i<MAX_PLAYER_COUNT;i++){
	printf("%d%c  ",player->player_hand[i].value,player->player_hand[i].suite);
	}
	printf("\n");

}

struct Deck{
	struct Card cards_in_deck[DECK_SIZE];
	int num_cards;
    struct Card top_card;
};

void shuffle_deck(struct Deck *deck){
	srand(time(NULL));//seed random number generator
	for(int i = deck->num_cards -1;i>0;i--){
		int j = rand() % (i+1);
		//The Fisher-Yates shuffle
		struct Card temp = deck->cards_in_deck[i];
		deck->cards_in_deck[i] = deck->cards_in_deck[j];
		deck->cards_in_deck[j] = temp;
	}
    deck->top_card = deck->cards_in_deck[deck->num_cards - 1];
}

void deal_cards(struct Deck *deck){
	if(deck->num_cards == 0){
    	printf("There are no cards left");
    	return;
	}

	for(int i = 0;i<active_players;i++){
    		if(player_container[i] == NULL){
	    	continue;	
		    }
	
	    for(int j = 0;j<3;j++){

		    if(deck->num_cards >= 3){
		    player_container[i]->player_hand[j] = deck->cards_in_deck[deck->num_cards-1-i];
		    deck->num_cards--;
		    deck->top_card = deck->cards_in_deck[deck->num_cards-j];
            player_container[i]->hand_size++;
	        }
	    }
	}

}

void player_draw_card(struct Player *player,struct Deck *deck){
    if(player->hand_size == 3){
    	struct Card top_card = deck->cards_in_deck[deck->num_cards-1];
    	player->player_hand[3] = top_card;
        print_hand(player);
	    deck->num_cards--;
        deck->top_card = deck->cards_in_deck[deck->num_cards - 1];
        printf("\n");
        printf("Player %d picked\n",player->player_id);
        print_hand(player);
        player->hand_size++;
	}
}



void throw_card(struct Player *player){
   

	unsigned int card_index;
    printf("Enter index of card to throw: \n");
    if(player->hand_size == MAX_HAND_SIZE){
        if(scanf("%d",&card_index) != 1 || card_index <0 || card_index > player->hand_size){
            printf("Invalid input. Try again\n");
            return;
        }
        
        struct Card discarded_card = player->player_hand[card_index];
        for(int i = card_index; i<player->hand_size;i++){
            player->player_hand[i] = player->player_hand[i+1];
        }

    player->player_hand[player->hand_size - 1] = (struct Card){0};
    player->hand_size--;
    printf("Card %d%c discarded.\n",discarded_card.value, discarded_card.suite);
    print_hand(player);
	}

}


void pair_by_twin(struct Player *players[],int active_players){

    for(int j = 0;j < active_players;j++){
        
        if(players[j]->player_hand[0].value == players[j]->player_hand[1].value){
        //Do nothing card(0,1) are already adjacent;
        }
        else if(players[j]->player_hand[0].value == players[j]->player_hand[2].value){
            players[j]->player_hand[3] = players[j]->player_hand[1];
            players[j]->player_hand[1] = players[j]->player_hand[2];
            players[j]->player_hand[2] = players[j]->player_hand[3];
            players[j]->player_hand[3] = (struct Card){0};
        
        }
        else if(players[j]->player_hand[1].value == players[j]->player_hand[2].value){
            players[j]->player_hand[3] = players[j]->player_hand[0];
            for(int i = 1;i < 4; i++){
                players[j]->player_hand[i-1] = players[j]->player_hand[i];
            
            } 
            players[j]->player_hand[3] = (struct Card){0};
        }
        
    }
}

struct Deck *populate_deck(){	
struct Deck *deck = malloc(sizeof(*deck));
assert(deck != NULL);

size_t number_of_cards = sizeof(values)/sizeof(values[0]);
size_t number_of_suites = sizeof(suites)/sizeof(suites[0]);
int i=0;
    for(size_t n =0;n<number_of_cards;n++){
	    for(size_t s = 0; s<number_of_suites;s++){
	        if(n>=52) break;
	        deck->cards_in_deck[i].value = values[n];
	        deck->cards_in_deck[i].suite = suites[s];
	        i++;
        }
    }
deck->num_cards = 52;
return deck;
}

void print_deck(const struct Deck *deck) {
    for (int i = 0; i < deck->num_cards; i++) {
	    printf("Card %d: %c\n", deck->cards_in_deck[i].value, deck->cards_in_deck[i].suite);
	}
}

int main(){

    struct Deck *deck = populate_deck();
    assert(deck != NULL);

    printf("Total cards in deck: %d\n",deck->num_cards);
    shuffle_deck(deck);
//print_deck(deck);
    printf("Welcome to twins and followers card game\n");
    int user_choice;
    while(1){
        printf("Enter 1 to play or 2 for the Rul3z\n");
        if(scanf("%d",&user_choice) != 1 || user_choice <= 0 || user_choice > 2){
            printf("Invalid input. Please try again...\n");
        }
        else break;
    }
    int num_players;
    while(1){
        printf("Enter number of players\n");
        if(scanf("%d",&num_players) == 1 && num_players <= MAX_PLAYER_COUNT && num_players > 0){
            for(int i = 0;i < num_players;i++){
                create_player();
            }
        }
    break;
    }

    printf("%d active players\n",active_players);
    print_players();
    deal_cards(deck);

    printf("Number of cards in deck: %d\n",deck->num_cards);
    pair_by_twin(player_container,active_players);
    for(int i = 0;i<active_players;i++){
        print_hand(player_container[i]);}

    for(int i = 0;i < active_players;i++){
        player_draw_card(player_container[i],deck);
        throw_card(player_container[i]);
    }
//print_hand(player_1);
//printf("Number of cards in deck: %d\n",deck->num_cards);

//throw_card(player_1);
//print_hand(player_1);
 return 0;
}

