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
hashmap *create_hashmap(int capacity) { 
    hashmap *hash = (hashmap *)malloc(sizeof(hashmap));
    if (hash == NULL) {
        printf("memory allocation failed");
        return NULL;
    }
    hash->list = (linkedlist *)malloc(sizeof(linkedlist) * capacity);
    if (hash->list == NULL) {
        printf("memory allocation failed");
        return NULL;
    }
    int index = 0;
    while (index != capacity) {
        hash->list[index].head = NULL;
        index++;
    }
    hash->count = 0;
    hash->capacity = capacity;
    return hash;
}