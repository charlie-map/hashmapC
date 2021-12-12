#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"

/*
	big: 0 if a decimal point should be added (0.334), 1 if not
	size: how many digits
	range: numbers to use: 1 for 0 and 1, 2 for 0, 1, and 2, etc
*/
char *makestring(int big, int size, int range) {
	char *query = malloc(sizeof(char) * (size + 1));

	for (int i = 0; i < size; i++) {
		query[i] = (char) ((rand() % (range + 1)) + 48);
	}

	if (!big)
		query[1] = '.';

	query[size] = '\0';
	return query;
}

void printObjectPayload(void *str) {
	printf ("\t\tTest %s\n", (char *)str);
}

void destroyObject(void *str) {
	free((char *)str);
}

int main() {
	hashmap *mymap = make__hashmap(2, printObjectPayload, destroyObject);

	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		insert__hashmap(mymap, makestring(1, 2, 2), makestring(0, 10, 9));
	}

	// 02102

	print__hashmap(mymap);

	destroy__hashmap(mymap);

	printf("destroyed a map?\n");

	return 0;
}