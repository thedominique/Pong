//
//  main.c
//  ClientEx
//
//  Created by Dominique Persson on 2019-03-28.
//  Copyright © 2019 Dominique Persson. All rights reserved.
//

#include <stdio.h>
#include "SDL_net.h"
#include <SDL2/SDL.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    
    IPaddress ip;

    //char adress[]="www.google.com";
    SDLNet_ResolveHost(&ip, "130.229.149.121", 1234); // server ip adress
    
    // tcp connection
    const char* sendtext = "Jag vill ha en cheese burgare\n";
    TCPsocket client = SDLNet_TCP_Open(&ip);
    //TCPsocket server = SDLNet_TCP_Open(&remote_ip);
    
    //IPaddress *remote_ip;
    
    char text[100];
    SDLNet_TCP_Recv(client, text, 100);
    //remote_ip = SDLNet_TCP_GetPeerAddress(server);
    SDLNet_TCP_Send(client, sendtext, strlen(sendtext));
    printf("%s",text);
    
    
    SDLNet_TCP_Close(client);
    
    SDLNet_Quit();
    SDL_Quit();
    
}
