#ifndef GENERAL_DEBUG_H
#define GENERAL_DEBUG_H

#define DEBUG_MODE 1

#if DEBUG_MODE
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* Not DEBUG_MODE */
#define PRINTF(...) /* Dummy */
#endif /* DEBUG_MODE */

#endif /* GENERAL_DEBUG_H */
