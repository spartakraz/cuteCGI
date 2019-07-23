/*** ccgi_session.c: Implementation of interface to session data
 */

// default session directory
#define CCGI_DEFAULT_SESSION_DIR "/var/tmp/"

// current session directory
static char *ccgi_session_dir = NULL;

static char *
ccgi_session_vars_get_path(char *field_name);

static char *
ccgi_session_vars_get_path(char *field_name)
{
    char *path = !ccgi_session_dir ? snew(CCGI_DEFAULT_SESSION_DIR) : snew(ccgi_session_dir);
    path = sadds(path, ccgi_env_vars_read("REMOTE_ADDR"));
    path = sadds(path, field_name);
    return path;
}

CCGI_BOOL
ccgi_session_vars_set_dir(char *dir_name)
{
    ccgi_session_dir = snew(dir_name);
    struct stat dir_stat = {0};

    if (stat(ccgi_session_dir, &dir_stat) == -1) {
        return mkdir(ccgi_session_dir, 0777) == 0 ? TRUE : FALSE;
    } else {
        return TRUE;
    }
}

int
ccgi_session_vars_write(char *field_name, char *field_value)
{
    char *path = ccgi_session_vars_get_path(field_name);
    return ssave(field_value, path);
}

char *
ccgi_session_vars_read(char *field_name)
{
    char *path = ccgi_session_vars_get_path(field_name);
    return sdupl(sload(path));
}

extern CCGI_BOOL
ccgi_session_vars_isset(char *field_name)
{
    return (access(ccgi_session_vars_get_path(field_name), F_OK) != -1) ? TRUE : FALSE;
}
