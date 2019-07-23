/*** ccgi.c: cCGI implementation
 */

#include "ccgi.h"
#include "ccgi_str.c"
#include "ccgi_list.c"
#include "ccgi_env.c"
#include "ccgi_request.c"
#include "ccgi_cookie.c"
#include "ccgi_html.c"
#include "ccgi_header.c"
#include "ccgi_session.c"

static void ccgi_initialize(void)
{
    ccgi_env_vars_initialize();
    ccgi_cookie_vars_update();
    ccgi_request_vars_update();
}

static void
ccgi_finalize(void)
{
    ccgi_cookie_vars_clear();
    ccgi_env_vars_clear();
    ccgi_request_vars_clear();
}

int
main(void)
{
    ccgi_initialize();
    int rc = ccgi_process_request();
    ccgi_finalize();
    return rc;
}