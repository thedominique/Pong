//
//  main.c
//  TCP_serverEx
//
//  Created by Dominique Persson on 2019-03-28.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL_net.h"
#include <string.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello\n");
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    
    IPaddress ip;
    //char adress[]="www.google.com";
    SDLNet_ResolveHost(&ip, NULL, 1234); // server ip adress
    
    // tcp connection
    TCPsocket server = SDLNet_TCP_Open(&ip);
    TCPsocket client[3];
    int i=0;
    const char* text = "SMF Ricardo";
    char sendtext[100];
    while (1) {
        client[i] = SDLNet_TCP_Accept(server);
        while (client[i]) {
            // communicate w client
            SDLNet_TCP_Send(client[i], text, strlen(text));
            SDLNet_TCP_Recv(client[i], sendtext, 100);
            printf("client %d says: %s", i, sendtext);
            
            SDLNet_TCP_Close(client[i]);
            i++;
            if(i==3){
                i=0;
            }
            break;
        }
    }
    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
    
    
}
