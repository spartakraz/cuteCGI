/*** ccgi_cookie.c: implementation of the interface to HTTP cookies
 */

#define CCGI_HTTP_COOKIE_SEPARATOR ";"

static LIST *ccgi_http_cookie_vars = NULL;

static int
ccgi_cookie_vars_update(void)
{
    ccgi_http_cookie_vars = NULL;
    char *cookie_str = ccgi_env_vars_read("HTTP_COOKIE");
    if (!cookie_str || !strlen(cookie_str)) {
        return 0;
    }
    ccgi_http_cookie_vars = lparse(cookie_str, CCGI_HTTP_COOKIE_SEPARATOR);
    return lcount(ccgi_http_cookie_vars);
}

int
ccgi_cookie_vars_count(void)
{
    return lcount(ccgi_http_cookie_vars);
}

CCGI_BOOL
ccgi_cookie_vars_isset(char *name)
{
    return lhaskey(ccgi_http_cookie_vars, name);
}

void
ccgi_cookie_vars_set(char *name,
        char *value,
        char *expires,
        char *path,
        char *domain,
        int secure)
{
    ccgi_echo("Set-Cookie: %s=%s;", name, sesc(value));
    if (expires) {
        ccgi_echo(" EXPIRES=%s;", expires);
    }
    if (path) {
        ccgi_echo(" PATH=%s;", path);
    }
    if (domain) {
        ccgi_echo(" DOMAIN=%s;", domain);
    }
    if (secure) {
        ccgi_echo(" SECURE");
    }
    ccgi_echo("\n");
}

char *
ccgi_cookie_vars_read(char *name)
{
    return lreadkey(ccgi_http_cookie_vars, name);
}

char *
ccgi_cookie_vars_name_nth(int n)
{
    return lkeyat(ccgi_http_cookie_vars, n);
}

char *
ccgi_cookie_vars_value_nth(int n)
{
    return lvalat(ccgi_http_cookie_vars, n);
}

void
ccgi_cookie_vars_clear(void)
{
    ldel(ccgi_http_cookie_vars);
}