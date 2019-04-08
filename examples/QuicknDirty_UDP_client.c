//
//  main.c
//  UDPClientQuicknDirty
//
//  Created by Dominique Persson on 2019-04-05.
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
    UDPsocket clientSock = SDLNet_UDP_Open(PORT);
    IPaddress ip;
    UDPpacket *packet;
    
    SDLNet_ResolveHost(&ip, "130.229.160.32", PORT);
    packet = SDLNet_AllocPacket(SIZE);
    
    for(;;){
        printf("Fill buffer\n>");
        gets((char *)packet->data);
    
        packet->address.host = ip.host;    /* Set the destination host */
        packet->address.port = ip.port;    /* And destination port */
        
        /* Quit if packet contains "quit" */
        if (!strcmp((char *)packet->data, "quit"))
            break;

        //send dat b
        SDLNet_UDP_Send(clientSock, -1, packet);
        
        if (SDLNet_UDP_Recv(clientSock, packet)){
            printf("%s\n", (char*)packet->data);
        }
    }
    SDLNet_FreePacket(packet);
    SDLNet_Quit();
    
    return EXIT_SUCCESS;
    return 0;
}
