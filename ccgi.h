/* cuteCGI (cCGI): small C library for CGI scripting.
 * version 1.2.22
 */

#ifndef CCGI_INCLUDED
#define CCGI_INCLUDED

#if !defined(__linux__) && !defined(__unix__)
#error "***ERROR: Unsupported platform"
#endif

#define CCGI_EXIT(msg)                              \
do {                                                \
    fprintf(stdout, msg);                           \
    exit(EXIT_FAILURE);                             \
} while(0)

#define CCGI_VERSION "ccgi-1.12"

#include "ccgi_shared.h"
#include "ccgi_stdout.h"
#include "ccgi_env.h"
#include "ccgi_request.h"
#include "ccgi_cookie.h"
#include "ccgi_header.h"
#include "ccgi_html.h"
#include "ccgi_session.h"

extern int
ccgi_process_request(void);

#endif
