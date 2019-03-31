#include <stdio.h> 
#include <string.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
#include <sys/time.h>


// Global Variables

pthread_t th_id[100]; 
long int th_count = 0;
pthread_mutex_t th_locks[100]; 
pthread_mutex_t m_lock;

// Co-Routine Functions 
//initialisation
void co_init(){
	
	int i = 0; 
	for(i = 0; i < 100; i++){
		pthread_mutex_init(&(th_locks[i]), NULL);
		pthread_mutex_lock(&(th_locks[i]));
	}

	pthread_mutex_lock(&m_lock);
}

// Create a co-routine by passing function pointer
int co_create(void *(*func)(void *)){
	pthread_create(&(th_id[th_count++]), NULL, func, NULL);
	pthread_mutex_lock(&m_lock);
	return th_count - 1;
}

// Send signal to continue execution
void co_send(int co_id){
	pthread_mutex_unlock(&(th_locks[co_id]));
	pthread_mutex_lock(&m_lock);
}

// to break a co-routine and jump to next one
void co_breakpoint(int co_id){
	pthread_mutex_unlock(&m_lock);
	pthread_mutex_lock(&(th_locks[co_id]));
}

// Kill a co routine
void co_kill(int co_id){
	pthread_cancel(th_id[co_id]);
	pthread_mutex_destroy(&(th_locks[co_id])); 
}

//SUBROUTINES FOR SNAKE & LADDER
int pl_loc[100];
int snake_src[7] = {26, 34, 48, 55, 78, 85, 99};
int snake_dst[7] = {4, 10, 5, 20, 30, 55, 9};

int ladder_src[7] = {5, 11, 17, 24, 41, 63, 80};
int ladder_dst[7] = {30, 43, 76, 87, 65, 92, 97};

int PlayerWin[100];

// Roll the dice
int roll_dice(){
	return (rand()%6) + 1;
}

int get_ind(int v, int * l, int len){
	int i = 0;
	for(i = 0; i < len; i++){
		if(l[i] == v) return i;	
	}
	return -1;
}

//CO-ROUTINES FOR SNAKE & LADDER
// Play a round
void* player(void *arg) 
{ 
	clock_t t;
	t=clock();
	int co_id = th_count - 1;
	int player_id = co_id;
	pl_loc[player_id] = 0;
	PlayerWin[player_id] = 0;
	
	while(1){
		co_breakpoint(co_id);
		
		int dice_roll = roll_dice();
		pl_loc[player_id] += dice_roll;

		while(1){
			// exit if no snake or ladder encountered
			if(get_ind(pl_loc[player_id], snake_src, 7) == -1 && get_ind(pl_loc[player_id], ladder_src, 7) == -1){
				break;
			}
			// Check for snake
			else if(get_ind(pl_loc[player_id], snake_src, 7) != -1){
				int i = get_ind(pl_loc[player_id], snake_src, 7);
				pl_loc[player_id] = snake_dst[i];
			}	
			// Check for ladder
			else if(get_ind(pl_loc[player_id], ladder_src, 7) != -1){
				int i = get_ind(pl_loc[player_id], ladder_src, 7);
				pl_loc[player_id] = ladder_dst[i];	
			}	
		}

		// Check for win
		if(pl_loc[player_id] >= 100)
		{
			t=clock()-t;
			double time_taken=((double)t)/CLOCKS_PER_SEC;
			printf("Player %d Wins @ location %d.. \nThe game finished in %f us\n", co_id + 1, pl_loc[player_id], time_taken*1000000);
			printf("\nStats:\nPlayer1 at pos: %d\nPlayer2 at pos: %d\nPlayer3 at pos: %d\nPlayer4 at pos: %d\n",pl_loc[0],pl_loc[1],pl_loc[2],pl_loc[3]);
			exit(0);
		}
	
	}

	return NULL; 
} 

int main(void) 
{ 	
	struct timeval stop, start;
	srand(time(0)); 
	// Initialize
	co_init();


	// Create co-routines 
	int p1 = co_create(&player);
	int p2 = co_create(&player);
	int p3 = co_create(&player);
	int p4 = co_create(&player);
	
	// Send some messages to co-routines
	while(PlayerWin[p1] == 0 | PlayerWin[p2] == 0 | PlayerWin[p3] == 0 | PlayerWin[p4] == 0){
		
		if(PlayerWin[p1] == 0)co_send(p1);
		if(PlayerWin[p2] == 0)co_send(p2);
		if(PlayerWin[p3] == 0)co_send(p3);
		if(PlayerWin[p4] == 0)co_send(p4);
	}

	// Kill the routines
	co_kill(p1);
	co_kill(p2);
	co_kill(p3);
	co_kill(p4);
		
	return 0; 
} 