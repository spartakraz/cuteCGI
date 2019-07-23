#include <fcntl.h>
#include <gdbm-ndbm.h>
#include <ccgi/ccgi.h>

#define DB_FILE_NAME "/var/tmp/guestbook-data"

#define TITLE "CCGI GuestBook Sample"
#define ACTION "guestbook.cgi"
#define METHOD "post"

#define DEFAULT_NAME "johndoe"
#define DEFAULT_EMAIL "johndoe@somemail.net"
#define DEFAULT_MESSAGE "Hello from johndoe"

#define NAME_ERROR "name not defined"
#define EMAIL_ERROR "email not defined"
#define MESSAGE_ERROR "message not defined"
#define DB_ERROR "guestbook: connection error!"
#define DATA_ERROR "guestbook: data edit error!"

#define NAME_SIZE 11
#define EMAIL_SIZE 51
#define MESSAGE_SIZE 121
#define ERROR_SIZE 21

typedef struct guest_rec {
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char message[MESSAGE_SIZE];
} GUEST_REC;

static DBM *dbm_ptr = NULL;

static GUEST_REC *
new_guest_rec(const char *name,
        const char *email,
        const char *message);

static void
update_cookies(void);
static char *
get_name(void);
static char *
get_email(void);
static char *
get_message(void);

static void
start_html(void);
static void
end_html(void);
static void
handle_edit_submit(const char *error);
static void
handle_view_submit(void);
static void
handle_add_submit(void);
static void
default_handler(void);

static CCGI_BOOL
connect_db(void);
static void
disconnect_db(void);
static CCGI_BOOL
add_record(GUEST_REC *rec_ptr);
static CCGI_BOOL
print_all_records(void);

static GUEST_REC *
new_guest_rec(const char *name,
        const char *email,
        const char *message) {
    GUEST_REC *new_rec_ptr = (GUEST_REC *) malloc(sizeof (GUEST_REC));
    strcpy(new_rec_ptr->name, name);
    strcpy(new_rec_ptr->email, email);
    strcpy(new_rec_ptr->message, message);
    return new_rec_ptr;
}

static void
update_cookies(void) {
    if (ccgi_cookie_vars_isset("name") == FALSE) {
        ccgi_cookie_vars_set("name", DEFAULT_NAME, NULL, NULL, NULL, 0);
    }
    if (ccgi_cookie_vars_isset("email") == FALSE) {
        ccgi_cookie_vars_set("email", DEFAULT_EMAIL, NULL, NULL, NULL, 0);
    }
    if (ccgi_cookie_vars_isset("message") == FALSE) {
        ccgi_cookie_vars_set("message", DEFAULT_MESSAGE, NULL, NULL, NULL, 0);
    }
    if (ccgi_request_vars_isset("nameText") == TRUE) {
        ccgi_cookie_vars_set("name", ccgi_request_vars_read("nameText"), NULL, NULL, NULL, 0);
    }
    if (ccgi_request_vars_isset("emailText") == TRUE) {
        ccgi_cookie_vars_set("email", ccgi_request_vars_read("emailText"), NULL, NULL, NULL, 0);
    }
    if (ccgi_request_vars_isset("messageText") == TRUE) {
        ccgi_cookie_vars_set("message", ccgi_request_vars_read("messageText"), NULL, NULL, NULL, 0);
    }
}

static char *
get_name(void) {
    if (ccgi_request_vars_isset("nameText") == TRUE) {
        return ccgi_request_vars_read("nameText");
    }
    return ccgi_cookie_vars_isset("name") ? ccgi_cookie_vars_read("name") : DEFAULT_NAME;
}

static char *
get_email(void) {
    if (ccgi_request_vars_isset("emailText") == TRUE) {
        return ccgi_request_vars_read("emailText");
    }
    return ccgi_cookie_vars_isset("email") ? ccgi_cookie_vars_read("email") : DEFAULT_EMAIL;
}

static char *
get_message(void) {
    if (ccgi_request_vars_isset("messageText") == TRUE) {
        return ccgi_request_vars_read("messageText");
    }
    return ccgi_cookie_vars_isset("message") ? ccgi_cookie_vars_read("message") : DEFAULT_MESSAGE;
}

static void
start_html(void) {
    ccgi_echoln("<HTML>");
    ccgi_echoln("<HEAD>");
    ccgi_echoln("<TITLE>%s</TITLE>", TITLE);
    ccgi_echoln("<STYLE>");
    ccgi_echoln(".messages {");
    ccgi_echoln("width: 300px;");
    ccgi_echoln("height: 120px;");
    ccgi_echoln("}");
    ccgi_echoln(".name-email {");
    ccgi_echoln("width: 200px;");
    ccgi_echoln("}");
    ccgi_echoln(".submits {");
    ccgi_echoln("width: 100px;");
    ccgi_echoln("border: 1;");
    ccgi_echoln("border-radius: 2px 2px 2px 2px;");
    ccgi_echoln("color:color;");
    ccgi_echoln("cursor: pointer;");
    ccgi_echoln("display: inline-block;");
    ccgi_echoln("font-size: 10px;");
    ccgi_echoln("font-weight: bold;");
    ccgi_echoln("line-height: 11px;");
    ccgi_echoln("margin-bottom: 0;");
    ccgi_echoln("margin-top: 10px;");
    ccgi_echoln("padding: 7px 10px;");
    ccgi_echoln("text-transform: none;");
    ccgi_echoln("transition: all 0.3s ease 0s;");
    ccgi_echoln("-moz-transition: all 0.3s ease 0s;");
    ccgi_echoln("-webkit-transition: all 0.3s ease 0s;");
    ccgi_echoln("}");
    ccgi_echoln(".errors {");
    ccgi_echoln("color: #f00;");
    ccgi_echoln("}");
    ccgi_echoln(".records {");
    ccgi_echoln("color: #404080;");
    ccgi_echoln("}");
    ccgi_echoln("</STYLE>");
    ccgi_echoln("</HEAD>");
    ccgi_echoln("<BODY>");
    ccgi_echoln("<FORM name='mainForm' action='%s' method='%s'>", ACTION, METHOD);
    ccgi_echoln("<H4>%s</H4>", TITLE);
    ccgi_echoln("<HR>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<INPUT type='submit' name='editSubmit' class='submits' value='Edit'>");
    ccgi_echoln("<INPUT type='submit' name='viewSubmit' class='submits' value='View'>");
    ccgi_echoln("<BR><BR>");
    ccgi_echoln("<HR>");
    ccgi_echoln("<BR>");
}

