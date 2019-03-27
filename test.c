#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct
{
    int x, y;
    short life;
    char *name;
} Man;

int processEvents(SDL_Window *window, Man *man)
{
    SDL_Event event;
    int done = 0;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
                break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
                break;
            case SDL_QUIT:
                //quit out of the game
                done = 1;
                break;
        }
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        man->x -= 20;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        man->x += 20;
    }
    if (state[SDL_SCANCODE_UP])
    {
        man->y -= 20;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        man->y += 20;
    }
    
    return done;
}

void doRender(SDL_Renderer *renderer, Man *man)
{
    //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    SDL_Rect rect = { man->x,man->y, 20, 200 };
    SDL_RenderFillRect(renderer, &rect);
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}

void rend2(SDL_Renderer *ren2, Man *man2)
{
    SDL_SetRenderDrawColor(ren2, 0, 0, 50, 255);
    
    SDL_RenderClear(ren2);
    
    SDL_SetRenderDrawColor(ren2, 255, 255, 255, 255);
    
    SDL_Rect rect2 = { 10,10,20,20 };
    
    SDL_RenderFillRect(ren2, &rect2);
    
    SDL_RenderPresent(ren2);
    
    
    
}

int main(int argc, char *argv[])
{
    SDL_Window *window;                    // Declare a window
    SDL_Renderer *renderer;                // Declare a renderer
    
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    
    Man man,man2;
    man.x = 220;
    man.y = 140;
    man.x = 220;
    man.y = 140;
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",                     // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              1000,                               // width, in pixels
                              800,                               // height, in pixels
                              0                                  // flags
                              );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    
    
    // The window is open: enter program loop (see SDL_PollEvent)
    int done = 0;
    
    //Event loop
    while (!done)
    {
        //Check for events
        done = processEvents(window, &man);
        
        //Render display
        doRender(renderer, &man);
        
        
        
        //don't burn up the CPU
        SDL_Delay(1000/60);
    }
    
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Clean up
    SDL_Quit();
    return 0;
}
