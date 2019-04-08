#ifndef util_h
#define util_h

#include "SDL_net.h"
#include <stdbool.h>

#define MIN(a,b) (((a)<(b))?(a):(b)) // om a < b ta a
#define MAX(a,b) (((a)>(b))?(a):(b)) // om a > b ta a

// server client check
bool address_equal(IPaddress a, IPaddress b);

#endif /* util_h */
