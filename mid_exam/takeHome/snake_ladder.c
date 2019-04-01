#include <stdio.h> 
#include <string.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
#include <sys/time.h>


// Global Variables

pthread_t th_id[4]; 
long int th_count = 0;
pthread_mutex_t th_locks[4]; 
pthread_mutex_t m_lock;

// Co-Routine Functions 
//initialisation
void handle_init(){
	
	int i = 0; 
	for(i = 0; i < 4; i++){
		pthread_mutex_init(&(th_locks[i]), NULL);
		pthread_mutex_lock(&(th_locks[i]));
	}

	pthread_mutex_lock(&m_lock);
}

// Create a co-routine by passing function pointer
int handle_create(void *(*func)(void *)){
	pthread_create(&(th_id[th_count++]), NULL, func, NULL);
	pthread_mutex_lock(&m_lock);
	return th_count - 1;
}

// Send signal to continue execution
void handle_send(int handle_id){
	pthread_mutex_unlock(&(th_locks[handle_id]));
	pthread_mutex_lock(&m_lock);
}

// to break a co-routine and jump to next one
void handle_breakpoint(int handle_id){
	pthread_mutex_unlock(&m_lock);
	pthread_mutex_lock(&(th_locks[handle_id]));
}

// Kill a co routine
void handle_kill(int handle_id){
	pthread_cancel(th_id[handle_id]);
	pthread_mutex_destroy(&(th_locks[handle_id])); 
}

//SUBROUTINES FOR SNAKE & LADDER
int pl_loc[100];
int snake_src[8] = {26, 34, 48, 55, 62, 78, 85, 99};
int snake_dst[8] = {4, 10, 5, 20, 30, 24,55 , 9};

int ladder_src[8] = {5, 11, 17, 24, 33, 41, 63, 80};
int ladder_dst[8] = {30, 43, 76, 87, 59, 65, 92, 97};

int pl_win[100];

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
	int handle_id = th_count - 1;
	int pl_id = handle_id;
	pl_loc[pl_id] = 0;
	pl_win[pl_id] = 0;
	
	while(1){
		handle_breakpoint(handle_id);
		
		int dice_roll = roll_dice();
		pl_loc[pl_id] += dice_roll;

		while(1){
			// exit if no snake or ladder encountered
			if(get_ind(pl_loc[pl_id], snake_src, 8) == -1 && get_ind(pl_loc[pl_id], ladder_src, 8) == -1){
				break;
			}
			// Check for snake
			else if(get_ind(pl_loc[pl_id], snake_src, 8) != -1){
				int i = get_ind(pl_loc[pl_id], snake_src, 7);
				pl_loc[pl_id] = snake_dst[i];
			}	
			// Check for ladder
			else if(get_ind(pl_loc[pl_id], ladder_src, 8) != -1){
				int i = get_ind(pl_loc[pl_id], ladder_src, 7);
				pl_loc[pl_id] = ladder_dst[i];	
			}	
		}

		// Check for win
		if(pl_loc[pl_id] >= 100)
		{
			t=clock()-t;
			double time_taken=((double)t)/CLOCKS_PER_SEC;
			printf("Player %d Wins @ location %d.. \nThe game finished in %f us\n", handle_id + 1, pl_loc[pl_id], time_taken*1000000);
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
	handle_init();


	// Create co-routines 
	int p1 = handle_create(&player);
	int p2 = handle_create(&player);
	int p3 = handle_create(&player);
	int p4 = handle_create(&player);
	
	// Send some messages to co-routines
	while(pl_win[p1] == 0 | pl_win[p2] == 0 | pl_win[p3] == 0 | pl_win[p4] == 0){
		
		if(pl_win[p1] == 0)handle_send(p1);
		if(pl_win[p2] == 0)handle_send(p2);
		if(pl_win[p3] == 0)handle_send(p3);
		if(pl_win[p4] == 0)handle_send(p4);
	}

	// Kill the routines
	handle_kill(p1);
	handle_kill(p2);
	handle_kill(p3);
	handle_kill(p4);
		
	return 0; 
} 
