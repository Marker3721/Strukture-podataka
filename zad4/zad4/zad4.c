#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 200
#define FILEPATH "tekst.txt"

typedef struct Stack {
	float variable;
	struct Stack* next;
}Stack;

Stack* create(float);
Stack* push(Stack*, float);
Stack* pop(Stack*, float*, float*);
char* remove_spaces(char*);

int main() {
	int counter = 0;
	float op1, op2, result;
	char buffer[MAX];
	char* clean = remove_spaces(buffer);
	Stack* head = NULL;

	while (*(clean + counter) != '\0') {
		char var_op = *(clean + counter);
		if (var_op >= '0' && var_op <= '9') {
			head = push(head, (float)(var_op - '0'));
		}
		else {
			switch (var_op) {
			case '+': {
				head = pop(head, &op1, &op2);
				result = op1 + op2;
				head = push(head, result);
				break;
			}
			case '-': {
				head = pop(head, &op1, &op2);
				result = op1 - op2;
				head = push(head, result);
				break;
			}
			case '*': {
				head = pop(head, &op1, &op2);
				result = op1 * op2;
				head = push(head, result);
				break;
			}
			case '/': {
				head = pop(head, &op1, &op2);
				if (op2 == 0) {
					printf("Greska! Dijeljenje sa 0!\n");
					return 1;
				}
				result = op1 / op2;
				head = push(head, result);
				break;
			}

			default:
				printf("Znak nije prepoznat!\n");
				break;
			}
		}
		counter++;
	}
	if (head == NULL) {
		printf("Greska! Prazan stog!\n");
	}
	else if (head->next != NULL) {
		printf("Greska! Na stogu ima više od jednog elementa!\n");
	}
	else {
		printf("\nRezultat: %.1f\n", head->variable);
	}
}

char* remove_spaces(char* buffer) {
	int i, j;
	char* real_buffer = (char*)malloc(200 * sizeof(char));
	if (!real_buffer) {
		printf("Greska u alociranju!\n");
		return NULL;
	}
	FILE* f1 = fopen(FILEPATH, "r");
	if (!f1) {
		printf("Greska u otvaranju datoteke!\n");
		return NULL;
	}
	fgets(buffer, MAX, f1);

	for (i = 0, j = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] != '\n' && buffer[i] != ' ') {
			real_buffer[j] = buffer[i];
			j++;
		}
	}
	*(real_buffer + j) = '\0';
	fclose(f1);

	return real_buffer;
}

Stack* create(float var) {
	Stack* new_var = (Stack*)malloc(sizeof(Stack));
	if (!new_var) {
		printf("Greska u alokaciji!\n");
		return NULL;
	}
	new_var->next = NULL;
	new_var->variable = var;

	return new_var;
}

Stack* push(Stack* head, float var) {
	Stack* new_var = create(var);
	if (!new_var) {
		printf("Greska u kreiranju osobe!\n");
		return NULL;
	}

	if (head == NULL) {
		return new_var;
	}

	new_var->next = head;
	return new_var;
}

Stack* pop(Stack* head, float* op1, float* op2) {
	if (head == NULL || head->next == NULL) {
		printf("Nedovoljno elemenata za racunanje!\n");
		exit(1);
	}
	Stack* temp = head;

	*op2 = head->variable;
	head = head->next;
	free(temp);

	*op1 = head->variable;
	temp = head;
	head = head->next;
	free(temp);

	return head;
}