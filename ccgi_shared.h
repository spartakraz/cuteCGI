/*** ccgi_shared.h: stuff to be shared by cuteCGI modules
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE (!TRUE)
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef int CCGI_BOOL;


