/*** ccgi_stdout.h: STDOUT routines
 */

// wrapper around printf

#define ccgi_out stdout
#define ccgi_in stdin

static inline void
ccgi_echo(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    (void) vfprintf(ccgi_out, fmt, args);
    va_end(args);
}

// wrapper around

static inline void
ccgi_echoln(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    (void) vfprintf(ccgi_out, fmt, args);
    (void) fprintf(ccgi_out, "\n");
    va_end(args);
}


