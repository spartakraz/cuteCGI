/*** ccgi_env.c: implementation of the interface to HTTP environment variables
 */

static LIST *ccgi_http_env_vars = NULL;

static void
ccgi_env_vars_initialize(void)
{
    ccgi_http_env_vars = NULL;
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "AUTH_TYPE", getenv("AUTH_TYPE"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "CONTENT_LENGTH", getenv("CONTENT_LENGTH"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "CONTENT_TYPE", getenv("CONTENT_TYPE"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "DOCUMENT_ROOT", getenv("DOCUMENT_ROOT"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "DOCUMENT_URI", getenv("DOCUMENT_URI"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "GATEWAY_INTERFACE", getenv("GATEWAY_INTERFACE"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "HTTP_COOKIE", getenv("HTTP_COOKIE"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "HTTP_REFERER", getenv("HTTP_REFERER"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "HTTP_USER_AGENT", getenv("HTTP_USER_AGENT"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "HTTPS", getenv("HTTPS"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "PATH_INFO", getenv("PATH_INFO"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "PATH_TRANSLATED", getenv("PATH_TRANSLATED"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "QUERY_STRING", getenv("QUERY_STRING"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REMOTE_ADDR", getenv("REMOTE_ADDR"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REMOTE_HOST", getenv("REMOTE_HOST"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REMOTE_IDENT", getenv("REMOTE_IDENT"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REMOTE_PORT", getenv("REMOTE_PORT"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REMOTE_USER", getenv("REMOTE_USER"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REQUEST_URI", getenv("REQUEST_URI"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "REQUEST_METHOD", getenv("REQUEST_METHOD"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SCRIPT_FILENAME", getenv("SCRIPT_FILENAME"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SCRIPT_NAME", getenv("SCRIPT_NAME"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SERVER_ADDR", getenv("SERVER_ADDR"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SERVER_NAME", getenv("SERVER_NAME"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SERVER_PORT", getenv("SERVER_PORT"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SERVER_PROTOCOL", getenv("SERVER_PROTOCOL"));
    ccgi_http_env_vars = ladd(ccgi_http_env_vars, "SERVER_SOFTWARE", getenv("SERVER_SOFTWARE"));
}

int
ccgi_env_vars_count(void)
{
    return lcount(ccgi_http_env_vars);
}

CCGI_BOOL
ccgi_env_vars_isset(char *name)
{
    return lhaskey(ccgi_http_env_vars, name);
}

char *
ccgi_env_vars_read(char *name)
{
    return lreadkey(ccgi_http_env_vars, name);
}

char *
ccgi_env_vars_name_nth(int n)
{
    return lkeyat(ccgi_http_env_vars, n);
}

char *
ccgi_env_vars_value_nth(int n)
{
    return lvalat(ccgi_http_env_vars, n);
}

void
ccgi_env_vars_clear(void)
{
    ldel(ccgi_http_env_vars);
}