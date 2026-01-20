#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H 11

typedef struct _City {
    char name[64];
    int pop;
    struct _City* l;
    struct _City* r;
} City;

typedef struct _Country {
    char name[64];
    char file[64];
    City* cities;
    struct _Country* next;
} Country;

typedef struct {
    Country* t[H];
} Hash;

static int hash(const char* s);
static int parseCountry(char* l, char* n, char* f);
static int parseCity(char* l, char* n, int* p);

static int insertCountry(Hash* h, const char* n, const char* f);
static int insertCity(City** r, const char* n, int p);

static int loadCities(Country* c);
static int build(Hash* h, const char* f);

static int printCities(City* r);
static int printAbove(City* r, int p);
static int print(Hash* h);

static int find(Hash* h, const char* n, Country** out);

static int freeCities(City* r);
static int freeHash(Hash* h);

int main(void) {
    Hash h;
    Country* f = NULL;
    char q[64];
    int p;

    build(&h, "drzave.txt");
    print(&h);

    scanf("%s", q);
    scanf("%d", &p);

    find(&h, q, &f);
    if (f) printAbove(f->cities, p);

    freeHash(&h);
    return 0;
}

static int hash(const char* s) {
    int i, sum = 0;
    for (i = 0;i < 5 && s[i];i++) sum += s[i];
    return sum % H;
}

static int parseCountry(char* l, char* n, char* f) {
    return sscanf(l, "%s %s", n, f) == 2 ? 0 : -1;
}

static int parseCity(char* l, char* n, int* p) {
    return sscanf(l, " %[^,], %d", n, p) == 2 ? 0 : -1;
}

static int insertCountry(Hash* h, const char* n, const char* f) {
    int k = hash(n);
    Country* c = malloc(sizeof(Country));
    Country* cur = h->t[k];
    Country* prev = NULL;

    strcpy(c->name, n);
    strcpy(c->file, f);
    c->cities = NULL;
    c->next = NULL;

    while (cur && strcmp(cur->name, n) < 0) {
        prev = cur; cur = cur->next;
    }

    if (!prev) { c->next = h->t[k]; h->t[k] = c; }
    else { c->next = cur; prev->next = c; }

    return 0;
}

static int insertCity(City** r, const char* n, int p) {
    if (!*r) {
        *r = malloc(sizeof(City));
        strcpy((*r)->name, n);
        (*r)->pop = p;
        (*r)->l = (*r)->r = NULL;
        return 0;
    }
    if (p < (*r)->pop || (p == (*r)->pop && strcmp(n, (*r)->name) < 0))
        return insertCity(&(*r)->l, n, p);
    return insertCity(&(*r)->r, n, p);
}

static int loadCities(Country* c) {
    FILE* f = fopen(c->file, "r");
    char l[128], n[64];
    int p;
    if (!f) return -1;

    while (fgets(l, 128, f))
        if (!parseCity(l, n, &p)) insertCity(&c->cities, n, p);

    fclose(f);
    return 0;
}

static int build(Hash* h, const char* f) {
    FILE* in = fopen(f, "r");
    char l[128], n[64], ff[64];
    int i;

    for (i = 0;i < H;i++) h->t[i] = NULL;

    while (fgets(l, 128, in))
        if (!parseCountry(l, n, ff)) insertCountry(h, n, ff);

    fclose(in);

    for (i = 0;i < H;i++) {
        Country* c = h->t[i];
        while (c) { loadCities(c); c = c->next; }
    }
    return 0;
}

static int printCities(City* r) {
    if (!r) return 0;
    printCities(r->l);
    printf("%s %d\n", r->name, r->pop);
    printCities(r->r);
    return 0;
}

static int print(Hash* h) {
    int i;
    Country* c;
    for (i = 0;i < H;i++) {
        c = h->t[i];
        while (c) {
            printf("%s\n", c->name);
            printCities(c->cities);
            c = c->next;
        }
    }
    return 0;
}

static int find(Hash* h, const char* n, Country** out) {
    int k = hash(n);
    Country* c = h->t[k];
    *out = NULL;
    while (c) {
        if (!strcmp(c->name, n)) { *out = c; return 0; }
        c = c->next;
    }
    return 0;
}

static int printAbove(City* r, int p) {
    if (!r) return 0;
    printAbove(r->l, p);
    if (r->pop > p) printf("%s %d\n", r->name, r->pop);
    printAbove(r->r, p);
    return 0;
}

static int freeCities(City* r) {
    if (!r) return 0;
    freeCities(r->l);
    freeCities(r->r);
    free(r);
    return 0;
}

static int freeHash(Hash* h) {
    int i;
    Country* c, * n;
    for (i = 0;i < H;i++) {
        c = h->t[i];
        while (c) {
            n = c->next;
            freeCities(c->cities);
            free(c);
            c = n;
        }
    }
    return 0;
}