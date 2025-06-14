#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int *items;   
    int count;  
    int capacity;
} PriorityQueue;

typedef PriorityQueue *PQPointer;

// Αρχικοποίηση της ουράς προτεραιότητας
PQPointer QUEUEinit(int maxN)
{
    PQPointer pq = malloc(sizeof(PriorityQueue));
    pq->items = malloc(sizeof(int) * (maxN + 1));
    pq->count = 0;
    pq->capacity = maxN;
    return pq;
}

// Επιστρέφει true αν η ουρά είναι άδεια
int QUEUEempty(PQPointer pq)
{
    return pq->count == 0;
}

// Συνάρτηση για την ανύψωση του στοιχείου
void swim(int *items, int k)
{
    while (k > 1 && items[k / 2] < items[k])
    {
        int temp = items[k];
        items[k] = items[k / 2];
        items[k / 2] = temp;
        k = k / 2;
    }
}

// Εισαγωγή στοιχείου στην ουρά προτεραιότητας
void QUEUEput(PQPointer pq, int item)
{
    if (pq->count >= pq->capacity)
    {
        printf("Priority Queue is full\n");
        return;
    }
    pq->items[++pq->count] = item;
    swim(pq->items, pq->count);
}

// Συνάρτηση για την καθίζηση του στοιχείου
void sink(int *items, int k, int N)
{
    while (2 * k <= N)
    {
        int j = 2 * k;
        if (j < N && items[j] < items[j + 1])
            j++;
        if (!(items[k] < items[j]))
            break;
        int temp = items[k];
        items[k] = items[j];
        items[j] = temp;
        k = j;
    }
}

// Ανάκτηση του μεγαλύτερου στοιχείου από την ουρά
int QUEUEget(PQPointer pq)
{
    if (QUEUEempty(pq))
    {
        printf("Priority Queue is empty\n");
        return -1;
    }
    int max = pq->items[1];
    pq->items[1] = pq->items[pq->count--];
    sink(pq->items, 1, pq->count);
    return max;
}

int main()
{
    PQPointer pq = QUEUEinit(10);
    QUEUEput(pq, 10);
    QUEUEput(pq, 20);
    QUEUEput(pq, 15);

    printf("Max item: %d\n", QUEUEget(pq));
    printf("Next max item: %d\n", QUEUEget(pq));

    free(pq->items);
    free(pq);

    return 0;
}
