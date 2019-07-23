/*** ccgi_env.h: Interface to HTTP Environment variables
 */

// returns the number of HTTP environment variables set
extern int
ccgi_env_vars_count(void);
// checks if HTTP environment variable with given name is set
extern CCGI_BOOL
ccgi_env_vars_isset(char *name);
// reads the value of HTTP environment variable with given name
extern char *
ccgi_env_vars_read(char *name);
// reads the name of n-th HTTP environment variable
extern char *
ccgi_env_vars_name_nth(int n);
// reads the value of n-th HTTP environment variable
extern char *
ccgi_env_vars_value_nth(int n);
// clears HTTP environment vars store - call it only when exiting your app
extern void
ccgi_env_vars_clear(void);


