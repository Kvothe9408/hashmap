// Standard Libraries
#include <stdio.h> // functions like printf
#include <stdlib.h> // functions like malloc
#include <string.h> // functions like strlen (string length), strcmp (string compare)

// Defintions

typedef struct Node Node; // declare the Node strcut to avoid self referencing in the next structu
typedef struct Node {
    char *key; // char is the string type in C / because this a a hashmap, the initial input value needs to be noted in the struct so the search can happen later a specific key/value pair
    char *value;
    struct Node *next;
} Node;
typedef struct linkedlist {
    Node *head;
} linkedlist;
typedef struct hashmap {
    linkedlist *list;
    int count; // counts the number of values that have been stored in the hashmap, which is needed for the load factor (count/capacity)
    int capacity; // indicates the capacity of the hashmap array, which is needed for the load factor
} hashmap;

// functions
unsigned int hash(char *key) { //unisgned int indicates that the number must stay positive, so instead of having a range of -2000 -> 2000 it's 0 -> 4000 / char is C for string variables
    int result = 5381; // defining interger variable / 5381 is standrad for this type of hash function
    int index = 0; // defining index
    while(key[index] != '\0') { // loop that keeps going as long as the key doesn't equal \0 how string arrays terminate in C / note all strings in C are just an array of numbers
        result = result * 31; // 31 is standard prime to use for this type of hash function as it has shown to distribute values better
        result = result + key[index]; // after the multiplication we add the result with the current int value at key[index]
        index++; // increment the index count
    } 
    return result; // returns the result / mod happens in the function itself once, the capacity is defined
}
hashmap *create_hashmap(int capacity) { // as noted, this is a version of an array
    hashmap *hash = (hashmap *)malloc(sizeof(hashmap)); // memory allocation for the struct
    if (hash == NULL) { // check if the memory failed to allocate
        printf("memory allocation failed"); 
        return NULL;
    }
    hash->list = (linkedlist *)malloc(sizeof(linkedlist) * capacity); // enter through the hash->list pointer and allocate memory for the array of linkedlist pointers
    if (hash->list == NULL) { // check is the memory allocation failed
        printf("memory allocation failed");
        return NULL;
    }
    int index = 0;
    while (index != capacity) { // this while loop re-assigns the linked list head in each slot in the hashmap array to NULL
        hash->list[index].head = NULL; // enter through hash->list pointer and reveal value at current index. Since the value at the index is a struct go to that structs head and set to NULL
        index++; // increment index
    }
    hash->count = 0; // set the count of the hashmap to 0
    hash->capacity = capacity; // set the hashmap capacity to the variable capacity;
    return hash; // return the hashmap pointer
}
hashmap *rehash(hashmap *old_hash) { // function to expand the hashmap, if the load factor gets to high. Will be nested in the insert function
    hashmap *new_hash = (hashmap *)malloc(sizeof(hashmap)); // memory allocation for the struct
    if (new_hash == NULL) { // NULL check
        printf("memory allocation failed");
        return NULL;
    }
    new_hash->list = (linkedlist *)malloc(sizeof(linkedlist) * old_hash->capacity * 2); // memory allocation for the new hashmap
    if (new_hash->list == NULL) { // NULL check
        printf("memory allocation failed");
        return NULL;
    }
    new_hash->capacity = old_hash->capacity * 2;
    int index_rehash = 0;
    while (index_rehash != new_hash->capacity) { // loop that is true while the index hasn't reached hashmap capacity yet
        new_hash->list[index_rehash].head = NULL; // within the new hashmap, go to list at "index" and set the head to NULL
        index_rehash++; // increment index by +1
    }
    int index_hash = 0;
    while (index_hash != old_hash->capacity) {
        Node *current = old_hash->list[index_hash].head;
        while (current != NULL) {
            unsigned int result = hash(current->key);
            int result_mod = result % new_hash->capacity;
            index_rehash = result_mod;
            Node *node = (Node*)malloc(sizeof(Node));
            if (node == NULL) {
                printf("memory allocation failed");
                return NULL;
            }
            node->value = current->value;
            node->key = current->key;
            node->next = new_hash->list[index_rehash].head;
            new_hash->list[index_rehash].head = node;
            Node *delete = current;
            current = current->next;
            free(delete);
        }
        index_hash++;
    }
    free(old_hash->list);
    free(old_hash);
    return new_hash;
}
// insert()
// get()
// delete ()

// print ()