#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

typedef struct ll_def {
	struct ll_def *next;
	
	char *key;
	int max__arrLength, arrIndex; // only for hash type 2
	int isArray;
	void *ll_meat; // single value pointer
				   // for hash__type = 0 or 1
				   // or array for
				   // hash__type = 2
} ll_main_t;

// will store a single hashmap
struct Store {
	int hashmap__size;
	// takes a type of hashmap
	// 0: ignore duplicate keys
	// 1: replace value
	// 2: append to growing value list
	int hash__type;
	ll_main_t **map;

	// used for printing the hashmap values
	void (*printer)(void *);
	// destroying hashmap values
	void (*destroy)(void *);
};

const int MAX_BUCKET_SIZE = 5;
const int START_SIZE = 1023; // how many initial buckets in array

unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}


// define some linked list functions (see bottom of file for function write outs):
ll_main_t *ll_makeNode(char *key, void *value, int hash__type);
int ll_insert(ll_main_t *node, char *key, void *payload, int hash__type, void (*destroy)(void *));

ll_main_t *ll_next(ll_main_t *curr);

int ll_print(ll_main_t *curr, void (*printer)(void *));

int ll_isolate(ll_main_t *node);
int ll_destroy(ll_main_t *node, void (destroyObjectPayload)(void *));


hashmap *make__hashmap(int hash__type, void (*printer)(void *), void (*destroy)(void *)) {
	hashmap *newMap = (hashmap *) malloc(sizeof(hashmap));

	newMap->hash__type = hash__type;
	newMap->hashmap__size = START_SIZE;

	// define needed input functions
	newMap->printer = printer;
	newMap->destroy = destroy;

	newMap->map = (ll_main_t **) malloc(sizeof(ll_main_t *) * newMap->hashmap__size);

	for (int i = 0; i < newMap->hashmap__size; i++) {
		newMap->map[i] = NULL;
	}

	return newMap;
}

// take a previous hashmap and double the size of the array
// this means we have to take all the inputs and calculate
// a new position in the new array size
// within hash__m->map we can access each of the linked list pointer
// and redirect them since we store the keys
int re__hashmap(hashmap *hash__m) {
	// define the sizes / updates to size:
	int old__mapLength = hash__m->hashmap__size;
	int new__mapLength = hash__m->hashmap__size * 2;

	// create new map (twice the size of old map (AKA hash__m->map))
	ll_main_t **new__map = (ll_main_t **) malloc(sizeof(ll_main_t *) * new__mapLength);

	for (int set__newMapNulls = 0; set__newMapNulls < new__mapLength; set__newMapNulls++)
		new__map[set__newMapNulls] = NULL;

	int new__mapPos = 0;

	for (int old__mapPos = 0; old__mapPos < old__mapLength; old__mapPos++) {
		// look at each bucket
		// if there is contents
		while (hash__m->map[old__mapPos]) { // need to look at each linked node
			// recalculate hash
			new__mapPos = hash(hash__m->map[old__mapPos]->key) % new__mapLength;

			// store the node in temporary storage
			ll_main_t *currNode = hash__m->map[old__mapPos];

			// extract currNode from old map (hash__m->map)
			hash__m->map[old__mapPos] = ll_next(currNode); // advance root
			ll_isolate(currNode); // isolate old root

			// defines the linked list head in the new map
			ll_main_t *new__mapBucketPtr = new__map[new__mapPos];

			// if there is one, we have to go to the tail
			if (new__mapBucketPtr) {

				while (new__mapBucketPtr->next) new__mapBucketPtr = ll_next(new__mapBucketPtr);

				new__mapBucketPtr->next = currNode;
			} else
				new__map[new__mapPos] = currNode;
		}
	}

	free(hash__m->map);
	hash__m->map = new__map;
	hash__m->hashmap__size = new__mapLength;

	return 0;
}

int insert__hashmap(hashmap *hash__m, char *key, void *value) {
	int mapPos = hash(key) % hash__m->hashmap__size;
	int bucketLength = 0; // counts size of the bucket at mapPos


	// see if there is already a bucket defined at mapPos
	if (hash__m->map[mapPos])
		bucketLength = ll_insert(hash__m->map[mapPos], key, value, hash__m->hash__type, hash__m->destroy);
	else
		hash__m->map[mapPos] = ll_makeNode(key, value, hash__m->hash__type);

	// if bucketLength is greater than an arbitrary amount,
	// need to grow the size of the hashmap (doubling)
	if (bucketLength >= MAX_BUCKET_SIZE)
		re__hashmap(hash__m);

	return 0;
}

int print__hashmap(hashmap *hash__m) {
	for (int i = 0; i < hash__m->hashmap__size; i++) {
		if (hash__m->map[i]) {
			printf("Added a value? %d ", i);
			ll_print(hash__m->map[i], hash__m->printer);
			printf("\n");
		}
	}
}

int destroy__hashmap(hashmap *hash) {
	// destroy linked list children
	for (int i = 0; i < hash->hashmap__size; i++) {
		if (hash->map[i]) {
			ll_destroy(hash->map[i], hash->destroy);
		}
	}

	// destroy map
	free(hash->map);
	free(hash);

	return 0;
}