static void
end_html(void) {
    ccgi_echoln("</FORM>");
    ccgi_echoln("</BODY>");
    ccgi_echoln("</HTML>");
}

static void
handle_edit_submit(const char *error) {
    start_html();
    ccgi_echoln("<LABEL>Name</LABEL>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<INPUT type='input' name='nameText' class='name-email' value='%s'>", get_name());
    ccgi_echoln("<BR>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<LABEL>E-Mail</LABEL>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<INPUT type='input' name='emailText' class='name-email' value='%s'>", get_email());
    ccgi_echoln("<BR>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<LABEL>Message</LABEL>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<TEXTAREA name='messageText' class='messages'>%s</TEXTAREA>", get_message());
    ccgi_echoln("<BR>");
    ccgi_echoln("<BR>");
    ccgi_echoln("<INPUT type='submit' name='addSubmit' class='submits' value='Add'>");
    if (error) {
        ccgi_echoln("<BR>");
        ccgi_echoln("<BR>");
        ccgi_echoln("<LABEL class='errors'>%s</LABEL>", error);
    }
    end_html();
}

static void
handle_view_submit(void) {
    start_html();
    if (print_all_records() == FALSE) {
        ccgi_echoln("<LABEL class='errors'>%s</LABEL>", DATA_ERROR);
    }
    end_html();
}

static void
handle_add_submit(void) {
    if (!connect_db()) {
        handle_edit_submit(DB_ERROR);
        return;
    }
    GUEST_REC *new_rec_ptr = new_guest_rec(get_name(), get_email(), get_message());
    if (!add_record(new_rec_ptr)) {
        handle_edit_submit(DATA_ERROR);
    }
    disconnect_db();
    handle_edit_submit(NULL);
}

static void
default_handler(void) {
    handle_edit_submit(NULL);
}

static CCGI_BOOL
connect_db(void) {
    dbm_ptr = dbm_open(DB_FILE_NAME, O_RDWR | O_CREAT, 0666);
    return dbm_ptr ? TRUE : FALSE;
}

static void
disconnect_db(void) {
    dbm_close(dbm_ptr);
}

static CCGI_BOOL
add_record(GUEST_REC *rec_ptr) {
    if (rec_ptr == NULL || rec_ptr->name == NULL || strlen(rec_ptr->name) == 0) {
        return FALSE;
    }
    datum key_datum, rec_datum;
    key_datum.dptr = (void *) rec_ptr->name;
    key_datum.dsize = (sizeof (char) * NAME_SIZE);
    rec_datum.dptr = (void *) rec_ptr;
    rec_datum.dsize = sizeof (GUEST_REC);
    int rc = dbm_store(dbm_ptr, key_datum, rec_datum, DBM_REPLACE);
    return (rc != 0) ? FALSE : TRUE;
}

static CCGI_BOOL
print_all_records(void) {
    if (!connect_db()) {
        ccgi_echoln("***ERROR: connection error!");
        exit(EXIT_FAILURE);
    }
    datum key_datum, rec_datum;
    GUEST_REC *rec_ptr = NULL;
    int no = 1;
    for (key_datum = dbm_firstkey(dbm_ptr); key_datum.dptr; key_datum = dbm_nextkey(dbm_ptr)) {
        rec_datum = dbm_fetch(dbm_ptr, key_datum);
        rec_ptr = (GUEST_REC *) rec_datum.dptr;
        ccgi_echoln("<LABEL class='records'>#%d %s (%s):</LABEL><BR>",
				no,
                rec_ptr->name,
                rec_ptr->email);
        ccgi_echoln("<P class='records'>");
        ccgi_echoln("<textarea class='messages'>%s</textarea>", (rec_ptr->message));
        ccgi_echoln("</P>");
        ccgi_echoln("<HR>");
        ccgi_echoln("<BR>");
        ++no;
    }
    disconnect_db();
    return TRUE;
}

int
ccgi_process_request(void) {
    update_cookies();
    ccgi_header_send_text_html();
    if (ccgi_submit_clicked("editSubmit")) {
        handle_edit_submit(NULL);
    } else if (ccgi_submit_clicked("addSubmit")) {
        handle_add_submit();
    } else if (ccgi_submit_clicked("viewSubmit")) {
        handle_view_submit();
    } else {
        default_handler();
    }
    return 0;
}
