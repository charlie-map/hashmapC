# hashmap
###### Charlie Hall

This is a simple implementation of a hashmap where a user can input a `key` along with a `value` and have constant time insertion and selection. This follows the general approach for a hashmap; uses a hash function (a bit shifting hash function), which will devide keys uniformly into buckets of values. The buckets are implemented using a built in linked list that has been altered to have functionality specifically for this approach. Since buckets are in use and no hash function has perfect hashing, the theoretical complexity for insertion and selection is `m` where `m` is the number of items in a single bucket. As seen at the top of `hashmap.c`, the char *allocatedKey = malloc(sizeof(char) * 5);
	strcpy(allocatedKey, "hey!");

	char *randomChar3 = malloc(sizeof(char) * 6);
	strcpy(randomChar3, "nice?");

	insert__hashmap(mymap, allocatedKey, randomChar3, printCharKey, compareCharKey, destroyCharKey);
variable `MAX_BUCKET_SIZE` defines how many elements can be in a single bucket before a resizing (see below) of the hashmap occurs. Each linked list node will have a `void *` pointer to whatever input is given, which means the hashmap can contain any form of information from small `char *` to major structs. The hashmap has the following main functionalities:
1. [Hashmap Creation - `make__hashmap()`](#create-the-hashmap)
2. [Hashmap Insertion - `insert__hashmap()`](#insert-into-the-hashmap)
3. [Hashmap Selection - `get__hashmap()`](#select-from-hashmap)
4. [Hashmap Deletion - `delete__hashmap()`](#delete-from-hashmap)
5. [Hashmap Destruction - `deepdestroy__hashmap()`](#destroy-the-hashmap)

# create the hashmap
The first part is creating a hashmap by including the `hashmap.h` header in your file with:

```C
#include "hashmap.h"
```

The next component is defining the hashmap. Hashmap will have three parameters following the design: hash type, printer, and destroy, which can then be created with:

```C
hashmap *mymap = make__hashmap(hash type, printer, destroy);
```

The parameters follow the below definitions:

## **hash type**
takes either a `0` or a `1` and will create the hashmap based on that specification:
    - 0: REPLACES duplicates. If the key is seen, this will overwrite
		the previous value at that key
    - 1: STRUCTURES duplicates. If the inputted key is already in the
		hashmap, this will create an array at that key and store both
		values (this can hold as many values as the user inserts)
## **printer**
an inputted function that can take the values inserted into the hashmap and print them. Due to the hashmap using `void *` for values, this is required for seeing the outputted information.
## **destroy**
an inputted function that handles the removal of the value inside of the hashmap. The simplest version would be the use of a `char *` as the value. Creating a `destroy` function in that scenario would look like:

```C
void destroyObject(void *str) {
   free((char *)str);
}
```
A similar pattern can be used for creating a `printer` function

# insert into the hashmap
Insert hashmap involves taking a key (**uses only strings currently**) and a value and creating a new linked list node in a specific bucket. Here's a simple example of this using a `char *` as the value. For this example, assume that **hash type 0** is in use. Also of note: ***make sure*** that the value is allocated to ensure that no invalid frees occur at destruction. As for the keys, any type of key can theoretically be used, but different functinos will be added to ensure that the key can work.

<a name="insert_char_hashtype0"></a>
###### insert into hash type 0 with `char *`
With a normal `char *` unallocated key, the following features can be used, with the fourth parameter `"-d"` means _default_ behavior, which uses a simple print, and comparer function for the keys (which can be seen in the [hashmap.c](https://github.com/charlie-map/hashmapC/blob/overloadFunction/hashmap.c) function). The two example shows two insertions with `char *`'s, which will also show the use of `"-d"` and of `robust` manual insertion of the key functions. The `NULL` in the second `insert__hashmap` defines the "destroyKey" position, but the key in this scenario does not need a function. ***Important***: unless you are using a simple parameter such as `"-d"`, all **three** parameters need to be inserted.

```C
char *someKey = "123";

char *someRandomAllocatedValue = malloc(sizeof(char) * 5);
strcpy(someRandomAllocatedValue, "hey!");

insert__hashmap(mymap, someKey, someRandomAllocatedValue, "-d");

char *someOtherRandomAllocatedValue = malloc(sizeof(char) * 5);
strcpy(someOtherRandomAllocatedValue, "test");

insert__hashmap(mymap, someKey, someOtherRandomAllocatedValue, printCharKey, compareCharKey, NULL);
```

Note that using **hash type 0** in this scenario means that `someRandomAllocatedValue` is overwritten and thus deleted in this process by the hashmap. The same can be done for integer keys. For simplicity, the parameter `"-i"` can be used to define the use of integers. Comparing and contrasting both for integers:

```C
int someKey = 20;

char *someRandomAllocatedValue = malloc(sizeof(char) * 5);
strcpy(someRandomAllocatedValue, "hey!");

insert__hashmap(mymap, &someKey, someRandomeAllocatedValue, "-i");

char *someOtherRandomAllocatedValue = malloc(sizeof(char) * 5);
strcpy(someOtherRandomAllocatedValue, "test");

insert__hashmap(mymap, &someKey, someOtherRandomAllocatedValue, printIntKey, compareIntKey, NULL);
```
As for inserting with larger values such as structs, the following example shows utilizing the simple struct class called `Animal`:

<a name="insert_struct_hashtype0"></a>
###### insert into hash type 0 with a struct
```C
typedef struct Animal {
    char *animal_name;
    int animal_size;
} animal;
```
```C
char *someDifferentKey = "456";

animal *giraffe = malloc(sizeof(animal));
giraffe->animal_name = "Jefferey";
giraffe->animal_size = "400";

insert__hashmap(mymap, someDifferentKey, giraffe, "-d");
```

The same concept can be used for **hash type 1** where the values create an array at each location in the hashmap. The only change is instead creating the hashmap with `hash type = 0`, this would be `hash type = 1`:

###### insert into hash type 1 with `char *`
<a name="#insert_char_hashtype1"></a>
*Same insertion code again*

# select from hashmap
Select will utilize an input key to rederive the hashed key, and then return the value within that specific hashmap bucket. Select will look very similar to how insertion works as well. The main difference will be for each type:
1. [type 0](#selection_type1)
2. [type 1](#selection_type2)

<a name="selection_type1"></a>
## Selection type 0
Hash type 0 will always return a single `void *` value. This can be demonstrated with the following by selecting the value we inserted in the [hash type 0 insertion](#insert_char_hashtype1) example:

```C
void *getSingleValue = get__hashmap(mymap, "123");

printf("The value at '123' is %s\n", (char *) getSingleValue);
```
The expected result based on inputs will be `The value at '123' is test`

This same process is utilized for accessing a struct `void *` (based on the [struct insertion example](#insert_struct_hashtype0)):

```C
void *getSingleGiraffe = get__hashmap(mymap, "456");

printf("Values of giraffe at '456' is: name-%s, size-%d\n", ((animal *) getSingleGiraffe)->animal_name, ((animal *) getSingleGiraffe)->animal_size);
```
The expected result based on inputs will be `Values of giraffe at '456' is: name-Jeffery, size-400`

<a name="selection_type2"></a>
## Selection type 1
Selection for **hash type 1** will follow a similar pattern. However, the resulting type returned from `get__hashmap` will be a `void *`. This means that the returned result will need to know the length of this array. For this reason, a struct, `hashmap__response`, was made to containerize the return values:

```C
typedef struct ReturnHashmap {
	void **payload;
	int payload__length;
} hashmap__response;
```

Thus, to read the values from [hash type 1 insertion](#insert_char_hashtype1), the `hashmap__response` can be iterated through:
```C
hashmap__response *getMultiValue = (hashmap__response *) get__hashmap(mymap, "123");

for (int printResponse = 0; printResponse < hashmap__response->payload__length; printResponse++) {
    printf ("The value at '123' is %s\n", (char *) ((void **) hashmap__response->payload)[printResponse]);
}
```

This same concept can then be used for defining a struct print pattern.

# delete from hashmap
Similar to [selection](#select-from-hashmap), this takes a `key` and finds the `value` and removes the value (and `free()`s) from the hashmap bucket. The following shows deleting a singular key (using `someKeys` from the [insertion example](#insert_char_hashtype0)) from a hashmap. This will work for all types of values in the hashmap:

```C
delete__hashmap(mymap, someKeys);
```

# destroy the hashmap
Destroy will free any memory allocated within itself. This will include using the `destroy` function parameter given in the [creation of the hashmap](#define-the-hashmap). Using the following will destroy the entire hashmap:

```C
deepdestroy__hashmap(mymap);
```

# future components
Looking forward, refactors will come out occasionally to support ease of use and efficiency boosts. Look for new functionality such as `get keys`and `get values`
