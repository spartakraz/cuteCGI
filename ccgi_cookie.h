/*** ccgi_cookie.h: interface to HTTP Cookies
 */

// returns the number of HTTP Cookies set
extern int
ccgi_cookie_vars_count(void);
// checks if given cookie is set
extern CCGI_BOOL
ccgi_cookie_vars_isset(char *name);
// sets cookie
extern void
ccgi_cookie_vars_set(char *name,
        char *value,
        char *expires,
        char *path,
        char *domain,
        int secure);
// reads cookie's value
extern char *
ccgi_cookie_vars_read(char *name);
// reads the name of n-th cookie
extern char *
ccgi_cookie_vars_name_nth(int n);
// reads the value of n-th cookie
extern char *
ccgi_cookie_vars_value_nth(int n);
// clears cookie vars store - call it only when exiting your app
extern void
ccgi_cookie_vars_clear(void);


