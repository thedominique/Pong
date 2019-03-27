#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello");
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    
    IPaddress ip;
    //char adress[]="www.google.com";
    SDLNet_ResolveHost(&ip, NULL, 8080); // server ip adress
    
    // tcp connection
    TCPsocket server = SDLNet_TCP_Open(&ip);
    TCPsocket client;
    
    const char* text = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: reset\r\n\r\n<h1>Welcome to my server b. You now have permission to sock my ass</h1>";
    while(1){
        client = SDLNet_TCP_Accept(server);
        if(client){
            // communicate w client
            SDLNet_TCP_Send(client,text, strlen(text)+1);
            SDLNet_TCP_Close(client);
            break;
        }
    }
    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
    
    
}
