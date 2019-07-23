/*** ccgi_str.c: Helper string routines
 */

static pthread_mutex_t smtx = PTHREAD_MUTEX_INITIALIZER;

// converts a hex character to its integer value
static char
shex2int(char);
// converts an integer value to its hex character
static char
sint2hex(char);
// constructor for an empty string
static char *
sempty(void);
// constructor for a new string initialized with given value
static char *
snew(char *);
// duplicates given string
static char *
sdupl(char *);
// appends character to string
static char *
saddc(char *, char);
// appends one string to another
static char *
sadds(char *, char *);
// returns position of a character in a given string
static int
sindex(char *, char);
// trims given string from the left
static char *
sltrim(char *);
// decodes URL-encoded string
static char *
sunesc(char *);
// applies URL-encoding to string
static char *
sesc(char *);
// extracts key from 'key=value' pair
static char *
sextrkey(char *);
// extracts value from 'key=value' pair
static char *
sextrval(char *);
// saves string to file
static int
ssave(char *, const char *);
// reads file to string
static char *
sload(const char *);

static char
shex2int(char c)
{
    return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
}

static char
sint2hex(char val)
{
    static char hex[] = "0123456789abcdef";
    return hex[val & 15];
}

static char *
sempty(void)
{
    return "";
}

static char *
snew(char *val)
{
    if (!val) {
        return NULL;
    }
    size_t sz = sizeof (char) * (strlen(val) + 1);
    char *dst = (char *) malloc(sz);
    if (!dst) {
        CCGI_EXIT("***ERROR: malloc() failed");
    }
    strcpy(dst, val);
    return dst;
}

static char *
sdupl(char *s)
{
    char *dst = snew(s);
    return dst;
}

static char *
saddc(char *s, char c)
{
    pthread_mutex_lock(&smtx);
    int cnt = !s || !strlen(s) ? 1 : strlen(s) + 1;
    char *dst = (cnt == 1) ? (char *) malloc(sizeof (char) * (cnt + 1)) : (char *) realloc(s, sizeof (char) * (cnt + 1));
    if (!dst) {
        CCGI_EXIT("***ERROR: malloc() failed");        
    }
    dst[cnt] = 0;
    dst[cnt - 1] = c;
    pthread_mutex_unlock(&smtx);
    return dst;
}

static char *
sadds(char *dst, char *src)
{
    if (!src || !strlen(src)) {
        return dst;
    }
    register char *s = dst;
    int i;
    char c;
    for (i = 0; i < strlen(src); i++) {
        c = src[i];
        s = saddc(s, c);
    }
    return s;
}

static int
sindex(char *s, char c)
{
    if (!s || !strlen(s)) {
        return -1;
    }
    int i = -1;
    register char *p = s;
    while (*p) {
        ++i;
        if (*p == c) {
            return i;
        }
        p++;
    }
    return -1;
}

static char *
sltrim(char *s)
{
    pthread_mutex_lock(&smtx);
    if (!s || !strlen(s)) {
        return s;
    }
    register char *p = s;
    while (isspace(*p)) {
        p++;
    }
    pthread_mutex_unlock(&smtx);
    return p;
}

static char *
sunesc(char *s)
{
    pthread_mutex_lock(&smtx);
    char *p = s;
    char *bfr = malloc(strlen(s) + 1);
    char *bfrp = bfr;
    while (*p) {
        if (*p == '%') {
            if (p[1] && p[2]) {
                *bfrp++ = shex2int(p[1]) << 4 | shex2int(p[2]);
                p += 2;
            }
        } else if (*p == '+') {
            *bfrp++ = ' ';
        } else {
            *bfrp++ = *p;
        }
        p++;
    }

    *bfrp = '\0';
    pthread_mutex_unlock(&smtx);
    return bfr;

}

static char *
sesc(char *s)
{
    pthread_mutex_lock(&smtx);
    char *p = s;
    char *bfr = malloc(strlen(s) * 3 + 1);
    if (!bfr) {
        CCGI_EXIT("***ERROR: malloc() failed");        
    }
    char *bfrp = bfr;
    while (*p) {
        if (isalnum(*p) || *p == '-' || *p == '_' || *p == '.' || *p == '~')
            *bfrp++ = *p;
        else if (*p == ' ')
            *bfrp++ = '+';
        else {
            *bfrp++ = '%';
            *bfrp++ = sint2hex(*p >> 4);
            *bfrp++ = sint2hex(*p & 15);
        }
        p++;
    }
    *bfrp = '\0';
    pthread_mutex_unlock(&smtx);
    return bfr;
}

static char *
sextrkey(char *s)
{
    if (!s) {
        return NULL;
    }
    int end = sindex(s, '=');
    if (end <= 0) {
        return NULL;
    }
    pthread_mutex_lock(&smtx);
    char *key = sempty();
    int i;
    for (i = 0; i < end; i++) {
        key = saddc(key, s[i]);
    }
    pthread_mutex_unlock(&smtx);
    return sdupl(sltrim(sunesc(key)));
}

static char *
sextrval(char *s)
{
    if (!s) {
        return NULL;
    }
    int start = sindex(s, '=');
    if (start == -1) {
        return NULL;
    }
    if (start == strlen(s) - 1) {
        return "";
    }
    pthread_mutex_lock(&smtx);
    register char *cp = strchr(s, '=') + 1;
    char *val = sempty();
    while (*cp) {
        val = saddc(val, *cp);
        cp++;
    }
    pthread_mutex_unlock(&smtx);
    return sdupl(sunesc(val));
}

static int
ssave(char *s, const char *fname)
{
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        return -1;
    }
    pthread_mutex_lock(&smtx);
    int n = fprintf(fp, "%s", s);
    fclose(fp);
    fp = NULL;
    pthread_mutex_unlock(&smtx);
    return n;
}

static char *
sload(const char *fname)
{
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        return NULL;
    }
    pthread_mutex_lock(&smtx);
    char ch;
    char *s = sempty();
    while (((ch = fgetc(fp)) != EOF)) {
        s = saddc(s, ch);
    }
    fclose(fp);
    fp = NULL;
    pthread_mutex_unlock(&smtx);
    return sdupl(s);
}