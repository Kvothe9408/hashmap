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
unsigned int hash(char *key) {
    int result = 5381;
    int index = 0;
    while(key[index] != '\0') {
        result = result * 31;
        result = result + key[index];
        index++;
    } 
    return result;
}
// create_hashmap()