#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATE_LENGTH 10
#define MAX_ARTICLE_NAME_LENGTH 20
#define MAX_LINE_LENGTH 256
#define MAX_FILE_NAME_LENGTH 20

typedef struct article {
    char name[MAX_ARTICLE_NAME_LENGTH];
    int amount;
    float price;
    struct article* next;
} article;

typedef struct receipt {
    char date[MAX_DATE_LENGTH];
    article* article;
    struct receipt* next;
} receipt;

int read_all_receipts(char* filename, receipt** head);
int read_receipt(char* filename, receipt** head);
int add_receipt(receipt** head, char* date);
int add_article(article** head, char* name, int amount, float price);
int free_articles(article* head);
int free_receipts(receipt* head);
int query_article(receipt* head, char* name, char* dateStart, char* dateEnd, int* totalAmount, float* totalMoney);

int main() {
    receipt* head = NULL;

    if (read_all_receipts("racuni.txt", &head)) {
        printf("Greska pri citanju datoteke racuni.txt\n");
        return 1;
    }

    char name[MAX_ARTICLE_NAME_LENGTH];
    char dateStart[MAX_DATE_LENGTH];
    char dateEnd[MAX_DATE_LENGTH];

    printf("Upisite naziv artikla: ");
    scanf("%s", name);

    printf("Pocetni datum (YYYY-MM-DD): ");
    scanf("%s", dateStart);

    printf("Zavrsni datum (YYYY-MM-DD): ");
    scanf("%s", dateEnd);

    int totalAmount = 0;
    float totalMoney = 0;

    query_article(head, name, dateStart, dateEnd, &totalAmount, &totalMoney);

    printf("Ukupno kupljeno: %d\n", totalAmount);
    printf("Ukupno potroseno: %.2f\n", totalMoney);

    free_receipts(head);
    return 0;
}

int add_receipt(receipt** head, char* date) {
    receipt* r = malloc(sizeof(receipt));
    if (!r) {
        printf("Memory allocation error\n");
        return 1;
    }

    strncpy(r->date, date, MAX_DATE_LENGTH);
    r->article = NULL;
    r->next = NULL;

    if (*head == NULL || strcmp(date, (*head)->date) < 0) {
        r->next = *head;
        *head = r;
        return 0;
    }

    receipt* prev = *head;
    receipt* curr = (*head)->next;

    while (curr && strcmp(date, curr->date) > 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = r;
    r->next = curr;

    return 0;
}

int add_article(article** head, char* name, int amount, float price) {
    article* a = malloc(sizeof(article));
    if (!a) {
        printf("Memory allocation error\n");
        return 1;
    }

    strcpy(a->name, name);
    a->amount = amount;
    a->price = price;
    a->next = NULL;

    if (*head == NULL || strcmp(name, (*head)->name) < 0) {
        a->next = *head;
        *head = a;
        return 0;
    }

    article* prev = *head;
    article* curr = (*head)->next;

    while (curr && strcmp(name, curr->name) > 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = a;
    a->next = curr;

    return 0;
}

int read_receipt(char* filename, receipt** head) {
    FILE* f = fopen(filename, "r");
    if (!f) return 1;

    char line[MAX_LINE_LENGTH];
    char date[MAX_DATE_LENGTH];
    char name[MAX_ARTICLE_NAME_LENGTH];
    int amount;
    float price;

    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return 1;
    }

    sscanf(line, "%s", date);
    add_receipt(head, date);

    receipt* r = *head;
    while (r && strcmp(r->date, date) != 0) r = r->next;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') break;
        sscanf(line, "%s %d %f", name, &amount, &price);
        add_article(&r->article, name, amount, price);
    }

    fclose(f);
    return 0;
}

int read_all_receipts(char* filename, receipt** head) {
    FILE* f = fopen(filename, "r");
    if (!f) return 1;

    char fileName[MAX_FILE_NAME_LENGTH];

    while (fgets(fileName, sizeof(fileName), f)) {
        fileName[strcspn(fileName, "\n")] = 0;
        read_receipt(fileName, head);
    }

    fclose(f);
    return 0;
}

int query_article(receipt* head, char* name, char* dateStart, char* dateEnd, int* totalAmount, float* totalMoney) {
    *totalAmount = 0;
    *totalMoney = 0;

    receipt* r = head;
    while (r) {
        if (strcmp(r->date, dateStart) >= 0 && strcmp(r->date, dateEnd) <= 0) {
            article* a = r->article;
            while (a) {
                if (strcmp(a->name, name) == 0) {
                    *totalAmount += a->amount;
                    *totalMoney += a->amount * a->price;
                }
                a = a->next;
            }
        }
        r = r->next;
    }

    return 0;
}

int free_articles(article* head) {
    article* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

int free_receipts(receipt* head) {
    receipt* temp;
    while (head) {
        free_articles(head->article);
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
