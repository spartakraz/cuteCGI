/*** ccgi_html.c: implementation of HTML routines
 */

void
ccgi_html_start(char *title)
{
    ccgi_echoln("<html>");
    ccgi_echoln("<head><title>%s</title></head>", title);
    ccgi_echoln("<body>");
}

void
ccgi_html_end(void)
{
    ccgi_echoln("</body>");
    ccgi_echoln("</html>");
}

