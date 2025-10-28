#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR_FILE_OPENING -2
#define ERROR_ALLOCATION NULL
#define ERROR_GENERIC_STRUCT NULL
#define ERROR_GENERIC -1

typedef struct Person {
	char name[20];
	char surname[20];
	int day, month, year;
	struct Person* next;
}Person;

Person* add_in_front(Person*);
Person* add_to_end(Person*);
Person* add_in_front_of(Person*, char*);
Person* add_behind_of(Person*, char*);
Person* delete_from_list(Person*, char* surname);
Person* read_from_file(FILE*, Person*);
Person* sort_by_surname(Person* head);
Person* create();
int find_in_list(Person*, char*);
int print_list(Person*);
int print_in_file(Person*);

int main() {
	int choice;
	int status = 0;
	char sur[20];
	Person* head = NULL;
	do {
		printf("Izaberite funkciju (11 za izlaz):\n");
		printf("1 - Dodaj na pocetak\n"); printf("2 - Dodaj na kraj\n"); printf("3 - Izbrisi iz liste\n"); printf("4 - Pronadi\n"); printf("5 - Ispisi listu\n"); printf("6 - Dodaj iza clana\n"); printf("7 - Dodaj ispred clana\n"); printf("8 - Ispis u datoteku\n"); printf("9 - Upis iz datoteke\n"); printf("10 - Sortiraj listu po prezimenima\n"); scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			head = add_in_front(head);
			break;
		case 2:
			head = add_to_end(head);
			break;
		case 3:
			printf("Unesite prezime za izbrisati: "); 
			scanf("%19s", sur);
			if (delete_from_list(head, sur) == NULL) {
				return ERROR_GENERIC;
			}
			break;
		case 4:
			printf("Unesite prezime za pronaci: "); 
			scanf("%19s", sur);
			if (find_in_list(head, sur) == ERROR_GENERIC) {
				return ERROR_GENERIC;
			}
			break;
		case 5:
			status = print_list(head);
			if (status != 0)
				printf("Doslo je do greske! (kod: %d)\n", status);
			break;
		case 6:
			printf("Unesite prezime: "); 
			scanf("%19s", sur);
			head = add_behind_of(head, sur);
			break;
		case 7:
			printf("Unesite prezime: "); 
			scanf("%19s", sur);
			head = add_in_front_of(head, sur);
			break;
		case 8:
			status = print_in_file(head);
			if (status != 0)
				printf("Doslo je do greske! (kod: %d)\n", status);
			break;
		case 9:
		{
			FILE* f1 = fopen("tekst.txt", "r");
			if (f1 == NULL) {
				printf("Greska u otvaranju filea!\n");
				return ERROR_FILE_OPENING;
			}
			head = read_from_file(f1, head);
			fclose(f1);
			break;
		}
		case 10:
			head = sort_by_surname(head);
			break;
		case 11:
			break;
		default:
			printf("Niste unijeli pravilan broj!\n");
			return 1;
			break;
		}
	} while (choice != 11);

	Person* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
	return 0;
}

Person* create() {
	Person* new_person = (Person*)malloc(sizeof(Person));
	if (new_person == NULL) {
		printf("Greska u alociranju memorije!\n");
		return ERROR_ALLOCATION;
	}

	printf("Unesite ime: ");
	scanf("%19s", new_person->name);
	printf("Unesite prezime: ");
	scanf("%19s", new_person->surname);
	printf("Unesite datum rodenja (dd mm yyyy): ");
	scanf("%d %d %d", &new_person->day, &new_person->month, &new_person->year);
	new_person->next = NULL;

	return new_person;
}

Person* add_in_front(Person* head) {
	Person* person_to_add = create();
	if (person_to_add == NULL) {
		printf("Greska u stvaranju osobe!\n");
		return head;
	}

	person_to_add->next = head;
	return person_to_add;
}

Person* add_to_end(Person* head) {
	Person* person_to_add = create();
	if (person_to_add == NULL) {
		printf("Greska u stvaranju osobe!\n");
		return head;
	}
	if (head == NULL) {
		return person_to_add;
	}

	Person* temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	person_to_add->next = temp->next;
	temp->next = person_to_add;
	return head;
}

