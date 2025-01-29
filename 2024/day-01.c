#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

struct node {
    int value;
    int count_left;
    int count_right;
    struct node *next;
};

struct hashTable {
    struct node **entry;
};

size_t hashFunction(int value, size_t capacity) {
    return value % capacity;
}

void merge(int *list, size_t left, size_t mid, size_t right) {
    // use temporary arrays so original array can be used as "new list"
    size_t left_size = mid - left + 1; // offset 0-indexing
    size_t right_size = right - mid;
    
    int *tempL = malloc(left_size * sizeof(int));
    int *tempR = malloc(right_size * sizeof(int));

    assert(tempL && tempR);

    // move data to new temporary arrays
    // remember: memmove moves bytes!
    memmove(tempL, list + left, left_size * sizeof(int)); // left moves as well is not fixed to first list entry
    memmove(tempR, list + mid + 1, right_size * sizeof(int));

    // zip both sub-lists according to shortest
    size_t lidx = 0, ridx = 0, list_index = left;
    while (lidx < left_size && ridx < right_size) {
        if (tempL[lidx] <= tempR[ridx]) {
            list[list_index] = tempL[lidx];
            lidx++;
        } else {
            list[list_index] = tempR[ridx];
            ridx++;
        }
        list_index++;
    }

    // any entries left in left sub-array?
    while (lidx < left_size) {
        list[list_index] = tempL[lidx];
        list_index++;
        lidx++;
    }

    // any entries left in right sub-array?
    while (ridx < right_size) {
        list[list_index] = tempR[ridx];
        list_index++;
        ridx++;
    }

    free(tempL);
    free(tempR);
}

void mergeSort(int *list, size_t left, size_t right) {
    // leave array alone, if condition is not satisfied anymore because array has size one then
    if (left < right) {
        size_t mid = (right + left) / 2;

        // IMPORTANT: don't hardcode left as 0
        mergeSort(list, left, mid);
        mergeSort(list, mid + 1, right); // offset 0-indexing

        // IMPORTANT: don't hardcode left as 0
        merge(list, left, mid, right);
    }
}

int main(void) {
    // hardcoded from input file
    static size_t size = 1000;
    int left[size];
    int right[size];

    FILE *input = fopen("input-01.txt", "r");
    assert(input);
    int a, b;
    size_t actual_size = 0;
    while (fscanf(input, "%d %d", &a, &b) == 2) {
        left[actual_size] = a;
        right[actual_size] = b;
        actual_size++;
    }

    mergeSort(left, 0, actual_size - 1);
    mergeSort(right, 0, actual_size - 1);

    int summedDifference = 0;
    for (size_t i = 0; i < actual_size; i++) {
        summedDifference += (abs(left[i] - right[i]));
    }

    printf("Part 1: %d\n", summedDifference);
    
    /* This is majorly inefficient. I walk over both lists anyhow but 
        additionally need to walk over the hashmap in the end as well
        instead of simply looping over list left and then list right.
        But hey, practiced hash maps again :)
    */

    // error checking omitted
    size_t binSize = 500;
    struct hashTable *table = calloc(1, sizeof(struct hashTable));
    table->entry = calloc(binSize, sizeof(struct node*));

    // build hash table
    struct node *head = NULL;
    for (size_t i = 0; i < actual_size; i++) {
        size_t index = hashFunction(left[i], binSize);

        head = table->entry[index];

        while (head != NULL) {
            if (head->value == left[i]) {
                head->count_left++;
                break;
            } else {
                head = head->next;
            }
        }
        
        if (head == NULL) {
            // insert new node
            struct node *new = calloc(1, sizeof(struct node));
            new->value = left[i];
            new->count_left = 1;
            new->count_right = 0;
            new->next = NULL;

            // depending on whether the starting entry is NULL, we can deduce where to append
            if (table->entry[index] == NULL) {
                table->entry[index] = new;
            } else {
                // append to head
                new->next = table->entry[index];
                table->entry[index] = new;
            }
        }
    }

    // insert from right list (i.e. build histogram)
    for (size_t i = 0; i < actual_size; i++) {
        size_t index = hashFunction(right[i], binSize);
        struct node *head = table->entry[index];
        while (head != NULL) {
            if (head->value == right[i]) {
                head->count_right++;
                break;
            }
            head = head->next;
        }
    }

    // compute total difference
    int totalDifference = 0;
    for (size_t i = 0; i < binSize; i++) {
        struct node *head = table->entry[i];
        while (head != NULL) {
            totalDifference += (head->value * head->count_left * head->count_right);
            head = head->next;
        }
    }
    printf("Part 2: %d\n", totalDifference);

    // free hash table
    struct node *tmp;
    for (size_t i = 0; i < binSize; i++) {
        head = table->entry[i];
        while (head != NULL) {
            tmp = head->next;
            free(head);
            head = tmp;
        }
    }
    free(table->entry);
    free(table);

    return 0;
}