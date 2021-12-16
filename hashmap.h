#ifndef __HASH_T__
#define __HASH_T__

typedef struct ReturnHashmap { // used only for type 1
	void **payload;
	int payload__length;
} hashmap__response;

typedef struct Store hashmap;

hashmap *make__hashmap(int hash__type, void (*printer)(void *), void (*destroy)(void *));

void *get__hashmap(hashmap *hash__m, void *key);

int print__hashmap(hashmap *hash__m);

int delete__hashmap(hashmap *hash__m, void *key);

int deepdestroy__hashmap(hashmap *hash);

int insert__hashmap(hashmap *hash__m, void *key, void (*printKey)(void *), int (*compareKey)(void *, void *), void *value);

#endif