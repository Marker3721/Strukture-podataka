#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person{
	char name[20];
	char surname[20];
	int day, month, year;
	struct Person* next;
}Person;

Person* add_in_front(Person*);
Person* delete_from_list(Person*, char* surname);
Person* add_to_end(Person*);
Person* create();
int find_in_list(Person*, char*);
int print_list(Person*);

int main() {
	int choice;
	int status = 0;
	char sur[20];
	Person* head = NULL;
	do {
		printf("Izaberite funkciju (6 za izlaz):\n");
		printf("1 - Dodaj na pocetak\n"); printf("2 - Dodaj na kraj\n"); printf("3 - Izbrisi iz liste\n"); printf("4 - Pronadi\n"); printf("5 - Ispisi listu\n"); scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			head = add_in_front(head);
			break;
		case 2:
			head = add_to_end(head);
			break;
		case 3:
			printf("Unesite prezime za izbrisati: "); scanf("%19s", sur);
			if (delete_from_list(head, sur) == NULL) {
				printf("Greska! Nema upisanih ljudi!\n");
				return 1;
			}
			break;
		case 4:
			printf("Unesite prezime za pronaci: "); scanf("%19s", sur);
			if (find_in_list(head, sur) == 1) {
				printf("Greska! Nema takve osobe!\n");
				return 1;
			}
			break;
		case 5:
			status = print_list(head);
			return status;
			break;
		case 6:
			break;
		default:
			printf("Niste unijeli pravilan broj!\n");
			return 1;
			break;
		}
	} while (choice != 6);

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
		return NULL;
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

	while (temp != NULL && strcmp(temp->surname, surname) != 0) {
		previous = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("Greska! Ne postoji osoba s tim prezimenom!\n");
		free(temp);
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
		return 1;
	}
	Person* temp = head;

	while (temp != NULL) {
		if (strcmp(temp->surname, surname) == 0) {
			printf("Pronadena je osoba: %s %s\trodena: %02d %02d %04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
			return 0;
		}
		temp = temp->next;
	}
	return 1;
}
