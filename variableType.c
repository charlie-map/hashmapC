#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

void printPayload(void *load) {
	printf("\t\tHere's the payload %s\n", (char *) load);
}

void destroyPayload(void *load) {
	free(load);
}

void printIntegerKey(void *integer) {
	printf("%d", *((int *) integer));
}

int compareIntegerKey(void *integer, void *otherValue) {
	return *((int *) integer) == *((int *) otherValue);
}

int main() {
	hashmap *mymap = make__hashmap(0, printPayload, destroyPayload);

	/* normal insert
	char *randomChar1 = malloc(sizeof(char) * 9);
	strcpy(randomChar1, "try this");

	insert__hashmap(mymap, "random key", randomChar1);
	*/

	// integer key insert
	int testKey = 97;

	char *randomChar2 = malloc(sizeof(char) * 21);
	strcpy(randomChar2, "this uses an integer");

	insert__hashmap(mymap, &testKey, printIntegerKey, compareIntegerKey, randomChar2);

	char *randomChar4 = malloc(sizeof(char) * 16);
	strcpy(randomChar4, "a different int");

	insert__hashmap(mymap, &testKey, printIntegerKey, compareIntegerKey, randomChar4);

	/* struct key insert
	struct ValuePassChar *passByValChar = malloc(sizeof(struct ValuePassChar));
	passByValChar->node[5] = '\0';

	for (int fillChar = 0; fillChar < 5; fillChar++) {
		passByValChar->node[fillChar] = (char) fillChar;
	}

	char *randomChar3 = malloc(sizeof(char) * 12);
	strcpy(randomChar3, "uses struct");

	insert__hashmap(mymap, passByValChar, randomChar3);
	*/

	print__hashmap(mymap);

	deepdestroy__hashmap(mymap);

	return 0;
}