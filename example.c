#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashmap.h"

char *makestring(int size, int range) {
	char *query = malloc(sizeof(char) * (size + 1));

	for (int i = 0; i < size; i++) {
		query[i] = (char) ((rand() % (range + 1)) + 48);
	}

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
	hashmap *mymap = make__hashmap(1, printObjectPayload, destroyObject);

	srand(time(NULL));

	char *someKeys = "123";

	char *someRandomAllocatedValue = malloc(sizeof(char) * 5);
	strcpy(someRandomAllocatedValue, "fun!");

	insert__hashmap(mymap, someKeys, someRandomAllocatedValue);

	char *someOtherRandomAllocatedValue = malloc(sizeof(char) * 5);
	strcpy(someOtherRandomAllocatedValue, "test");

	insert__hashmap(mymap, someKeys, someOtherRandomAllocatedValue);

	char *aDifferentKey = "456";

	char *aNewRandomAllocatedValue = malloc(sizeof(char) * 5);
	strcpy(aNewRandomAllocatedValue, "play");

	insert__hashmap(mymap, aDifferentKey, aNewRandomAllocatedValue);

	print__hashmap(mymap);

	delete__hashmap(mymap, someKeys);

	printf("AFTER DELETION\n");

	print__hashmap(mymap);

	deepdestroy__hashmap(mymap);

	return 0;
}