/*** ccgi_request.h: Interface to HTTP Request variables
 */

// returns the number of HTTP request variables set
extern int
ccgi_request_vars_count(void);
// checks if HTTP request variable with given name is set
extern CCGI_BOOL
ccgi_request_vars_isset(char *name);
// reads the value of HTTP request variable with given name
extern char *
ccgi_request_vars_read(char *name);
// reads the name of n-th HTTP request variable
extern char *
ccgi_request_vars_name_nth(int n);
// reads the value of n-th HTTP request variable
extern char *
ccgi_request_vars_value_nth(int n);
// clears HTTP request vars store - call it only when exiting your app
extern void
ccgi_request_vars_clear(void);

// convenience alias for checking if specified submit was clicked
#define ccgi_submit_clicked ccgi_request_vars_isset

