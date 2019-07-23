#include "ccgi/ccgi.h"

#define ACTION "visitcount.cgi"
#define METHOD "GET"

int
ccgi_process_request(void) {
    ccgi_session_vars_set_dir("/var/tmp/visitcount_session/");
    int count = ccgi_session_vars_isset("count") ? atoi(ccgi_session_vars_read("count")) : 0;
    count++;
    char *sesvar = (char *) malloc(3);
    sprintf(sesvar, "%d", count);
    (void) ccgi_session_vars_write("count", sesvar);
    ccgi_header_send_text_html();
    ccgi_html_start("Test SESSION");
    ccgi_echoln("<FORM name='mainfrm' ACTION='%s' METHOD='%s'>", ACTION, METHOD);
    ccgi_echoln("Count= %d<BR>", count);
    ccgi_echoln("<button type='submit' name='submit'>Submit</button>");
    ccgi_echoln("</FORM>");
    ccgi_html_end();
    return 0;
}
