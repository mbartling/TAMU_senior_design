#ifndef MY_DEBUG_H
#define MY_DEBUG_H

#define DEBUG_MODE 0

#if DEBUG_MODE
#include <stdio.h>
#define D_PRINTF(...) printf(__VA_ARGS__)
#else
#define D_PRINTF(...) /* Dummy */
#endif

#endif /* MY_DEBUG_H */