ll_main_t *ll_makeNode(char *key, void *newValue, int hash__type) {
	ll_main_t *new__node = (ll_main_t *) malloc(sizeof(ll_main_t));

	new__node->isArray = 0;
	new__node->next = NULL;
	new__node->key = key;
	new__node->ll_meat = newValue;

	return new__node;
}

/*
	for hash__type = 1
		takes a linked list node value ptr
		and replaces the value with
		updated value
*/
int ll_specialUpdateIgnore(void *ll_oldVal, char *newKey, void *newValue, void (*destroy)(void *)) {
	// free new key since we already
	// have the key stored
	free(newKey);

	// clean up previous info at this pointer
	destroy(ll_oldVal);

	// update
	ll_oldVal = newValue;

	return 0;
}

// takes the ll_pointer->ll_meat and doubles
// size of current array
int ll_resizeArray(ll_main_t *ll_pointer) {
	// declare new array
	void **new__arrayPtr = malloc(sizeof(void *) * ll_pointer->max__arrLength * 2);

	// copy values over
	for (int copyVal = 0; copyVal < ll_pointer->max__arrLength; copyVal++) {
		new__arrayPtr[copyVal] = (void *) ((void **) ll_pointer->ll_meat)[copyVal];
	}

	// free old array pointer
	free(ll_pointer->ll_meat);

	// update to new meat
	ll_pointer->ll_meat = new__arrayPtr;

	return 0;
}

/*
	for hash__type = 2
		takes linked list pointer
		- first makes sure that ll_pointer->ll_meat
		is an array, if not it creates and array
		- then appends value to end of array
*/
int ll_specialUpdateArray(ll_main_t *ll_pointer, char *key, void *newValue) {
	free(key);

	if (!ll_pointer->isArray) { // need to create an array
		void *ll_tempMeatStorage = ll_pointer->ll_meat;

		// update settings for this pointer
		ll_pointer->max__arrLength = 8;
		ll_pointer->arrIndex = 0;
		ll_pointer->isArray = 1;

		ll_pointer->ll_meat = (void **) malloc(sizeof(void *) * ll_pointer->max__arrLength);
		((void **) (ll_pointer->ll_meat))[0] = ll_tempMeatStorage;

		for (int memSet = 1; memSet < ll_pointer->arrIndex; memSet++)
			((void **) (ll_pointer->ll_meat))[memSet] = NULL;
	}

	// add new value
	ll_pointer->arrIndex++;
	((void **) (ll_pointer->ll_meat))[ll_pointer->arrIndex] = newValue;

	if (ll_pointer->arrIndex == ll_pointer->max__arrLength - 1)
		ll_resizeArray(ll_pointer);

	return 0;
}

// finds the tail and appends
int ll_insert(ll_main_t *crawler__node, char *key, void *newValue, int hash__type, void (*destroy)(void *)) {

	int bucket_size = 1, addedPayload = 0;

	// search through the entire bucket
	// (each node in this linked list)
	while (crawler__node->next) {
		// found a duplicate (only matters
		// for hash__type == 1 or 2)
		if (strcmp(crawler__node->key, key) == 0) {
			if (hash__type == 1) {
				ll_specialUpdateIgnore(crawler__node->ll_meat, key, newValue, destroy);
				addedPayload = 1;
			} else if (hash__type == 2) {
				ll_specialUpdateArray(crawler__node, key, newValue);
				addedPayload = 1;
			}
		}

		crawler__node = ll_next(crawler__node);
		bucket_size++;
	}

	if (strcmp(crawler__node->key, key) == 0) {
		if (hash__type == 1) {
			ll_specialUpdateIgnore(crawler__node->ll_meat, key, newValue, destroy);
			addedPayload = 1;
		} else if (hash__type == 2) {
			ll_specialUpdateArray(crawler__node, key, newValue);
			addedPayload = 1;
		}
	}

	if (addedPayload == 0) {
		crawler__node->next = ll_makeNode(key, newValue, hash__type);
	}

	// return same head
	return bucket_size;
}

ll_main_t *ll_next(ll_main_t *curr) {
	return curr->next;
}

int ll_printNodeArray(ll_main_t *curr, void (*printer)(void *)) {
	for (int printVal = 0; printVal < curr->arrIndex + 1; printVal++) {
		printer(((void **) curr->ll_meat)[printVal]);
	}

	return 0;
}

int ll_print(ll_main_t *curr, void (*printer)(void *)) {
	printf("\n\tLL %s\n", curr->key);
	if (curr->isArray)
		ll_printNodeArray(curr, printer);
	else
		printer(curr->ll_meat);

	while (curr = ll_next(curr)) {
		printf("\tLL\n");
		if (curr->isArray)
			ll_printNodeArray(curr, printer);
		else
			printer(curr->ll_meat);
	}

	return 0;
}

int ll_isolate(ll_main_t *node) {
	node->next = NULL;

	return 0;
}

int ll_destroy(ll_main_t *node, void (destroyObjectPayload)(void *)) {
	ll_main_t *node_nextStore;

	do {
		free(node->key);
		if (node->isArray) {
			for (int destroyVal = 0; destroyVal < node->arrIndex + 1; destroyVal++)
				destroyObjectPayload(((void **)node->ll_meat)[destroyVal]);

			free(node->ll_meat);
		} else
			destroyObjectPayload(node->ll_meat);

		node_nextStore = node->next;
		free(node);
	} while (node = node_nextStore);

	return 0;
}