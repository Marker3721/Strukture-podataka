#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_name_len 100

typedef struct _dir {
	char name[max_name_len];
	struct _dir* child; 
	struct _dir* sibling;
	struct _dir* parent; 
} Dir;

Dir* createDir(const char* name, Dir* parent);
int md(Dir* current);
Dir* cd(Dir* current);
Dir* cd_up(Dir* current);
int dir(Dir* current);
int freeTree(Dir* dir);

int main() {
	Dir* root = createDir("root", NULL);
	Dir* current = root;
	int choice;


	do {
		printf("1 - md\n2 - cd dir\n3 - cd..\n4 - dir\n5 - izlaz\n");
		scanf(" %d", &choice);

		switch (choice) {
		case 1:
			md(current);
			break;
		case 2:
			current = cd(current);
			break;
		case 3:
			current = cd_up(current);
			break;
		case 4:
			dir(current);
			break;
		case 5:
			freeTree(root);
			break;
		default:
			printf("Error");
		}
	} while (choice != 5);

	return 0;
}

Dir* createDir(const char* name, Dir* parent) {
	Dir* d = (Dir*)malloc(sizeof(Dir));
	if (!d) {
		printf("Error");
		return 1;
	}
	strcpy(d->name, name);
	d->child = NULL;
	d->sibling = NULL;
	d->parent = parent;
	return d;
}

int md(Dir* current) {
	char name[max_name_len];
	printf("Unesite ime novog direktorija: ");
	scanf(" %s", name);


	Dir* newDir = createDir(name, current);


	if (current->child == NULL) {
		current->child = newDir;
	}
	else {
		Dir* temp = current->child;
		while (temp->sibling)
			temp = temp->sibling;
		temp->sibling = newDir;
	}
	return 0;
}

Dir* cd(Dir* current) {
	char name[max_name_len];
	scanf(" %s", name);

	Dir* temp = current->child;
	while (temp) {
		if (strcmp(temp->name, name) == 0)
			return temp;
		temp = temp->sibling;
	}

	return current;
}


Dir* cd_up(Dir* current) {
	if (current->parent)
		return current->parent;
	return current;
}

int dir(Dir* current) {
	Dir* temp = current->child;
	if (!temp) {
		printf("Direktorij je prazan.\n");
		return 1;
	}
	while (temp) {
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}
	return 0;
}
int freeTree(Dir* dir) {
	if (!dir) {
		return 1;
	}
	freeTree(dir->child);
	freeTree(dir->sibling);
	free(dir);
	return 0;
}