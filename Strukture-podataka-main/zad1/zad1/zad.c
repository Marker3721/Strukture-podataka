#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char ime[10];
	char prezime[10];
	int apsolutniBrojBodova;
	float relativniBrojBodova;
}_Student;

int redci(FILE* dat) {
	int br = 0;
	char l[100];
	while (fgets(l, 100, dat)) {
		br++;
	}
	return br;
}

_Student* upis(_Student* s, FILE* dat, int n) {
	for (int i = 0; i < n; i++) {
		fscanf(dat, "%s %s %d", s[i].ime, s[i].prezime, &s[i].apsolutniBrojBodova);
		s[i].relativniBrojBodova = ((float)s[i].apsolutniBrojBodova / 100) * 100;
	}
	return s;
}

void ispis(_Student* s, int n) {
	for (int i = 0; i < n; i++) {
		printf("%s\t%s\t%d\t%.2f", s[i].ime, s[i].prezime, s[i].apolutniBrojBodova, s[i].relativniBrojBodova);
	}
}

int main() {
	int n;
	FILE* dat = fopen("zad1File.txt", "r");
	if (dat == NULL) {
		printf("ERROR pri otvaranju datoteke");
		return 1;
	}

	n = redci(dat);
	rewind(fp);

	_Student* s = malloc(n*sizeof(_Student));
	s = upis(s, dat, n);
	ispis(s, n);
	return 0;
}




