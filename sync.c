#include "sync.h"

#include "constants.h"
#include <stdbool.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>

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

