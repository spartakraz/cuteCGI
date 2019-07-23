/*** ccgi_http.c: implementation of HTTP header routines
 */

void
ccgi_header_send_text_html(void)
{
    ccgi_echo("Content-type: text/html\n\n");
}

void
ccgi_header_send_mime(char *mime)
{
    ccgi_echo("Content-type: %s\n\n", mime);
}

void
ccgi_header_send_status(char *status)
{
    ccgi_echo("Status: %s\n", status);
}

void
ccgi_header_send_pragma(char *pragma)
{
    ccgi_echo("Pragma: %s\n", pragma);
}

void
ccgi_header_send_location(char *location)
{
    ccgi_echo("Location: %s\n\n", location);
}
