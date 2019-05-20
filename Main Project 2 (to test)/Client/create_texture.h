#ifndef create_texture_h
#define create_texture_h
#include "objects.h"


SDL_Texture *init_board_texture(SDL_Renderer *renderer);
SDL_Texture *create_player_texture(SDL_Renderer *renderer, char image_name[]);
SDL_Texture *init_player2_texture(SDL_Renderer *renderer);
SDL_Texture *init_player3_texture(SDL_Renderer *renderer);


#endif // !create_texture_h
