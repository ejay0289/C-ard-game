#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PLAYER_COUNT 4
#define DECK_SIZE 52
#define MAX_HAND_SIZE 4

int game_won = 0;
struct Player;
int check_twins(struct Player *player);
void check_win(struct Player *player);
int check_followers(struct Player *player);

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
//    printf("%d",player->hand_size);
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

    printf("%d",player_container[i]->player_hand[3].value);
	}
    

}

void player_draw_card(struct Player *player,struct Deck *deck){
    if(player->hand_size == 3){
    	struct Card top_card = deck->cards_in_deck[deck->num_cards-1];
    	player->player_hand[3] = top_card;
	    deck->num_cards--;
        deck->top_card = deck->cards_in_deck[deck->num_cards - 1];
        printf("\n");
        printf("Player %d picked\n",player->player_id);
        player->hand_size++;
        print_hand(player);


    if(!check_twins(player) && player->player_hand[3].value != 0){

            if(player->player_hand[3].value == player->player_hand[2].value){
            struct Card temp1 = player->player_hand[2];
            struct Card temp2 = player->player_hand[3];

            player->player_hand[2] = player->player_hand[0];
            player->player_hand[3] = player->player_hand[1];
            player->player_hand[0] = temp1;
            player->player_hand[1] = temp2;
            print_hand(player);
            }
            else if(player->player_hand[3].value == player->player_hand[1].value){
            struct Card temp = player->player_hand[3];
            player->player_hand[3] = player->player_hand[0];
            player->player_hand[0] = temp;
                     
            print_hand(player);
            }

            else if(player->player_hand[3].value == player->player_hand[0].value){
                struct Card temp = player->player_hand[3];
                player->player_hand[3] = player->player_hand[1];
                player->player_hand[1] = temp;
            print_hand(player);

            }
        }
    if(!check_twins(player) && player->player_hand[3].value != 0 && !check_followers(player)){
        if(player->player_hand[3].value == player->player_hand[2].value-1 || player->player_hand[3].value == player->player_hand[2].value+1){
            struct Card temp1 = player->player_hand[2];
            struct Card temp2 = player->player_hand[3];
            player->player_hand[2] = player->player_hand[0];
            player->player_hand[3] = player->player_hand[1];
            player->player_hand[0] = temp1;
            player->player_hand[1] = temp2;
            print_hand(player);
    }
        else if(player->player_hand[3].value == player->player_hand[1].value-1 || player->player_hand[3].value == player->player_hand[1].value+1){
            struct Card temp = player->player_hand[3];
            player->player_hand[3] = player->player_hand[0];
            player->player_hand[0] = temp;

            print_hand(player);
    }

        else if(player->player_hand[3].value == player->player_hand[0].value-1 || player->player_hand[3].value == player->player_hand[0].value+1){
            struct Card temp = player->player_hand[3];
            player->player_hand[3] = player->player_hand[1];
            player->player_hand[1] = temp;

            print_hand(player);
        }
    }
      check_win(player); 
	}
}

void check_win(struct Player *player){
    if(player->hand_size == 4){
        if(player->player_hand[0].value == player->player_hand[1].value && (player->player_hand[2].value == player->player_hand[3].value - 1 || player->player_hand[2].value == player->player_hand[3].value + 1)){
            printf("Player %d won. Game 0ver\n",player->player_id);
            game_won = 1;
         }
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


int check_twins(struct Player *player){
    int pair_found = 0;
    
            
              
        if(player->player_hand[0].value == player->player_hand[1].value){
            pair_found = 1;
        }
        else if(player->player_hand[0].value == player->player_hand[2].value){
            player->player_hand[3] = player->player_hand[1];
            player->player_hand[1] = player->player_hand[2];
            player->player_hand[2] = player->player_hand[3];
            player->player_hand[3] = (struct Card){0};
            pair_found = 1;    
        }
        else if(player->player_hand[1].value == player->player_hand[2].value){
            player->player_hand[3] = player->player_hand[0];
            for(int i = 1;i < 4; i++){
                player->player_hand[i-1] = player->player_hand[i];
            
            } 
            player->player_hand[3] = (struct Card){0};
            pair_found = 1;
        }
        else pair_found = 0;    
    
    return pair_found;
}

int check_followers(struct Player *player){
int followers_found = 0;
if(!check_twins(player)){
    


    if(player->player_hand[0].value == player->player_hand[1].value - 1 || player->player_hand[0].value == player->player_hand[1].value + 1){
        followers_found = 1;
        printf("Player %d has followers!\n",player->player_id);
    }

    else if(player->player_hand[0].value == player->player_hand[2].value-1||(player->player_hand[0].value == player->player_hand[2].value+1)){
            player->player_hand[3] = player->player_hand[1];
            player->player_hand[1] = player->player_hand[2];
            player->player_hand[2] = player->player_hand[3];
            player->player_hand[3] = (struct Card){0};
            followers_found = 1;
        printf("Player %d has followers!\n",player->player_id);
        }
    else if(player->player_hand[0].value == player->player_hand[2].value-1||player->player_hand[0].value == player->player_hand[2].value+1){
            player->player_hand[3] = player->player_hand[0];
            for(int i = 1;i < 4; i++){
                player->player_hand[i-1] = player->player_hand[i];

            }
            player->player_hand[3] = (struct Card){0};
            followers_found = 1;
        printf("Player %d has followers!\n",player->player_id);
        }
    else if(player->player_hand[1].value == player->player_hand[2].value-1||(player->player_hand[1].value == player->player_hand[2].value+1)){
            player->player_hand[3] = player->player_hand[0];
            player->player_hand[0] = player->player_hand[2];
            player->player_hand[2] = player->player_hand[3];
            player->player_hand[3] = (struct Card){0};
            followers_found = 1;
        printf("Player %d has followers!\n",player->player_id);
        }
    else followers_found = 0;
}
    return followers_found;

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
   
while(!game_won){ 
    for(int i = 0;i<active_players;i++){
        check_twins(player_container[i]);
        check_followers(player_container[i]);
    }

    for(int i = 0;i<active_players;i++){
        print_hand(player_container[i]);
    }

    for(int i = 0;i < active_players;i++){
        printf("\n");
        player_draw_card(player_container[i],deck);
        if(game_won) break;
        throw_card(player_container[i]);
    }
}
//print_hand(player_1);
//printf("Number of cards in deck: %d\n",deck->num_cards);

//throw_card(player_1);
//print_hand(player_1);
 return 0;
}

