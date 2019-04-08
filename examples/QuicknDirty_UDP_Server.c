//
//  main.c
//  UDPservQuicknDirty
//
//  Created by Dominique Persson on 2019-04-04.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//

#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>
#define PORT 8080
#define SIZE 1024



int main(int argc, const char * argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    //SDLNet_init();
    
    IPaddress ip;
    SDLNet_ResolveHost(&ip, NULL, PORT);
    
    UDPsocket serverSock = SDLNet_UDP_Open(PORT);
    UDPpacket *packet;
    packet = SDLNet_AllocPacket(SIZE);
    
    for(;;){
        if(SDLNet_UDP_Recv(serverSock, packet)){
            printf("This is what you received bitch: %s\n", (char*)packet->data);
        }
    }
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(serverSock);
    SDLNet_Quit();
    
    return 0;
}
