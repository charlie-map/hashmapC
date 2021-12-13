#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"

char *makestring(int size, int range) {
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

	char *randomKeys = "333";

	insert__hashmap(mymap, randomKeys, makestring(0, 10, 9));
	insert__hashmap(mymap, randomKeys, makestring(0, 10, 9));

	hashmap__response *test = get__hashmap(mymap, randomKeys);

	print__hashmap(mymap);

	destroy__hashmap(mymap);

	return 0;
}