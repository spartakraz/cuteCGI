/*** ccgi_request.c: implementation of the interface to HTTP request variables
 */

#define CCGI_QUERY_STRING_SEPARATOR "&"
#define CCGI_DEFAULT_CONTENT_LENGTH 65536

#define CCGI_GET_METHOD (1 << 0)
#define CCGI_POST_METHOD (1 << 1)
#define CCGI_UNKNOWN_METHOD (1 << 2)

// associative container for storing both HTTP_GET_VARS and HTTP_POST_VARS
static LIST *ccgi_http_request_vars = NULL;

static int
ccgi_get_method(void)
{
    int flags = 0;
    flags &= ~(CCGI_GET_METHOD | CCGI_POST_METHOD | CCGI_UNKNOWN_METHOD);
    char *method = ccgi_env_vars_read("REQUEST_METHOD");
    if (!method || !strcmp("GET", method)) {
        flags |= CCGI_GET_METHOD;

    } else if (!strcmp("POST", method)) {
        flags |= CCGI_POST_METHOD;
    } else {
        flags |= CCGI_UNKNOWN_METHOD;
    }
    return flags;
}

// reloads HTTP_GET_VARS container on new request

static int
ccgi_get_vars_update(void)
{
    ccgi_http_request_vars = NULL;
    char *query_string = ccgi_env_vars_read("QUERY_STRING");
    if (!query_string || !strlen(query_string)) {
        return 0;
    }
    ccgi_http_request_vars = lparse(query_string, CCGI_QUERY_STRING_SEPARATOR);
    return lcount(ccgi_http_request_vars);
}

// reloads HTTP_POST_VARS container on new request

static int
ccgi_post_vars_update(void)
{
    ccgi_http_request_vars = NULL;
    char *content_len = ccgi_env_vars_read("CONTENT_LENGTH");
    int input_len = content_len ? atoi(content_len) + 1 : CCGI_DEFAULT_CONTENT_LENGTH;
    char *input = (char *) malloc(sizeof (char) * input_len);
    fscanf(ccgi_in, "%s", input);
    ccgi_http_request_vars = lparse(input, CCGI_QUERY_STRING_SEPARATOR);
    return lcount(ccgi_http_request_vars);
}

int
ccgi_request_vars_update(void)
{
    int flags = ccgi_get_method();
    if (flags & CCGI_POST_METHOD) {
        return ccgi_post_vars_update();
    } else if (flags & CCGI_GET_METHOD) {
        return ccgi_get_vars_update();
    } else if (flags & CCGI_UNKNOWN_METHOD) {
        return ccgi_get_vars_update();
    } else {
        return ccgi_get_vars_update();
    }
}

int
ccgi_request_vars_count(void)
{
    return lcount(ccgi_http_request_vars);
}

CCGI_BOOL
ccgi_request_vars_isset(char *name)
{
    return lhaskey(ccgi_http_request_vars, name);
}

char *
ccgi_request_vars_read(char *name)
{
    return lreadkey(ccgi_http_request_vars, name);
}

char *
ccgi_request_vars_name_nth(int n)
{
    return lkeyat(ccgi_http_request_vars, n);
}

char *
ccgi_request_vars_value_nth(int n)
{
    return lvalat(ccgi_http_request_vars, n);
}

void
ccgi_request_vars_clear(void)
{
    ldel(ccgi_http_request_vars);
}