int print_list(Person* head) {
	if (head == NULL) {
		printf("Greska! Nema ljudi za ispisati!\n");
		return 1;
	}
	Person* temp = head;
	printf("%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	printf("-----------------------------------------------------------------------\n");

	do {
		printf("%-20s %-20s %02d.%02d.%04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
		temp = temp->next;
	} while (temp != NULL);

	return 0;
}

Person* delete_from_list(Person* head, char* surname) {
	Person* temp = head;
	Person* previous = NULL;
	if (head == NULL) {
		printf("Lista je prazna!\n");
		return ERROR_GENERIC_STRUCT;
	}

	while (temp != NULL && strcmp(temp->surname, surname) != 0) {
		previous = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("Greska! Ne postoji osoba s tim prezimenom!\n");
		return head;
	}

	if (previous == NULL) {
		head = temp->next;
	}
	else {
		previous->next = temp->next;
	}

	free(temp);
	return head;
}

int find_in_list(Person* head, char* surname) {
	if (head == NULL) {
		printf("Greska! Nema takve osobe!\n");
		return ERROR_GENERIC;
	}
	Person* temp = head;

	while (temp != NULL) {
		if (strcmp(temp->surname, surname) == 0) {
			printf("Pronadena je osoba: %s %s\trodena: %02d %02d %04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
			return 0;
		}
		temp = temp->next;
	}

	printf("Greska! Nema takve osobe!\n");
	return ERROR_GENERIC;
}

Person* add_in_front_of(Person* head, char* surname) {
	Person* curr = head;
	Person* prev = NULL;

	while (curr != NULL) {
		if (strcmp(curr->surname, surname) == 0) {
			Person* new_person = create();
			if (new_person == NULL) {
				return head;
			}
			if (prev == NULL) {
				new_person->next = head;
				return new_person;
			}
			prev->next = new_person;
			new_person->next = curr;
			return head;
		}
		prev = curr;
		curr = curr->next;
	}

	printf("Osoba s prezimenom %s nije pronadena!\n", surname);
	return head;
}

Person* add_behind_of(Person* head, char* surname) {
	Person* curr = head;

	while (curr != NULL) {
		if (strcmp(curr->surname, surname) == 0) {
			Person* new_person = create();
			new_person->next = curr->next;
			curr->next = new_person;
		}
		curr = curr->next;
	}

	return head;
}

int print_in_file(Person* head) {
	FILE* f1 = fopen("ispis.txt", "w");
	if (f1 == NULL) {
		printf("Greska u otvaranju filea!");
		return ERROR_FILE_OPENING;
	}

	Person* temp = head;
	fprintf(f1, "%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	fprintf(f1, "-----------------------------------------------------------------------\n");
	while (temp != NULL) {
		fprintf(f1, "%-20s %-20s %02d.%02d.%04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
		temp = temp->next;
	}

	fclose(f1);
	return 0;
}

Person* read_from_file(FILE* f1, Person* head) {
	Person* temp = head;
	char buffer[1020];
	fgets(buffer, 1020, f1);
	fgets(buffer, 1020, f1);

	if (temp != NULL) {
		while (temp->next != NULL)
			temp = temp->next;
	}

	while (1) {
		Person* newp = (Person*)malloc(sizeof(Person));
		if (newp == NULL) {
			printf("Greska u alokaciji memorije!\n");
			return head;
		}

		int read = fscanf(f1, "%19s %19s %d.%d.%d",
			newp->name, newp->surname,
			&newp->day, &newp->month, &newp->year);

		if (read != 5) {
			free(newp);
			break;
		}

		newp->next = NULL;

		if (head == NULL) {
			head = newp;
			temp = newp;
		}
		else {
			temp->next = newp;
			temp = newp;
		}
	}

	return head;
}
Person* sort_by_surname(Person* head) {
	if (head == NULL || head->next == NULL)
		return head;

	Person* i;
	Person* j;
	Person* prev = NULL;
	Person* temp = NULL;
	int swapped;

	do {
		swapped = 0;
		prev = NULL;
		i = head;

		while (i->next != NULL) {
			j = i->next;
			if (strcmp(i->surname, j->surname) > 0) {
				if (prev == NULL) {
					i->next = j->next;
					j->next = i;
					head = j;
				}
				else {
					prev->next = j;
					i->next = j->next;
					j->next = i;
				}
				swapped = 1;
				prev = j;
			}
			else {
				prev = i;
				i = i->next;
			}
		}
	} while (swapped);

	return head;
}






