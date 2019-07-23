/*** ccgi_list.c: Helper key-value store class
 */
typedef struct list LIST;

// list for storing 'key=value' pairs

struct list {
    char *key;
    char *val;
    LIST *nextp;
};

static pthread_mutex_t lmtx = PTHREAD_MUTEX_INITIALIZER;

// constructor for a new list
static LIST *
lnew(char *, char *, LIST *);
// destructor for a list
static void
ldel(LIST *);
// returns node at given position within list
static LIST *
lnodat(LIST *, int);
// searches node in the list by key
static LIST *
lfind(LIST *, char *);
// counts nodes in the list
static int
lcount(LIST *);
// appends new 'key=value' pair to list
static LIST *
ladd(LIST *, char *, char *);
// checks if the list has node with given key
static CCGI_BOOL
lhaskey(LIST *, char *);
// reads value of the node with given key
static char *
lreadkey(LIST *, char *);
// returns key of the node at given position
static char *
lkeyat(LIST *, int);
// returns value of the node at given position
static char *
lvalat(LIST *, int);
// builds 'key=value' list from parsing the given string
static LIST *
lparse(char *, char *);

static LIST *
lnew(char *key, char *val, LIST *nextp)
{
    LIST *p = (LIST *) malloc(sizeof (LIST));
    if (!p) {
        CCGI_EXIT("***ERROR: malloc() failed");        
    }
    p->key = sdupl(key);
    p->val = val ? sdupl(val) : NULL;
    p->nextp = nextp;
    return p;
}

static void
ldel(LIST *hp)
{
    if (!hp) {
        return;
    }
    pthread_mutex_lock(&lmtx);
    LIST *prvp = NULL;
    LIST *cp = hp;
    while (cp) {
        prvp = cp;
        cp = cp->nextp;
        free(prvp);
    }
    hp = NULL;
    pthread_mutex_unlock(&lmtx);
}

static LIST *
lnodat(LIST *hp, int pos)
{
    if (!hp || pos >= lcount(hp)) {
        return NULL;
    }
    int i = -1;
    LIST *cp = hp;
    while (cp) {
        ++i;
        if (i == pos) {
            return cp;
        }
        cp = cp->nextp;
    }
    return NULL;
}

static LIST *
lfind(LIST *hp, char *key)
{
    if (!hp) {
        return NULL;
    }
    LIST *cp = hp;
    while (cp) {
        if (!strcmp(key, cp->key)) {
            return cp;
        }
        cp = cp->nextp;
    }
    return NULL;
}

static int
lcount(LIST *hp)
{
    if (!hp) {
        return 0;
    }
    register int cnt = 0;
    LIST *cp = hp;
    while (cp) {
        cnt++;
        cp = cp->nextp;
    }
    return cnt;
}

static LIST *
ladd(LIST *hp, char *key, char *val)
{
    if (!key) {
        return hp;
    }
    pthread_mutex_lock(&lmtx);
    LIST *p = lnew(key, val, NULL);
    if (!hp) {
        pthread_mutex_unlock(&lmtx);
        return p;
    }
    LIST *cp = lnodat(hp, lcount(hp) - 1);
    cp->nextp = p;
    pthread_mutex_unlock(&lmtx);
    return hp;
}

static CCGI_BOOL
lhaskey(LIST *hp, char *key)
{
    return lfind(hp, key) ? TRUE : FALSE;
}

static char *
lreadkey(LIST *hp, char *key)
{
    LIST *cp = lfind(hp, key);
    return cp ? cp->val : NULL;
}

static char *
lkeyat(LIST *hp, int pos)
{
    LIST *cp = lnodat(hp, pos);
    return cp ? cp->key : NULL;
}

static char *
lvalat(LIST *hp, int pos)
{
    LIST *cp = lnodat(hp, pos);
    return cp ? cp->val : NULL;
}

static LIST *
lparse(char *str, char *sep)
{
    if (!str || !sep) {
        return NULL;
    }
    LIST *cp = NULL;
    char *bfr = sdupl(str);
    char *tok = strtok(bfr, sep);
    while (tok) {
        cp = ladd(cp, sextrkey(tok), sextrval(tok));
        tok = strtok(NULL, sep);
    }
    return cp;
}

