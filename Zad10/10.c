#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _City {
    char name[128];
    int pop;
    struct _City* next;
} City;

typedef struct _Country {
    char name[128];
    char file[256];
    City* cities;
    struct _Country* left;
    struct _Country* right;
} Country;

int trim(char* s);
int parseCountryLine(const char* line, char* cname, char* cfile);
int parseCityLine(const char* line, char* city, int* pop);

int insertCountryTree(Country** root, const char* name, const char* file);
int findCountryTree(Country* root, const char* name, Country** out);

int insertCityListSorted(City** head, const char* name, int pop);
int loadCitiesIntoCountry(Country* c);

int buildTreeFromFile(const char* drzaveFile, Country** root);
int printCityList(City* head);
int printCityListAbove(City* head, int minPop);
int printCountryTree(Country* root);

int freeCityList(City* head);
int freeCountryTree(Country* root);

int main(void) {
    Country* root = NULL;

    if (buildTreeFromFile("drzave.txt", &root) != 0) {
        return 1;
    }

    printCountryTree(root);

    {
        char q[128];
        int minPop = 0;
        Country* found = NULL;

        if (scanf(" %127s", q) != 1) {
            freeCountryTree(root);
            return 1;
        }
        if (scanf("%d", &minPop) != 1) {
            freeCountryTree(root);
            return 1;
        }

        findCountryTree(root, q, &found);
        if (!found) {
            freeCountryTree(root);
            return 0;
        }
        printCityListAbove(found->cities, minPop);
    }

    freeCountryTree(root);
    return 0;
}

int trim(char* s) {
    int i = 0, j, n;
    if (!s) return -1;

    n = (int)strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1])))
        s[--n] = '\0';

    while (s[i] && isspace((unsigned char)s[i])) i++;
    if (i > 0) {
        j = 0;
        while (s[i]) s[j++] = s[i++];
        s[j] = '\0';
    }
    return 0;
}

int parseCountryLine(const char* line, char* cname, char* cfile) {
    if (!line || !cname || !cfile) return -1;
    cname[0] = 0; cfile[0] = 0;
    if (sscanf(line, " %127s %255s", cname, cfile) != 2) return -2;
    return 0;
}

int parseCityLine(const char* line, char* city, int* pop) {
    char tmp[256];
    if (!line || !city || !pop) return -1;
    tmp[0] = 0; city[0] = 0; *pop = 0;
    if (sscanf(line, " %255[^,], %d", tmp, pop) != 2) return -2;
    strncpy(city, tmp, 127);
    city[127] = '\0';
    trim(city);
    return 0;
}

int insertCountryTree(Country** root, const char* name, const char* file) {
    int cmp;
    Country* n;

    if (!root || !name || !file) return -1;

    if (!*root) {
        n = (Country*)malloc(sizeof(Country));
        if (!n) return -2;
        strncpy(n->name, name, 127); n->name[127] = '\0';
        strncpy(n->file, file, 255); n->file[255] = '\0';
        n->cities = NULL;
        n->left = NULL;
        n->right = NULL;
        *root = n;
        return 0;
    }

    cmp = strcmp(name, (*root)->name);
    if (cmp < 0) return insertCountryTree(&((*root)->left), name, file);
    if (cmp > 0) return insertCountryTree(&((*root)->right), name, file);
    return 0;
}

int findCountryTree(Country* root, const char* name, Country** out) {
    int cmp;
    if (!out) return -1;
    *out = NULL;
    if (!root || !name) return 0;

    cmp = strcmp(name, root->name);
    if (cmp == 0) { *out = root; return 0; }
    if (cmp < 0) return findCountryTree(root->left, name, out);
    return findCountryTree(root->right, name, out);
}

int insertCityListSorted(City** head, const char* name, int pop) {
    City* n;
    City* cur;
    City* prev;

    if (!head || !name) return -1;

    n = (City*)malloc(sizeof(City));
    if (!n) return -2;
    strncpy(n->name, name, 127); n->name[127] = '\0';
    n->pop = pop;
    n->next = NULL;

    prev = NULL;
    cur = *head;
    while (cur) {
        if (pop < cur->pop) break;
        if (pop == cur->pop && strcmp(name, cur->name) < 0) break;
        prev = cur;
        cur = cur->next;
    }

    if (!prev) {
        n->next = *head;
        *head = n;
    }
    else {
        n->next = cur;
        prev->next = n;
    }
    return 0;
}

int loadCitiesIntoCountry(Country* c) {
    FILE* f;
    char line[512];
    char cname[128];
    int pop;

    if (!c) return -1;
    f = fopen(c->file, "r");
    if (!f) return -2;

    while (fgets(line, sizeof(line), f)) {
        if (parseCityLine(line, cname, &pop) == 0) {
            insertCityListSorted(&(c->cities), cname, pop);
        }
    }

    fclose(f);
    return 0;
}

int buildTreeFromFile(const char* drzaveFile, Country** root) {
    FILE* f;
    char line[512];
    char cname[128], cfile[256];

    if (!drzaveFile || !root) return -1;
    f = fopen(drzaveFile, "r");
    if (!f) return -2;

    while (fgets(line, sizeof(line), f)) {
        if (parseCountryLine(line, cname, cfile) == 0) {
            insertCountryTree(root, cname, cfile);
        }
    }
    fclose(f);

    f = fopen(drzaveFile, "r");
    if (!f) return -3;

    while (fgets(line, sizeof(line), f)) {
        Country* found = NULL;
        if (parseCountryLine(line, cname, cfile) == 0) {
            findCountryTree(*root, cname, &found);
            if (found) loadCitiesIntoCountry(found);
        }
    }
    fclose(f);

    return 0;
}

int printCityList(City* head) {
    City* cur = head;
    while (cur) {
        printf("%s %d\n", cur->name, cur->pop);
        cur = cur->next;
    }
    return 0;
}

int printCityListAbove(City* head, int minPop) {
    City* cur = head;
    int any = 0;
    while (cur) {
        if (cur->pop > minPop) {
            printf("%s %d\n", cur->name, cur->pop);
            any = 1;
        }
        cur = cur->next;
    }
    if (!any) printf("0\n");
    return 0;
}

int printCountryTree(Country* root) {
    if (!root) return 0;
    printCountryTree(root->left);
    printf("%s\n", root->name);
    printCityList(root->cities);
    printCountryTree(root->right);
    return 0;
}

int freeCityList(City* head) {
    City* cur = head;
    while (cur) {
        City* nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    return 0;
}

int freeCountryTree(Country* root) {
    if (!root) return 0;
    freeCountryTree(root->left);
    freeCountryTree(root->right);
    freeCityList(root->cities);
    free(root);
    return 0;
}