#include "util.h"

// server client check
bool address_equal(IPaddress a, IPaddress b) {
    return a.host == b.host && a.port == b.port;
}
