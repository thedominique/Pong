#include "sync.h"

#include "constants.h"
#include <stdbool.h>

// vad klienten ska skicka
typedef struct client_packet {
    double pad_y;
} client_packet_t;

// vad servern ska skicka
typedef struct server_packet {
    double pad_y;
    double ball_x;
    double ball_y;
    double ball_dv_x;
    double ball_dv_y;
    bool is_player1;
} server_packet_t;

void init_tick(){
    long ticks_per_sec = SDL_GetPerformanceFrequency();
    long tick_t0 = SDL_GetPerformanceCounter();
    
    long next_net_tick = tick_t0; // make pointer dude
    long net_tick_interval = (1 / NET_TICK_RATE) * ticks_per_sec; // make pointer dude eller extern 
}

// use fucntion to ge the high resolution counter. Returns a platform specific counter / sec
double main_tick_loop(long tick_t0, long ticks_per_sec){
    long tick_t1 = SDL_GetPerformanceCounter();
    double dt = (tick_t1 - tick_t0) / (double) ticks_per_sec;
    return dt; // vet ej om den ska va här, ev gör en pekare till main idk
}


