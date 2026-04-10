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
} hashmap; // found out later that the load factor could have been included in the struct, probably should be instead of being hardcoded in the functions

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
hashmap *rehash(hashmap *old_hash) { // function to expand the hashmap, if the load factor gets to high. Will be nested in the insert function, had to create the variable old_hash so there isn't a conflict with the funciton call hash
    hashmap *new_hash = (hashmap *)malloc(sizeof(hashmap)); // memory allocation for the struct, needed new_hash for the same reason as old_hash
    if (new_hash == NULL) { // NULL check
        printf("memory allocation failed");
        return NULL;
    }
    new_hash->list = (linkedlist *)malloc(sizeof(linkedlist) * old_hash->capacity * 2); // memory allocation for the new hashmap
    if (new_hash->list == NULL) { // NULL check
        printf("memory allocation failed");
        return NULL;
    }
    new_hash->capacity = old_hash->capacity * 2; // setting the capacity of the new hashmap to double of the old hashmap
    int index_rehash = 0; // new variable for index that I will be using to increment walk through the hashmap of linkedlists
    while (index_rehash != new_hash->capacity) { // loop that is true while the index hasn't reached hashmap capacity yet
        new_hash->list[index_rehash].head = NULL; // within the new hashmap, go to list at "index" and set the head to NULL
        index_rehash++; // increment index by +1
    }
    int index_hash = 0;
    while (index_hash != old_hash->capacity) { // loop to move through the hashmap and rehash all the values into the new hashmap
        Node *current = old_hash->list[index_hash].head; // creating a node pointer that can walk through the linkedlists
        while (current != NULL) { // loop rehash value from the old to the new hashmap
            unsigned int result = hash(current->key); // creates variable 'result' and sets it to the hashed value of the key being used from teh original hashmap
            int result_mod = result % new_hash->capacity; // creates a variable 'result_mod' and sets it to the mod of result but using the capacity of the new hashmap
            index_rehash = result_mod; // sets the variable index_rehash to result_mod, so we can use it to place the key/value pair in the right linkedlist in the new hashmap
            Node *node = (Node*)malloc(sizeof(Node)); // creates a memory alloaction for a Node struct
            if (node == NULL) { // NULL check
                printf("memory allocation failed");
                return NULL;
            }
            node->value = current->value; // places value from the node in the old hashmap into the value field of the newly created node
            node->key = current->key; // ditto for the key
            node->next = new_hash->list[index_rehash].head; // sets the node's next pointer to the previous head of the linkedlist, as we are pushing this new node to the fornt of the list
            new_hash->list[index_rehash].head = node; // setting the linkedlist head pointer to point at the new node
            current = current->next; // increment the current pointer to the next node in the original hashmap
            Node *delete = current; // create pointer delete and set to current, so we delete the node form the original hashmap as we move along the linkedlist during the rehash
            free(delete); // free the node from the original hashmap linkedlist
        }
        index_hash++; // increment the index of the original hashmap by 1
    }
    free(old_hash->list); // free up the memory for the old hashmap
    free(old_hash); // free up the memory struct for the olde hashmap
    return new_hash; // return the new hashmap with the rehashed data
}

hashmap *insert(hashmap *hash_old, char *key, char *value) { // function to insert a new value into the hashmap / don't forget that there can be naming conflicts with variables and functions like the hash function and then variables that are in place of the hashmap pointer
    unsigned int result = hash(key); // result is unsigned int because it can't be negative for the mod to work within the hash function
    int result_mod = result % hash_old->capacity; // calculating the mod of the hashed result
    int index_hash = result_mod; // setting the result value to the index value to use below
    Node *node = (Node*)malloc(sizeof(Node)); // creating memory allocation for the node that will hold the new data
    node->next = hash_old->list[index_hash].head; //setting the new node next value to the header of the previous node at the front of the linkedlist
    hash_old->list[index_hash].head = node; // setting the head pointer fo the linkedlist to point at the head of the new node
    node->value = value; // placing the value into the new node
    node->key = key; // placing the key into the new node
    hash_old->count++; // incrementing the count of the hashmap
    if ((float) hash_old->count/hash_old->capacity > 0.75) { // checking he load factor of the hashmap and determining if we need to rehash the hashmap / note that the load factor is hardcoded into the function but probably could and should be included in the hashmap struct / also note that one of the variables in the equation needs to be cast (temporarily change its data type) as float, so that C doesn't truncate the result and gives a decimal value
        hash_old = rehash(hash_old); // rehash is the load factor is too high
        return hash_old; // return the old hash if not
    }
    return hash_old; // return the hashmap pointer, which now points at the new rehashed hashmap
}

char *get(hashmap *current_hash, char *key) { //get function
    unsigned int result = hash(key); // hasing key to find index
    int result_mod = result % current_hash->capacity;  // mod
    int index_hash = result_mod;
    Node *current_head = current_hash->list[index_hash].head; // walk thgouth node
    if (current_head == NULL) { // NULL check for the linekedlist before starting the walk through
        printf("not found");
        return NULL;
        }
    while (strcmp(key, current_head->key) != 0) { // strcmp is the functionality taht checks whether two strings are equal / while loop to find the matching key within the nodes of the linkedlist
        current_head = current_head->next; // node walk through
        if (current_head == NULL) { // NULL check after every walk to make sure we stop when the linkedlist terminates
            printf("not found");
            return NULL;
        }
    }
    return current_head->value; // return the value of the key value pair
}

void delete(hashmap *current_hash, char *key) {
    unsigned int result = hash(key);
    int result_mod = result % current_hash->capacity;
    int index_hash = result_mod;
    Node *current_delete = current_hash->list[index_hash].head;
    if (current_delete == NULL) {
        printf("not found");
        return;
    }
    Node * current_2 = NULL;
    while (strcmp(key, current_delete->key) != 0) {
        current_2 = current_delete;
        current_delete = current_delete->next;
        if (current_delete == NULL) {
            printf("not found");
            return;
        }
    }
    if (current_2 == NULL) {
        current_hash->list[index_hash].head = current_delete->next;
        free(current_delete);
        return;
    }
    current_2->next = current_delete->next;
    free(current_delete);
}

void print(hashmap *current_hash, char *key) {
    int index_hash = 0;
    unsigned int result = hash(key);
    int result_mod = result % current_hash->capacity;
    printf("%s\n", key);
    printf("%d\n", result_mod);
    printf("\n");
    Node *current_node = NULL;
    for (index_hash = 0; index_hash != current_hash->capacity; index_hash++) {
        printf("%d\n", index_hash);
        current_node = current_hash->list[index_hash].head;
        if (current_node == NULL) {
            printf("no data");
            continue;
        }
        while (current_node != NULL) {
            printf("%s\n", current_node->key);
            printf("%s\n", current_node->value);
            printf("\n");
            current_node = current_node->next;
        }    
    }
}

int main(void) {
    return 0;
}