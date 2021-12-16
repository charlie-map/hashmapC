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

void printCharKey(void *characters) {
	printf("%s", (char *) characters);
}

int compareCharKey(void *characters, void *otherValue) {
	return strcmp((char *) characters, (char *) otherValue) == 0;
}

void printIntKey(void *integer) {
	printf("%d", *((int *) integer));
}

int compareIntKey(void *integer, void *otherValue) {
	return *((int *) integer) == *((int *) otherValue);
}

int main() {
	hashmap *mymap = make__hashmap(0, printPayload, destroyPayload);

	// normal insert
	char *randomChar1 = malloc(sizeof(char) * 9);
	strcpy(randomChar1, "try this");

	insert__hashmap(mymap, "random key", printCharKey, compareCharKey, randomChar1);
	

	// integer key insert
	int testKey = 97;

	char *randomChar2 = malloc(sizeof(char) * 21);
	strcpy(randomChar2, "this uses an integer");

	insert__hashmap(mymap, &testKey, printIntKey, compareIntKey, randomChar2);

	char *randomChar4 = malloc(sizeof(char) * 16);
	strcpy(randomChar4, "a different int");

	insert__hashmap(mymap, &testKey, printIntKey, compareIntKey, randomChar4);

	// test with manually allocated key:
	char *allocatedKey = malloc(sizeof(char) * 5);
	strcpy(allocatedKey, "hey!");

	char *randomChar3 = malloc(sizeof(char) * 6);
	strcpy(randomChar3, "nice?");

	insert__hashmap(mymap, allocatedKey, printCharKey, compareCharKey, randomChar3);

	print__hashmap(mymap);

	deepdestroy__hashmap(mymap);

	free(allocatedKey);

	return 0;
}