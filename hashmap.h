#ifndef __HASH_T__
#define __HASH_T__

typedef struct Store hashmap;

hashmap *make__hashmap(int hash__type, void (*printer)(void *), void (*destroy)(void *));

int insert__hashmap(hashmap *hash__m, char *key, void *value);

int print__hashmap(hashmap *hash__m);

int destroy__hashmap(hashmap *hash);

#endif