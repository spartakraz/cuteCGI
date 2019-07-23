/*** ccgi_header.h: HTTP Header routines
 */

extern void
ccgi_header_send_text_html(void);
extern void
ccgi_header_send_mime(char *mime);
extern void
ccgi_header_send_status(char *status);
extern void
ccgi_header_send_pragma(char *pragma);
extern void
ccgi_header_send_location(char *location);


