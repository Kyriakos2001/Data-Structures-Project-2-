#define MAXCOUNT 10
typedef int PQItem;
typedef PQItem PQArray[MAXCOUNT+1];

typedef struct {
   int Count;
   PQArray ItemArray;
} PriorityQueue;
