/*** ccgi_session.h: Interface to Session data
 */

// sets directory for storing session data, e.g. /var/tmp/
extern CCGI_BOOL
ccgi_session_vars_set_dir(char *);
// saves value of the given session variable
extern int
ccgi_session_vars_write(char *, char *);
// loads value of the given session variable
extern char *
ccgi_session_vars_read(char *);
// checks if session variable exists
extern CCGI_BOOL
ccgi_session_vars_isset(char *);
